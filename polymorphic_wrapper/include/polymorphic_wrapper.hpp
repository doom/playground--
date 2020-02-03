/*
** Created by doom on 03/04/19.
*/

#ifndef POLYMORPHIC_WRAPPER_HPP
#define POLYMORPHIC_WRAPPER_HPP

#include <memory>

namespace doom
{
    namespace details
    {
        template <typename T, typename U>
        concept same_as = std::is_same_v<T, U>;
    }

    template <typename Storage, typename Stored>
    concept PolymorphicStorage = requires(Storage t, const Storage u) {
        std::is_constructible_v<Storage, Stored> &&
        std::is_copy_constructible_v<Storage> &&
        std::is_move_constructible_v<Storage> &&
        std::is_copy_assignable_v<Storage> &&
        std::is_move_assignable_v<Storage>;
        { t.get() } -> details::same_as<Stored *>;
        { u.get() } -> details::same_as<const Stored *>;
    };

    namespace details
    {
        template <typename T>
        class erased_poly_storage
        {
        public:
            virtual ~erased_poly_storage() = default;

            [[nodiscard]] virtual T *get() noexcept = 0;

            [[nodiscard]] virtual const T *get() const noexcept = 0;

            [[nodiscard]] virtual std::unique_ptr<erased_poly_storage> clone() const = 0;

            [[nodiscard]] virtual std::unique_ptr<erased_poly_storage> move() = 0;
        };

        template <typename T, typename Concrete>
        class concrete_poly_storage : public erased_poly_storage<T>
        {
        private:
            Concrete c_;

            using this_type = concrete_poly_storage<T, Concrete>;

        public:
            explicit concrete_poly_storage(Concrete &&c) noexcept : c_(std::move(c))
            {
            }

            concrete_poly_storage(const concrete_poly_storage &) = default;

            concrete_poly_storage(concrete_poly_storage &&) noexcept = default;

            concrete_poly_storage &operator=(const concrete_poly_storage &) = default;

            concrete_poly_storage &operator=(concrete_poly_storage &&) noexcept = default;

            [[nodiscard]] T *get() noexcept override
            {
                return &c_;
            }

            [[nodiscard]] const T *get() const noexcept override
            {
                return &c_;
            }

            [[nodiscard]] std::unique_ptr<erased_poly_storage<T>> clone() const override
            {
                return std::make_unique<this_type>(*this);
            }

            [[nodiscard]] std::unique_ptr<erased_poly_storage<T>> move() override
            {
                return std::make_unique<this_type>(std::move(*this));
            }
        };
    }

    template <typename T>
    class heap_polymorphic_storage
    {
    private:
        template <typename Concrete>
        using concrete_storage = details::concrete_poly_storage<T, Concrete>;

        using erased_storage = details::erased_poly_storage<T>;

        std::unique_ptr<erased_storage> ptr_;

    public:
        template <typename Concrete>
        explicit heap_polymorphic_storage(Concrete &&c) noexcept requires (
            std::is_base_of_v<T, Concrete> &&
            not std::is_same_v<std::remove_reference_t<Concrete>, heap_polymorphic_storage<T>>
        ) :
            ptr_(std::make_unique<concrete_storage<Concrete>>(concrete_storage<Concrete>(std::forward<Concrete>(c))))
        {
        }

        heap_polymorphic_storage(const heap_polymorphic_storage &other) : ptr_(other.ptr_->clone())
        {
        }

        heap_polymorphic_storage(heap_polymorphic_storage &&other) noexcept : ptr_(other.ptr_->move())
        {
        }

        heap_polymorphic_storage &operator=(const heap_polymorphic_storage &other)
        {
            if (this != &other) {
                ptr_ = std::move(other.ptr_->clone());
            }
            return *this;
        }

        heap_polymorphic_storage &operator=(heap_polymorphic_storage &&other) noexcept
        {
            if (this != &other) {
                ptr_ = std::move(other.ptr_->move());
            }
            return *this;
        }

        [[nodiscard]] T *get() noexcept
        {
            return ptr_.get()->get();
        }

        [[nodiscard]] const T *get() const noexcept
        {
            return ptr_.get()->get();
        }
    };

    template <typename T, std::size_t MaxSize = sizeof(T), std::size_t MaxAlign = alignof(T)>
    class flat_polymorphic_storage
    {
    private:
        enum class operation
        {
            copy_construct,
            move_construct,
            copy_assign,
            move_assign,
            destroy,
        };

