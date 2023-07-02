#include <lib/number.h>
#include <iostream>

int main(){
    uint2022_t a = from_string("9034142571024269");
    uint2022_t b = from_string("90341423");
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a + b = " << (a + b) << std::endl;
    std::cout << "a - b = " << (a - b) << std::endl;
    std::cout << "a * b = " << (a * b) << std::endl;
    std::cout << "a / b = " << (a / b) << std::endl;
    std::cout << "a == b is " << (a == b) << std::endl;
    std::cout << "a != b is " << (a != b) << std::endl;
    std::cout << "a > b is " << (a > b) << std::endl;
    std::cout << "a < b is " << (a < b) << std::endl;
    return 0;
}