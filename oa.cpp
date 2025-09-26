#include <iostream>

using namespace std;
struct Test{
    int* x{};
    int* y{};

    Test() {
        x = new int(5);
        y = new int(6);
    }
    ~Test (){
        delete x;
        delete y;
    }

};
// class Base {
// public:
//     Base() {std::cout << "Cons A\n";}
//      ~Base() {std::cout << "Des A\n";}
// };

// class Derived : public Base {
// public:
//     Derived() {std::cout << "Cons B\n";}
//     ~Derived() {std::cout << "Des B\n";}
// };

// int main() {
//     //Derived d;
//     Base* p = new Derived;
//     delete p ;//= new Derived;  // Polymorphic behavior


// }
int main (){
    return 0;
}