#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"



using namespace std;
class BaseClass {
public:
    virtual void disp() {
        cout << "Function of Parent Class";
    }
};

class DerivedClass : public BaseClass {
public:
    void disp() {
        cout << "Function of Child Class";
    }
};

class DerivedClass2 : public BaseClass {
public:
    void disp() {
        cout << "Function of Child Class 2";
    }
};

class TestClass {
public:
    TestClass();

    TestClass(BaseClass * cl_src)
    {
        this->cl = cl_src;
    }
    BaseClass * cl;
};

int main() {
    DerivedClass2 derived;
    TestClass obj(&derived);
    obj.cl->disp();
    return 0;
}