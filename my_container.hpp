#pragma once

#include <memory>

namespace my {

using size_type = std::size_t;  

/**
    @brief Класс пользовательского итератора для последовательного контейнера.
 */
template<typename T>
class MyContainerIterator {    
public:
    MyContainerIterator() : ptr(nullptr) {}
    MyContainerIterator(T* ptr) : ptr(ptr) {}

    T& operator *() { return *ptr; }    
    T* operator ->() const { return ptr; }
    MyContainerIterator& operator ++() { ++ptr; return *this; }
    bool operator ==(const MyContainerIterator& other) const { return ptr == other.ptr; }    
    bool operator !=(const MyContainerIterator& other) const { return ptr != other.ptr; }    

private:
    T* ptr;
};


/**
    @brief Класс пользовательского контейнера.
      
    Эмулирует работу std::vector.
 */
template <typename T, typename Alloc = std::allocator<T>>
class MyContainer {
public:    
    using allocator_traits = std::allocator_traits<Alloc>;
    using pointer = typename allocator_traits::pointer;
    using size_type = typename allocator_traits::size_type;
    using iterator = MyContainerIterator<T>;
    using const_iterator = MyContainerIterator<const T>;

public:
    MyContainer() : alloc(), data(nullptr), size_(0), capacity_(0) {}

    ~MyContainer() {
        clear();
        allocator_traits::deallocate(alloc, data, capacity_);
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize(capacity_ == 0 ? 2 : capacity_ * 2);
        }
        allocator_traits::construct(alloc, data + size_, value);
        ++size_;
    }    
    
    void pop_back() {
        if (size_ > 0) {            
            allocator_traits::destroy(alloc, data + size_ - 1);        
            --size_;
        }        
    }

    void clear() {
        for (size_type i = 0; i < size_; ++i) {
            allocator_traits::destroy(alloc, data + i);
        }
        size_ = 0;
    }

    size_type size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    T& operator[](size_type index) {
        return data[index];
    }

    const T& operator[](size_type index) const {
        return data[index];
    }

    // итераторы
    iterator begin() { return iterator(data); }    
    iterator end() { return iterator(data + size_); }
    const_iterator begin() const { return const_iterator(data); }    
    const_iterator end() const { return const_iterator(data + size_); }    

private:
    Alloc alloc;
    pointer data;
    size_type size_;
    size_type capacity_;

    void resize(size_type new_capacity) {
        pointer new_data = allocator_traits::allocate(alloc, new_capacity);
        for (size_type i = 0; i < size_; ++i) {
            allocator_traits::construct(alloc, new_data + i, std::move(data[i]));
            allocator_traits::destroy(alloc, data + i);
        }
        allocator_traits::deallocate(alloc, data, capacity_);
        data = new_data;
        capacity_ = new_capacity;
    }
};

}