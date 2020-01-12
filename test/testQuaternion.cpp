#include <iostream>
#include <math.h>
#include "quaternion.hpp"
#include "direction.hpp"

using namespace libgeometry;

#define EPSYLON 0.001

void testConjugate() {
    std::cout << "Test Conjugate..." << std::endl;
    Quaternion<float> q(30,Direction<float,4>{2,1,4});
    Quaternion<float> c=q.conjugate();
    assert(c.im()==-q.im());
    assert(c.re()==q.re());
}

void testIm() {
    std::cout << "Test Image..." << std::endl;
    Quaternion<float> q1(30,Direction<float,4>{2,1,4});
    Quaternion<float> q2=q1;
    Vector<float,3> v=q1.im();
    assert(v[0]==q2[0]);
    assert(v[1]==q2[1]);
    assert(v[2]==q2[2]);
}

void testInverse() {
    std::cout << "Test Inverse..." << std::endl;
    Quaternion<float> q(30,Direction<float,4>{2,1,4});
    assert(q.inverse()==(q.conjugate()*(1/(q.norm()*q.norm()))));
}

void testRe() {
    std::cout << "Test Real..." << std::endl;
    Quaternion<float> q(30,Direction<float,4>{2,1,4});
    float angle=((30*M_PI)/180)/2;
    assert(abs(q.re()-cos(angle))<EPSYLON);
}

void testOperators() {
    std::cout << "Test Operators..." << std::endl;
    Quaternion<float> q1{1,2,3,4},q2{1,2,3,4};
    assert(q1==q2);
    Quaternion<float> q3=q1+q2;
    assert(q1!=q3);
    Quaternion<float> q1q2{2,4,6,8};
    q1+=q2;
    assert(q3==q1q2);
    assert(q1==q1q2);
    q3=q1q2-q2;
    assert(q3==q2);
    q1-=q2;
    assert(q1==q2);
    Quaternion<float> q1x{1,2,3,2},q2x{1,2,3,3};
    q3=q1x*q2x;
    q1x*=q2x;
    std::cout << q3 << std::endl;
    assert(q1x==q3);
    q1=q2;
    Quaternion<float> q2xx{2,4,6,8};
    q3=q2*2;
    assert(q3==q2xx);
    q2*=2;
    assert(q2==q2xx);
    q3=q1+2;
    q1+=2;
    assert(q1==q3);
}

int main() {
    testConjugate();
    testIm();
    testInverse();
    testRe();
    testOperators();
}