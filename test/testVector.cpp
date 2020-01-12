#include "matrix.hpp"
#include "vector.hpp"
#include <iostream>
#include <assert.h>

using namespace libmatrix;

#define EPSYLON 0.01

void testAt() {
    std::cout << "Test At..." << std::endl;
    bool e=false;
    Vec2r v{1.5,2.0};
    assert(v.at(0)==1.5);
    assert(v.at(1)==2.0);
    try {
        v.at(2);
    } catch (const std::out_of_range &oor) {
        e=true;
    }
    assert(e);
}

void testCross() {
    std::cout << "Test cross..." << std::endl;
    bool e=false;
    Vec3r v1{2,3,4},v2{5,6,7},v3;
    Vec4r v4{1.5,2.0,0,5.0};
    Vec2r v5{1.5,2.0};
    //cross product with same size
    try {
        v1.cross(v2);
    } catch(const std::out_of_range &oor) {
        e=true;
    }
    assert(!e);

    //cross product null
    try {
        assert(v1.cross(v3).is_null());
    } catch(const std::out_of_range &oor) {}

    //cross product with different sizes
    e=false;
    try {
        v1.cross(v4);
    } catch(const std::out_of_range &oor) {
        e=true;
    }
    assert(!e);

    //cross product with size<3
    e=false;
    try {
        v1.cross(v5);
    } catch(const std::out_of_range &oor) {
        e=true;
    }
    assert(e);
}

void testDot() {
    std::cout << "Test Dot..." << std::endl;
    Vec3r v1{4,8,10},v2{9,2,7},v3;
    assert(v1.dot(v3)==0);
    assert(v1.dot(v2)==122);
}

void testIsOrtho() {
    std::cout << "Test IsOrtho..." << std::endl;
    Vec2r v1{1,0},v2{0,1},v3{1,0},v4;
    assert(v1.is_ortho(v2));
    assert(!v3.is_ortho(v1));
    assert(!v1.is_ortho(v4));
}

void testIsNull() {
    std::cout << "Test IsNull..." << std::endl;
    Vec2r v1r{1,0},v2r;
    Vec2i v1i{1,0},v2i;
    assert(!v1r.is_null());
    assert(v2r.is_null());
    assert(!v1i.is_null());
    assert(v2i.is_null());
}

void testIsUnit() {
    std::cout << "Test IsUnit..." << std::endl;
    Vec2r v1{3,2},v2;
    assert(!v2.is_unit());
    Vec2r v1u=v1.to_unit();
    assert(v1u.is_unit());
}

void testNorm() {
    std::cout << "Test Norm..." << std::endl;
    Vec2r v1{3,2},v2;
    assert(abs(3.61-v1.norm())<EPSYLON);
    assert(v2.norm()==0);
}

void testToUnit() {
    std::cout << "Test ToUnit..." << std::endl;
    Vec2r v1{3,2},v2;
    Vec2r v1u=v1.to_unit();
    assert(0.83-v1u[0]<EPSYLON);
    assert(0.55-v1u[1]<EPSYLON);
    assert(v2.to_unit().is_null());
}

void testOperators() {
    std::cout << "Test Operators..." << std::endl;
    Vec2r v1{3,2},v2{1,2};
    Vec2r moins=v1-v2;
    assert(moins[0]==2);
    assert(moins[1]==0);
    Vec2r inverse = -v1;
    assert(inverse[0]==-3);
    assert(inverse[1]==-2);
    
    Vec2r mul1 = v2 * -2.f;
    assert(mul1[0]==-2);
    assert(mul1[1]==-4);
    Vec2r mul2 = -2.f * v2;
    assert(mul2[0]==-2);
    assert(mul2[1]==-4);
    mul2 *= -1.f;
    assert(mul2[0]==2);
    assert(mul2[1]==4);

    Mat44r m{1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
    Vec4r v1_4{1,2,3,4};
    Vec4r vm=v1_4*m,v2_4=v1_4;
    v2_4*=m;
    for(int i=0;i<4;++i) {
        assert(vm[i]==v1_4.dot(m[i]));
        assert(v2_4[i]==v1_4.dot(m[i]));
    }

    Vec4r eq1{1,2,3,4},eq2{1,2,3,4},eq3{4,3,2,1};
    assert(eq1==eq2);
    assert(!(eq1==eq3));
    assert(eq1!=eq3);
    assert(!(eq1!=eq2));
}

int main() {
	testAt();
    testCross();
    testDot();
    testIsOrtho();
    testIsNull();
    testIsUnit();
    testNorm();
    testToUnit();
    testOperators();
	return 0;
}
