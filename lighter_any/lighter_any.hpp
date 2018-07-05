//
// Created by doom on 01/07/18.
//

#ifndef LIGHTER_ANY_HPP
#define LIGHTER_ANY_HPP

#include <typeinfo>
#include <cstddef>
#include <random>

namespace doom
{
    template <typename T>
    struct in_place_type_t
    {

    };

    template <size_t Size, size_t Align>
    struct aligned_storage
    {
        aligned_storage() noexcept
        {
        }

        aligned_storage(const aligned_storage &)
        {

        }

        aligned_storage &operator=(const aligned_storage &)
        {
            return *this;
        }

        alignas(Align) unsigned char data[Size];
    };

    class bad_any_cast : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            return exception::what();
        }
    };

    namespace details
    {
        enum holder_action
        {
            destroy,
            move_to,
            copy_to,
            nothing,
        };

        template <typename T>
        static const std::type_info &_holder_model([[maybe_unused]] holder_action to_do,
                                                   [[maybe_unused]] const void *storage, [[maybe_unused]] void *other)
        {
            if constexpr (!std::is_same_v<T, void>) {
                switch (to_do) {
                    case destroy:
                        ((T *)storage)->~T();
                        break;
                    case copy_to: {
                        auto to_ptr = reinterpret_cast<T *>(other);
                        auto from = reinterpret_cast<const T *>(storage);
                        new(to_ptr) T(*from);
                        break;
                    }
                    case move_to: {
                        auto to_ptr = reinterpret_cast<T *>(other);
                        auto from = (T *)storage;
                        new(to_ptr) T(std::move(*from));
                        break;
                    }
                    case nothing:
                        break;
                }
            }
            return typeid(T);
        }

        struct any_caster
        {
            template <typename T, typename AnyT>
            T do_cast(const AnyT *any)
            {
                using U = std::remove_cv_t<std::remove_reference_t<T>>;
                return *(U *)(any->_raw_storage());
            }
        };
    }

    template <size_t MaxSize, size_t MaxAlign>
    class lighter_any
    {
    public:
        static constexpr auto max_size = MaxSize;
        static constexpr auto max_align = MaxAlign;

    private:
        template <size_t OtherMaxSize, size_t OtherMaxAlign>
        friend
        class lighter_any;

        friend details::any_caster;

        using storage_t = doom::aligned_storage<max_size, max_align>;
        using holder_func_t = const std::type_info &(*)(details::holder_action, const void *, void *);

        storage_t _stor;
        holder_func_t _func;

        void *_raw_storage() noexcept
        {
            return reinterpret_cast<void *>(_stor.data);
        }

        const void *_raw_storage() const noexcept
        {
            return reinterpret_cast<const void *>(_stor.data);
        }

        void _destroy()
        {
            _func(details::destroy, _raw_storage(), nullptr);
        }

        void _copy_to(void *other) const
        {
            _func(details::copy_to, _raw_storage(), other);
        }

        void _move_to(void *other) const
        {
            _func(details::move_to, _raw_storage(), other);
        }

        template <typename T, typename ...Args>
        T &_construct(Args &&...args)
        {
            _func = &details::_holder_model<T>;
            return *new(_raw_storage()) T(std::forward<Args>(args)...);
        }

    public:
        lighter_any() : _func{&details::_holder_model<void>}
        {
        }

        template <typename T, typename ...Args>
        explicit lighter_any(in_place_type_t<T>, Args &&...args)
        {
            _construct<T>(std::forward<Args>(args)...);
        }

        template <size_t OMaxSize, size_t OMaxAlign,
            typename = std::enable_if_t<max_size >= OMaxSize && max_align >= OMaxAlign>>
        lighter_any(const lighter_any<OMaxSize, OMaxAlign> &other) : lighter_any()
        {
            *this = other;
        }

        template <size_t OMaxSize, size_t OMaxAlign,
            typename = std::enable_if_t<max_size >= OMaxSize && max_align >= OMaxAlign>>
        lighter_any(lighter_any<OMaxSize, OMaxAlign> &&other) noexcept : lighter_any()
        {
            *this = std::move(other);
        }

        template <size_t OMaxSize, size_t OMaxAlign,
            typename = std::enable_if_t<max_size >= OMaxSize && max_align >= OMaxAlign>>
        lighter_any &operator=(const lighter_any<OMaxSize, OMaxAlign> &other)
        {
            _destroy();
            if (other.has_value())
                other._copy_to(_raw_storage());
            _func = other._func;
            return *this;
        }

        template <size_t OMaxSize, size_t OMaxAlign,
            typename = std::enable_if_t<max_size >= OMaxSize && max_align >= OMaxAlign>>
        lighter_any &operator=(lighter_any<OMaxSize, OMaxAlign> &&other) noexcept
        {
            _destroy();
            if (other.has_value())
                other._move_to(_raw_storage());
            _func = other._func;
            return *this;
        }

        template <typename T, typename ...Args>
        std::decay_t<T> &emplace(Args &&...args)
        {
            _destroy();
            return _construct<T>(std::forward<Args>(args)...);
        }

        void reset() noexcept
        {
            _destroy();
            _func = &details::_holder_model<void>;
        }

        bool has_value() const noexcept
        {
            return type() != typeid(void);
        }

        const std::type_info &type() const noexcept
        {
            return _func(details::nothing, nullptr, nullptr);
        }
    };

    template <typename T, size_t AMaxSize, size_t AMaxAlign>
    T any_cast(const lighter_any<AMaxSize, AMaxAlign> *any)
    {
        if (typeid(std::remove_cv_t<std::remove_reference_t<T>>) != any->type())
            throw bad_any_cast();
        details::any_caster caster;
        return caster.do_cast<T>(any);
    }

    template <typename T, size_t AMaxSize, size_t AMaxAlign>
    T any_cast(const lighter_any<AMaxSize, AMaxAlign> &any)
    {
        return any_cast<T>(&any);
    }

}

#endif //LIGHTER_ANY_HPP
