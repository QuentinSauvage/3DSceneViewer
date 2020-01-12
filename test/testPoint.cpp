#include <iostream>
#include <math.h>
#include "libmatrix.h"
#include "libgeometry.h"

using namespace libgeometry;

void testBehind() {
    std::cout << "Test Behind..." << std::endl;
    Point<float,4> p1{10,-5,4},p2{20,-5,4},p3{-3,-8,3};
    Plane<float,4> pl(Vector<float,4>{1,2,3,4});
    assert(!p1.behind(pl));
    assert(!p2.behind(pl));
    assert(p3.behind(pl));
}

void testLengthTo() {
    std::cout << "Test LengthTo..." << std::endl;
    Point<float,4> p1{10,15,3}, p2{2,8,16};
    Direction<float,4> expected1{-8,-7,13},expected2{8,7,-13},expected3{0,0,0};
    Direction<float,4> res1=p1.length_to(p2);
    assert(res1==expected1);
    Direction<float,4> res2=p2.length_to(p1);
    assert(res2==expected2);
    Direction<float,4> res3=p1.length_to(p1);
    assert(res3==expected3);
}

void testOutside() {
    std::cout << "Test Outside..." << std::endl;
    Point<float,4> p1{5,5,5}, p2{10,5,5}, p3{10,10,10},p4{6,6,6};
    Sphere<float,4> s(p1,5);
    assert(!p1.outside(s));
    assert(!p2.outside(s));
    assert(p3.outside(s));
    assert(!p4.outside(s));
}

void testRotate() {
    std::cout << "Test Rotate..." << std::endl;
    Quaternion<float> q1{3,0,-1,0},q2{0,0,1,2};
    Quaternion<float> q(90,Direction<float,4>{0,1,0});
    Point<float,4> p{1,0,0};
    Point<float,4> res=p.rotate(q);
    std::cout << res << std::endl;
}

void testInter() {
    std::cout << "Test Inter..." << std::endl;
    Point<float,4> p1{1,2,-3},p2{-1,2,0},p3{2,2,2},p4{0,4,2};
    LineSegment<float,4> ls1(p1,p2),ls2(p3,p4);
    Plane<float,4> pl1(Vector<float,4>{2,-1,3,-2}),pl2(Vector<float,4>{2,2,2,2});
    std::cout << ls1.inter(pl1) << std::endl;
    assert(ls1.inter_coef(pl2)>0);
    assert(ls2.inter_coef(pl2)==0);
}

void testSphereBehind() {
    std::cout << "Test SphereBehind..." << std::endl;
    Point<float,4> p{5,5,5};
    Sphere<float,4> s(p,5);
    Plane<float,4> pl(Vector<float,4>{1,2,3,4});
    std::cout << s.behind(pl) << std::endl;
}

void testTriangleArea() {
    std::cout << "Test TriangleArea..." << std::endl;
    Triangle<float,4> t{Point<float,4>{1,0,0},Point<float,4>{0,1,0},Point<float,4>{0,0,1}};
    std::cout << t.area() << std::endl;
}

int main() {
    testBehind();
    testLengthTo();
    testOutside();
    testRotate();
    testInter();
    testSphereBehind();
    testTriangleArea();
}