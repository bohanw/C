#include <iostream>
#include <stdexcept>


template<typename T>
class MyVector {
    T* data;
    size_t sz;
    size_t cap;

    void resize_if_needed()
    {
        if (sz == cap){
            if (cap == 0){
                cap = 1;
            }
            else {
                cap *= 2;
            }

            T* new_data = new T[cap];
            for( size_t i = 0 ; i < sz;i++){
                new_data[i] = data[i];
            }

            delete[] data;
            data = new_data;
        }
    }
public:
    MyVector();
    ~MyVector();

    void push_back(const T& val);
    void pop_back();
    size_t size() const { return sz; }
    size_t capacity() const { return cap; }

    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;

    T* begin() { return data; }
    T* end() { return data + sz; }
};
