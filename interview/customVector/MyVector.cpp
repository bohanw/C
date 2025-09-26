#include "MyVector.h"

template <typename T>
MyVector<T>::MyVector() : data(nullptr), sz(0), cap(0) {}

template <typename T>
MyVector<T>::~MyVector(){
    delete[] data;
}

template <typename T>
void MyVector<T>::push_back(const T& val){
    resize_if_needed();
    data[sz] = val;
    sz++;
}
template <typename T>
void MyVector<T>::pop_back(){
    if (sz == 0){
        throw std::out_of_range("Pop from empty vector");
    }
    sz--;
}

template <typename T>
T& MyVector<T>::operator[](size_t idx){
    if (idx >= sz){
        throw std::out_of_range("Index out of range");
    }
    return data[idx];
}



