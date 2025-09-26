#include <cstddef>

template <typename T>

class UniqPtr {
  T* ptr;
  
public:
    // Constructor
    explicit UniqPtr(T* p = nullptr) : ptr(p) {}  
    ~UniqPtr() {delete ptr;}

    UniqPtr(const UniqPtr&) = delete; // Disable copy constructor
    UniqPtr& operator=(const UniqPtr&) = delete; // Disable copy assignment

    //Movable
    UniqPtr(UniqPtr&& other) noexcept ;
    UniqPtr& operator=(UniqPtr&& other) noexcept ;
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};