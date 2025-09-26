#include <iostream>
using namespace std;

class A {
public:
    A()  { cout << "Constructor A\n"; }   // Runs whenever an A is created
    ~A() { cout << "Destructor A\n";  }   // Runs when an A is destroyed
};

class B : public A {
public:
    B()  { cout << "Constructor B\n"; }   // Runs after A() since B derives from A
    ~B() { cout << "Destructor B\n";  }   // Runs before ~A() when a B is destroyed
};

int main() {
    B b;             // First construct base A, then construct B
    A* p = new B;    // Allocates a B:
                     //   1. Runs A::A()
                     //   2. Then runs B::B()

    delete p;        // PROBLEM: A’s destructor is NOT virtual.
                     // Behavior is undefined: most compilers only call ~A().
                     //   → That means ~B() is skipped, and only ~A() runs here.

    // When main() ends:
    //   `b` (stack variable) is destroyed in reverse order:
    //   1. ~B()
    //   2. ~A()
}