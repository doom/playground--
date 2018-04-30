#include <cassert>
#include "delta_ptr.hpp"

int main()
{
    int arr[5] = {0, 1, 2, 3, 4};
    doom::delta_ptr<int> ptr(arr);

    assert(!ptr.is_overflowing());

    ++ptr;
    assert(!ptr.is_overflowing());

    ++ptr;
    assert(!ptr.is_overflowing());

    ++ptr;
    assert(!ptr.is_overflowing());

    ++ptr;
    assert(!ptr.is_overflowing());

    ++ptr;
    assert(ptr.is_overflowing());
//    *ptr = 1; //Will trigger a general protection fault

    --ptr;
    assert(!ptr.is_overflowing());

    doom::delta_ptr<int> ptr2 = arr;
    assert(!ptr2.is_overflowing());

    ptr2 += 3;
    assert(!ptr2.is_overflowing());

    ptr2 = ptr2 + 2;
    assert(ptr2.is_overflowing());

    ptr2--;
    assert(ptr == ptr2);

    ptr2 = arr;
//    ptr2[5] = 1; //Will trigger a general protection fault

    return 0;
}
