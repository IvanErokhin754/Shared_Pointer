#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cstddef>
#include <stdexcept>

enum class PointerType{
    Single,
    Array
};

template<typename T>
void DeleteObject(T *ptr) {
    delete ptr;
}

template<typename T>
void DeleteArray(T *ptr) {
    delete[] ptr;
}

template<typename T>
struct ControlBlock{
    size_t counter;
    void (*deleter)(T*);
    size_t size;
    PointerType type;
};

template<typename T>
class SharedPtr{
private:
    T *ptr;
    ControlBlock<T> *control_block;
    void Release() {
        if (control_block) {
            control_block->counter--;

            if (control_block->counter == 0) {
                control_block->deleter(ptr);
                delete control_block;
            }
        }
    }

public:
    SharedPtr() : ptr(nullptr), control_block(nullptr) {}
    SharedPtr(T *raw) : ptr(raw), control_block(new ControlBlock<T>()) {
        control_block->counter = 1;
        control_block->size = 1;
        control_block->type = PointerType::Single;
        control_block->deleter = DeleteObject;
    } 
    SharedPtr(T *raw, size_t n) : ptr(raw), control_block(new ControlBlock<T>()) {
        control_block->counter = 1;
        control_block->size = n;
        control_block->type = PointerType::Array;
        control_block->deleter = DeleteArray;
    }
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), control_block(other.control_block) {
        if (other.control_block) {
            control_block->counter++;
        }
    }
    SharedPtr(SharedPtr&& other) : ptr(other.ptr), control_block(other.control_block) {
        other.ptr = nullptr;
        other.control_block = nullptr;
    }

    T* Get() const {
        return ptr;
    }

    T* operator->() {
        if (!control_block)
            throw std::logic_error("Operator-> : control block is empty");
        if (control_block->type == PointerType::Array)
            throw std::logic_error("Operator-> : can not be used to array");
    
        return ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

    T& operator*() {
        if (ptr == nullptr)
            throw std::logic_error("Operator* : ptr is null");
        if (control_block->type == PointerType::Array)
            throw std::logic_error("Operator* : can not be used to array");
        
        return *ptr;
    }

    size_t UseCount() const {
        if (!control_block)
            return 0;
        
        return control_block->counter;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (&other == this)
            return *this;
        
        Release();

        ptr = other.ptr;
        control_block = other.control_block;

        if (control_block) {
            control_block->counter++;
        }

        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (&other == this) 
            return *this;
    
        Release();

        ptr = other.ptr;
        control_block = other.control_block;

        other.ptr = nullptr;
        other.control_block = nullptr;
        
        return *this;
    }

    T& operator[](size_t index) {
        if (!control_block)
            throw std::logic_error("Operator[] : Control block is empty");
        if (control_block->type == PointerType::Single)
            throw std::logic_error("Operator[] : can not be used to single pointer");
        
        if (index >= control_block->size)
            throw std::out_of_range("Operator[] : index out of range");
        
        return ptr[index];
    }


    ~SharedPtr() {
        Release();
    }

};

#endif /* SHARED_PTR_HPP */
