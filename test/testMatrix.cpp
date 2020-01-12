#include "matrix.hpp"
#include "vector.hpp"
#include <iostream>
#include <assert.h>

using namespace libmatrix;

#define SIZE 10

void testAt() {
    std::cout << "Test At..." << std::endl;
    Matrix<float,2,2> m{1,2,3,4};
    assert(m.at(0,0)==1);
    assert(m.at(0,1)==2);
    assert(m.at(1,0)==3);
    assert(m.at(1,1)==4);
    bool e=false;
    try {
        m.at(2,0);
    } catch (const std::out_of_range &oor) {
        e=true;
    }
    try {
        m.at(0,2);
    } catch (const std::out_of_range &oor) {
        e=true;
    }
    assert(e);
}

void testInverse() {
    std::cout << "Test Inverse..." << std::endl;
    Matrix<float,3,3> m{2,1,1,3,2,1,2,1,2},inv=m.inverse();
    std::cout << inv << std::endl;
}

void testIsNull() {
    std::cout << "Test IsNull..." << std::endl;
    Matrix<float,2,2> m1{1,2,3,4},m2;
    assert(!m1.is_null());
    assert(m2.is_null());
} 

void testIsOrtho() {
    std::cout << "Test IsOrtho..." << std::endl;
    Matrix<float,3,3> m1{0,1,0,0,0,1,1,0,0},m2{1,2,3,4,5,6,7,8,9};
    assert(m1.is_ortho());
    assert(!m2.is_ortho());
}

void testTranspose() {
    std::cout << "Test Transpose..." << std::endl;
    Matrix<float,3,4> m{1,2,3,4,5,6,7,8,9,10,11,12};
    for(int i=0;i<3;++i)
        for(int j=0;j<4;++j) {
            m[i][j]=(i*3)+j;
            assert(m[i][j]==(i*3)+j);
            assert(m.at(i,j)==(i*3)+j);
        }
    Matrix<float,4,3> t = m.transpose();
    std::cout << t << std::endl;
}

void testOperators() {
    std::cout << "Test Operators..." << std::endl;
    Matrix<float,2,3> m1{1,2,3,4,5,6};
    Matrix<float,3,2> m2{7,8,9,10,11,12};
    Matrix<float,2,2> m3 = m1*m2;
    Matrix<float,2,2> m4 = m1*m2;
    assert(m3==m4);
    Matrix<float,2,3> m5=m1;
    std::cout << m3 << std::endl;
    Matrix<float,2,2> mx{1,2,3,4};
    Matrix<float,2,2> mx2;
    mx2=mx*2;
    assert(mx2[0][0]==2);
    assert(mx2[0][1]==4);
    assert(mx2[1][0]==6);
    assert(mx2[1][1]==8);
    mx*=2;
    assert(mx==mx2);
}

int main() {
    testAt();
    testInverse();
    testIsNull();
    testIsOrtho();
    testTranspose();
    testOperators();
	return 0;
}