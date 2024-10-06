#include <iostream>
#include <map>
#include "my_allocator.hpp"
#include "my_container.hpp"


const size_t CONTAINER_MAX_SIZE = 10;
const size_t ALLOCATOR_PAGE_SIZE = 20;


/**
    @brief Функция, вычисляющая факториал числа.
  
    Вычисляет факториал числа и помещает его промежуточные значения в заданный контейнер.
      
    @param[in] n число, для которого необходимо вычислить факториал    
    @param[out] map std::map    
 */
template <typename T>
void factorial_2map(int n, T& map)
{
    int result = 1;
    map[0] = 1;
    for (int i = 1; i < n; ++i) {
        result *= i;
        map[i] = result;
    }
}


template <typename T>
void print_map(const T& map)
{
    for (const auto& [num, fact] : map) {
        std::cout << num << " " << fact << std::endl;
    }
}


template <typename T>
void print_vector(const T& vector)
{
    for (const auto& value : vector) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}


int main(int, const char *[])
{ 
    std::cout << "1. std::map<int, int> container with std::allocator:" << std::endl;
    std::map<int, int, std::less<int>, std::allocator<std::pair<int, int>>> map_container_alloc_std;    
    factorial_2map(CONTAINER_MAX_SIZE, map_container_alloc_std);
    print_map(map_container_alloc_std);

    std::cout << std::endl << "2. std::map<int, int> container with custom MyAllocator:" << std::endl;
    std::map<int, int, std::less<int>, my::MyAllocator<std::pair<int, int>, ALLOCATOR_PAGE_SIZE>> map_container_calloc_ustom;        
    factorial_2map(CONTAINER_MAX_SIZE, map_container_calloc_ustom);
    print_map(map_container_calloc_ustom);
    
    std::cout << std::endl << "3. Custom container MyContainer<int> with std::allocator:" << std::endl;    
    my::MyContainer<int, std::allocator<int>> my_container_alloc_std;
    for (size_t i = 0; i < CONTAINER_MAX_SIZE; ++i) {
        my_container_alloc_std.push_back(i);
    }
    print_vector(my_container_alloc_std);
    
    std::cout << std::endl << "4. Custom container MyContainer<int> with custom MyAllocator:" << std::endl;    
    my::MyContainer<int, my::MyAllocator<int, ALLOCATOR_PAGE_SIZE>> my_container_alloc_custom;
    for (size_t i = 0; i < 10; ++i) {
        my_container_alloc_custom.push_back(i);
    }        
    print_vector(my_container_alloc_custom);    
    
    return 0;
}
