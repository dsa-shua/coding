#include <iostream>

int main()
{
    int x = 42;

    int& ref = x;   // reference
    int* ptr = &x;  // pointer

    std::cout << "x = " << x << std::endl;
    std::cout << "ref = " << ref << std::endl;
    std::cout << "*ptr = " << *ptr << std::endl;

    ref = 100;
    std::cout << "After changing ref, x = " << x << std::endl;

    *ptr = 200;
    std::cout << "After changing *ptr, x = " << x << std::endl;

    return 0;
}