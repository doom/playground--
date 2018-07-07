//
// Created by doom on 03/05/18.
//

#ifndef DOOM_CONSTEXPR_SET_HPP
#define DOOM_CONSTEXPR_SET_HPP

#include "details.hpp"

namespace doom
{
    template <typename T, size_t Size, typename Less = std::less<T>>
    class set
    {
    private:
        using Array = details::array<T, Size>;

    public:
        using value_type = typename Array::value_type;
        using reference = typename Array::reference;
        using const_reference = typename Array::const_reference;
        using pointer = typename Array::pointer;
        using const_pointer = const value_type *;

        using iterator = typename Array::iterator;
        using const_iterator = typename Array::const_iterator;
        using reverse_iterator = typename Array::reverse_iterator;
        using const_reverse_iterator = typename Array::const_reverse_iterator;

        using size_type = typename Array::size_type;
        using difference_type = typename Array::difference_type;

        constexpr set(const T (&arr)[Size], const Less &less = {}) noexcept :
            _isLess{less},
            _arr(details::sort(details::array<T, Size>(arr), _isLess))
        {
        }

        constexpr set(std::initializer_list<T> l, const Less &less = {}) noexcept :
            _isLess{less},
            _arr(details::sort(details::array<T, Size>(l), _isLess))
        {
        }

        constexpr auto begin() noexcept
        {
            return _arr.begin();
        }

        constexpr auto end() noexcept
        {
            return _arr.end();
        }

        constexpr auto cbegin() const noexcept
        {
            return _arr.cbegin();
        }

        constexpr auto cend() const noexcept
        {
            return _arr.cend();
        }

        constexpr auto begin() const noexcept
        {
            return _arr.begin();
        }

        constexpr auto end() const noexcept
        {
            return _arr.end();
        }

        constexpr auto rbegin() noexcept
        {
            return _arr.rbegin();
        }

        constexpr auto rend() noexcept
        {
            return _arr.end();
        }

        constexpr auto crbegin() const noexcept
        {
            return _arr.crbegin();
        }

        constexpr auto crend() const noexcept
        {
            return _arr.crend();
        }

        constexpr auto rbegin() const noexcept
        {
            return _arr.rbegin();
        }

        constexpr auto rend() const noexcept
        {
            return _arr.rend();
        }

        constexpr auto find(const T &value) const noexcept
        {
            return details::binary_search(_arr.begin(), _arr.end(), value, _isLess);
        }

        constexpr auto &at(const T &value) const
        {
            const auto it = find(value);

            if (it == end())
                throw std::out_of_range("doom::constexpr_set: out of range");
            return *it;
        }

        constexpr bool contains(const T &value) const noexcept
        {
            return find(value) != _arr.end();
        }

        constexpr size_type count(const T &value) const noexcept
        {
            return contains(value) ? 1 : 0;
        }

        constexpr size_type size() const noexcept
        {
            return _arr.size();
        }

        constexpr bool empty() const noexcept
        {
            return size() == 0;
        }

    private:
        Less _isLess;
        Array _arr;
    };
}

#endif //DOOM_CONSTEXPR_SET_HPP
