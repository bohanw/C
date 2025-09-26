#include <iostream>
#include "UniqPtr.h"


template<typename T>
UniqPtr<T>::UniqPtr(UniqPtr&& other)noexcept : ptr(other.ptr){
    other.ptr = nullptr;
}

template<typename T>
UniqPtr<T>& UniqPtr<T>::operator=(UniqPtr&& other) noexcept{
    if (this != &other){
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
    }
    return *this;
};

struct Test{
    int x;
    Test(int val):x(val) {}
    void show() { std::cout << "Value: " << x << std::endl; }
};

int main(){
    UniqPtr<Test> testPtr(new Test(21));
    testPtr->show();
    UniqPtr<Test> testPtr2 = std::move(testPtr);
    if (testPtr.get() == nullptr){
        std::cout << "ptr1 is null after move" << std::endl;
    }
    testPtr2->show();
    return 0;
}
