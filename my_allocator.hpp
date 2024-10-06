#pragma once

#include <memory>
#include <vector>

namespace my {

using size_type = std::size_t;  

/**
    @brief Класс пользовательского аллокатора.
      
    Использует STL-вектор unique_ptr-указателей на выделенные блоки памяти.
    Размер блока определяется параметром MEM_PAGE_SIZE.
 */
template<typename T, size_type MEM_PAGE_SIZE=50>
class MyAllocator {
private:
    /// индекс текущего блока памяти
    size_type mem_cblock; 

    /// индекс курсора свободной области в текущем блоке
    size_type mem_cblock_cursor = 0; 

    /// вектор указателей на выделенные блоки памяти
    std::vector<std::unique_ptr<T[]>> mem_blocks; 

public:
    using value_type = T;       
    using pointer = T*;
    using const_pointer = const T*;
    
    MyAllocator() : mem_cblock(0), mem_cblock_cursor(0) {
        mem_blocks.emplace_back(std::make_unique<T[]>(MEM_PAGE_SIZE));           
    }

    ~MyAllocator() {           
       // нет необходимости в ручном освобождении памяти, т.к. используем unique_ptr-указатели
    }

    T* allocate(size_type n) {
        if (n > MEM_PAGE_SIZE) {
            throw std::bad_alloc();
        }
        
        if (mem_cblock >= mem_blocks.size() || (mem_cblock_cursor + n > MEM_PAGE_SIZE)) {                    
            mem_blocks.emplace_back(std::make_unique<T[]>(MEM_PAGE_SIZE));            
            ++mem_cblock;
            mem_cblock_cursor = 0;
        }
        
        T* result = mem_blocks[mem_cblock].get() + mem_cblock_cursor;
        mem_cblock_cursor += n;
        return result;
    }

    void deallocate(T*, std::size_t) noexcept {
        // не используется
    }

    template<typename U>
    constexpr MyAllocator(const MyAllocator<U, MEM_PAGE_SIZE>&) noexcept {}

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {        
        new(p) U(std::forward<Args>(args)...);        
    }

    template<typename U>
    void destroy(U* p) noexcept {        
        p->~U();
    }

    friend bool operator ==(const MyAllocator&, const MyAllocator&) { return true; }
    friend bool operator !=(const MyAllocator&, const MyAllocator&) { return false; }

    template <typename U>
    struct rebind {
        using other = MyAllocator<U, MEM_PAGE_SIZE>;
    };
};

}