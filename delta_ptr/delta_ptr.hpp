//
// Created by doom on 26/04/18.
//

#ifndef DELTA_PTR_HPP
#define DELTA_PTR_HPP

#include <cstddef>
#include <cstdint>
#include <ostream>
#ifdef DEBUG
#include <iostream>
#include <bitset>
#endif

/*
 * C++ implementation of Delta Pointers
 * As explained in https://www.cs.vu.nl/~herbertb/download/papers/delta-pointers_eurosys18.pdf
 *
 * Just a PoC, not meant to be used seriously
 */

#ifdef DEBUG
#define __DO_DBG(...)     __VA_ARGS__
#else
#define __DO_DBG(...)
#endif

namespace doom
{
    template <typename T>
    class delta_ptr
    {
    private:
        static constexpr T *_mask_ptr(T *ptr, size_t size) noexcept
        {
            auto n = (uintptr_t)ptr;
            auto tag = (1UL << 15) - size;
            n |= tag << 48;
            return (T *)n;
        }

        static constexpr T *_unmask_ptr(T *ptr) noexcept
        {
            auto n = (uintptr_t)ptr;
            n &= 0x8000ffffffffffff;
            return (T *)n;
        }

        static constexpr T *_addOffset(T *ptr, size_t size) noexcept
        {
            auto n = (uintptr_t)ptr;
            auto tag = (uint16_t)(n >> 48);
            tag += size;
            n &= 0x0000ffffffffffff;
            n |= (uintptr_t)tag << 48;
            return (T *)n;
        }

        static constexpr T *_subOffset(T *ptr, size_t size) noexcept
        {
            auto n = (uintptr_t)ptr;
            auto tag = (uint16_t)(n >> 48);
            tag -= size;
            n &= 0x0000ffffffffffff;
            n |= (uintptr_t)tag << 48;
            return (T *)n;
        }

    public:
        delta_ptr(T *ptr, size_t size) noexcept : _ptr(_mask_ptr(ptr, size))
        {
        }

        template <size_t N>
        delta_ptr(T (&arr)[N]) noexcept : _ptr(_mask_ptr(arr, N))
        {
        }

        delta_ptr(const delta_ptr &other) noexcept = default;
        delta_ptr(delta_ptr &&other) noexcept = default;

        delta_ptr &operator=(const delta_ptr &other) noexcept = default;
        delta_ptr &operator=(delta_ptr &&other) noexcept = default;

        template <size_t N>
        delta_ptr &operator=(T (&arr)[N]) noexcept
        {
            _ptr = _mask_ptr(arr, N);
            return *this;
        }

        T *get() const noexcept
        {
            return _unmask_ptr(_ptr);
        }

        T &operator*() const noexcept
        {
            return *_unmask_ptr(_ptr);
        }

        T *operator->() const noexcept
        {
            return _unmask_ptr(_ptr);
        }

        T &operator[](size_t index) const noexcept
        {
            return *_unmask_ptr(_addOffset(_ptr, index));
        }

        delta_ptr &operator++() noexcept
        {
            _ptr = _addOffset(_ptr, 1);
            return *this;
        }

        delta_ptr operator++(int) noexcept
        {
            auto cpy = *this;

            ++(*this);
            return cpy;
        }

        delta_ptr &operator+=(size_t n) noexcept
        {
            _ptr = _addOffset(_ptr, n);
            return *this;
        }

        delta_ptr operator+(size_t n) const noexcept
        {
            auto cpy = *this;

            cpy += n;
            return cpy;
        }

        delta_ptr &operator--() noexcept
        {
            _ptr = _subOffset(_ptr, 1);
            return *this;
        }

        delta_ptr operator--(int) noexcept
        {
            auto cpy = *this;

            --(*this);
            return cpy;
        }

        delta_ptr &operator-=(size_t n) noexcept
        {
            _ptr = _subOffset(_ptr, n);
            return *this;
        }

        delta_ptr operator-(size_t n) const noexcept
        {
            auto cpy = *this;

            cpy -= n;
            return cpy;
        }

        bool is_overflowing() const noexcept
        {
            __DO_DBG(std::cout << "Pointer: " << std::bitset<64>((uintptr_t)_unmask_ptr(_ptr)) << std::endl);
            __DO_DBG(std::cout << "Tag: " << std::bitset<16>(((uint16_t)((uintptr_t)_ptr >> 48))) << "\n" << std::endl);
            return ((uintptr_t)_ptr & (1UL << 63)) != 0;
        }

        bool operator==(const delta_ptr &other) const noexcept
        {
            return _ptr == other._ptr;
        }

        bool operator!=(const delta_ptr &other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const delta_ptr &other) const noexcept
        {
            return _ptr < other._ptr;
        }

        bool operator<=(const delta_ptr &other) const noexcept
        {
            return _ptr <= other._ptr;
        }

        bool operator>(const delta_ptr &other) const noexcept
        {
            return _ptr > other._ptr;
        }

        bool operator>=(const delta_ptr &other) const noexcept
        {
            return _ptr >= other._ptr;
        }

        friend std::ostream &operator<<(std::ostream &os, const delta_ptr &ptr) noexcept
        {
            return os << ptr._ptr;
        }

    private:
        T *_ptr;
    };
}

#undef __DO_DBG

#endif //DELTA_PTR_HPP
