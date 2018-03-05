#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <functional>
#include <sstream>

template <typename T>
struct Talking
{
public:
    Talking(T t) : _t(std::move(t))
    {
        std::cout << "Creating" << std::endl;
    }

    operator T &() noexcept
    {
        return _t;
    }

private:
    T _t;
};

namespace lazy
{
    template <typename T>
    class Value
    {
    private:
        using BuilderT = std::function<T()>;

        template <typename U, typename NoRefT = std::remove_reference_t<U>>
        using FuncCallDecay = std::conditional_t<std::is_array<NoRefT>::value, std::remove_extent_t<NoRefT> *, NoRefT>;

    public:
        template <typename FuncT, std::enable_if_t<std::is_convertible_v<FuncT, BuilderT>, bool> = true>
        explicit Value(FuncT f) : _builder{std::move(f)}
        {
        }

        template <typename ...Args, std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = true>
        explicit Value(Args &&...args) : _builder{std::bind([](std::tuple<FuncCallDecay<Args>...> &tuple) {
            return std::make_from_tuple<T>(std::move(tuple));
        }, std::make_tuple(std::forward<Args>(args)...))}
        {
        }

        template <typename U, std::enable_if_t<std::is_constructible_v<T, std::initializer_list<U>>, bool> = true>
        explicit Value(std::initializer_list<U> list) : _builder{std::bind([](std::initializer_list<U> &l) {
            return l;
        }, list)}
        {
        }

        T operator()() const
        {
            return _builder();
        }

        constexpr auto operator+(const lazy::Value<T> &b)
        {
            return lazy::Value<T>([a = *this, b]() {
                return a() + b();
            });
        }

        constexpr auto operator-(const lazy::Value<T> &b)
        {
            return lazy::Value<T>([a = *this, b]() {
                return a() - b();
            });
        }

    private:
        BuilderT _builder;
    };

    template <typename T, typename ...Args>
    constexpr auto value(Args &&...args)
    {
        return Value<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    constexpr auto ref(T &t)
    {
        return Value<std::add_lvalue_reference_t<T>>(t);
    }
}

template <typename T>
static inline constexpr auto operator<<(std::ostream &os, const lazy::Value<T> &v)
{
    return lazy::Value<std::ostream &>([&os, v]() -> std::ostream & {
        return os << v();
    });
}

template <typename T, typename U>
static inline constexpr auto operator<<(lazy::Value<T &> r, const lazy::Value<U> &v)
{
    return lazy::Value<T &>([r, v]() -> T & {
        return r() << v();
    });
}

template <typename T, typename U>
static inline constexpr auto operator<<(lazy::Value<T &> r, const U &v)
{
    return lazy::Value<T &>([r, v]() -> T & {
        return r() << v;
    });
}

int main()
{
    auto num = lazy::value<int>(2);
    auto str = lazy::value<Talking<std::string>>("Will never be constructed");
    std::cout << "num: " << num() << std::endl;

    auto x1 = lazy::value<int>(32);                 //Not evaluated
    auto x2 = lazy::value<int>(12);                 //Not evaluated
    auto sum = x1 + x2;                             //Still not evaluated

    std::cout << "sum: " << sum() << std::endl;     //Evaluated

    auto vec1 = lazy::value<std::vector<int>>(std::initializer_list<int>{1, 2, 3});
    auto vec2 = lazy::Value<std::vector<int>>{1, 2, 3};

    auto outputIntToStream = [](std::ostream &os, const char *name, const lazy::Value<int> &n) {
        return os << lazy::value<std::string>(name) << ": " << n << " ";
    };

    std::ostringstream oss;

    auto outputNum = outputIntToStream(oss, "num", num);
    auto outputX1 = outputIntToStream(oss, "x1", x1);

    outputX1();
    std::cout << "stream contents: '" << oss.str() << "'" << std::endl;
    outputNum();
    std::cout << "stream contents: '" << oss.str() << "'" << std::endl;

    std::swap(outputNum, outputX1);
    oss.str("");

    outputX1();
    std::cout << "stream contents: '" << oss.str() << "'" << std::endl;
    outputNum();
    std::cout << "stream contents: '" << oss.str() << "'" << std::endl;

}
