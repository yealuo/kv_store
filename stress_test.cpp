#include <bits/stdc++.h>

using namespace std;

class A {
public:
    A() {
        a();
    }

    virtual void a() = 0;
};

class B : public A {
public:
    B() = default;

    virtual void a() override { cout << 1; }
};

int main() {
    auto b = new B();
    return 0;
}