        static constexpr std::size_t max_size = MaxSize;
        static constexpr std::size_t max_align = MaxAlign;
        using Storage = std::aligned_storage_t<max_size, max_align>;

        Storage storage_;
        void (*handle_operation_)(operation, Storage *, Storage *);

        template <typename Concrete>
        static void handle_operation(operation op, Storage *s, Storage *other)
        {
            switch (op) {
                case operation::copy_construct:
                    new(s) Concrete(*reinterpret_cast<const Concrete *>(other));
                    break;
                case operation::move_construct:
                    new(s) Concrete(std::move(*reinterpret_cast<Concrete *>(other)));
                    break;
                case operation::copy_assign:
                    *reinterpret_cast<Concrete *>(s) = *reinterpret_cast<Concrete *>(other);
                    break;
                case operation::move_assign:
                    *reinterpret_cast<Concrete *>(s) = std::move(*reinterpret_cast<Concrete *>(other));
                    break;
                case operation::destroy:
                    reinterpret_cast<Concrete *>(s)->~Concrete();
                    break;
            }
        }

    public:
        template <typename Concrete>
        explicit flat_polymorphic_storage(Concrete &&ptr) noexcept requires (
            std::is_base_of_v<T, Concrete> &&
            not std::is_same_v<std::remove_reference_t<Concrete>, flat_polymorphic_storage<T>>
        ) :
            handle_operation_(&handle_operation<Concrete>)
        {
            static_assert(sizeof(Concrete) <= max_size);
            static_assert(alignof(Concrete) <= max_align);
            new(&storage_) Concrete(std::forward<Concrete>(ptr));
        }

        flat_polymorphic_storage(const flat_polymorphic_storage &other) : handle_operation_(other.handle_operation_)
        {
            handle_operation_(operation::copy_construct, &storage_, const_cast<Storage *>(&other.storage_));
        }

        flat_polymorphic_storage(flat_polymorphic_storage &&other) noexcept : handle_operation_(other.handle_operation_)
        {
            handle_operation_(operation::move_construct, &storage_, &other.storage_);
        }

        flat_polymorphic_storage &operator=(const flat_polymorphic_storage &other)
        {
            if (handle_operation_ == other.handle_operation_) {
                handle_operation_(operation::copy_assign, &storage_, const_cast<Storage *>(&other.storage_));
            } else {

                handle_operation_(operation::destroy, &storage_, nullptr);
                handle_operation_ = other.handle_operation_;
                handle_operation_(operation::copy_construct, &storage_, const_cast<Storage *>(&other.storage_));
            }
            return *this;
        }

        flat_polymorphic_storage &operator=(flat_polymorphic_storage &&other) noexcept
        {
            if (handle_operation_ == other.handle_operation_) {
                handle_operation_(operation::move_assign, &storage_, const_cast<Storage *>(&other.storage_));
            } else {
                handle_operation_(operation::destroy, &storage_, nullptr);
                handle_operation_ = other.handle_operation_;
                handle_operation_(operation::move_construct, &storage_, const_cast<Storage *>(&other.storage_));
            }
            return *this;
        }

        ~flat_polymorphic_storage() noexcept
        {
            handle_operation_(operation::destroy, &storage_, nullptr);
        }

        [[nodiscard]] T *get() noexcept
        {
            return reinterpret_cast<T *>(&storage_);
        }

        [[nodiscard]] const T *get() const noexcept
        {
            return reinterpret_cast<const T *>(&storage_);
        }
    };

    template <typename T, PolymorphicStorage<T> Storage = heap_polymorphic_storage<T>>
    class polymorphic_wrapper : private Storage
    {
    public:
        using Storage::Storage;

        polymorphic_wrapper(const polymorphic_wrapper &) = default;

        polymorphic_wrapper(polymorphic_wrapper &&) noexcept = default;

        polymorphic_wrapper &operator=(const polymorphic_wrapper &) = default;

        polymorphic_wrapper &operator=(polymorphic_wrapper &&) noexcept = default;

        using Storage::get;

        T *operator->() noexcept
        {
            return get();
        }

        const T *operator->() const noexcept
        {
            return get();
        }
    };
}

#endif /* !POLYMORPHIC_WRAPPER_HPP */
