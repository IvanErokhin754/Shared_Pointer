#include <iostream>
#include "SharedPtr.hpp"

int main()
{
    SharedPtr<int> a(new int[10], 10);

    std::cout << a[9];
    return 0;
}