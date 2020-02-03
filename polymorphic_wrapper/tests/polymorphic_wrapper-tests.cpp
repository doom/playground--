/*
** Created by doom on 14/12/18.
*/

#include <gtest/gtest.h>
#include "polymorphic_wrapper.hpp"

namespace
{
    class thing
    {
    public:
        virtual ~thing() noexcept = default;

        [[nodiscard]] virtual int get_id() const noexcept = 0;

        virtual void set_id(int id) noexcept = 0;
    };

    class concrete_thing : public thing
    {
    private:
        int id_;

    public:
        explicit concrete_thing(int id) noexcept : id_(id)
        {
        }

        [[nodiscard]] int get_id() const noexcept override
        {
            return id_;
        }

        void set_id(int id) noexcept override
        {
            id_ = id;
        }
    };

    class floaty_thing : public thing
    {
    private:
        float id_;

    public:
        explicit floaty_thing(int id) noexcept : id_((float)id)
        {
        }

        [[nodiscard]] int get_id() const noexcept override
        {
            return (int)id_;
        }

        void set_id(int id) noexcept override
        {
            id_ = (float)id;
        }
    };
}

template <typename T>
using heap_polymorphic_wrapper = doom::polymorphic_wrapper<T>;

template <typename T>
using flat_polymorphic_wrapper = doom::polymorphic_wrapper<T, doom::flat_polymorphic_storage<T, 16, 16>>;

TEST(heap_poly, basic_test)
{
    heap_polymorphic_wrapper<thing> p(concrete_thing(1));

    ASSERT_EQ(p->get_id(), 1);

    auto q = p;
    ASSERT_EQ(q->get_id(), 1);

    q->set_id(2);
    ASSERT_EQ(p->get_id(), 1);
    ASSERT_EQ(q->get_id(), 2);

    auto r = std::move(q);
    ASSERT_EQ(r->get_id(), 2);
    r->set_id(3);
    ASSERT_EQ(r->get_id(), 3);

    p = r;
    ASSERT_EQ(p->get_id(), r->get_id());

    heap_polymorphic_wrapper<thing> s(floaty_thing(42));
    s = std::move(r);
    ASSERT_EQ(s->get_id(), 3);
}

TEST(flat_poly, basic_test)
{
    flat_polymorphic_wrapper<thing> p(concrete_thing(1));

    ASSERT_EQ(p->get_id(), 1);

    auto q = p;
    ASSERT_EQ(q->get_id(), 1);

    q->set_id(2);
    ASSERT_EQ(p->get_id(), 1);
    ASSERT_EQ(q->get_id(), 2);

    auto r = std::move(q);
    ASSERT_EQ(r->get_id(), 2);
    r->set_id(3);
    ASSERT_EQ(r->get_id(), 3);

    p = r;
    ASSERT_EQ(p->get_id(), r->get_id());

    flat_polymorphic_wrapper<thing> s(floaty_thing(42));
    s = std::move(r);
    ASSERT_EQ(s->get_id(), 3);
}
