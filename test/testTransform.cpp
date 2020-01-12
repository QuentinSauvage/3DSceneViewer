#include <iostream>
#include <math.h>
#include "transform.hpp"
#include "quaternion.hpp"
#include "direction.hpp"
#include "point.hpp"

using namespace libgeometry;

#define EPSYLON 0.001

void testToQuat() {
    std::cout << "Test ToQuat..." << std::endl;
    Quaternion<float> q(30,Direction<float,4>{2,1,4});
    Transform<float> t(30,Direction<float,4>{2,1,4});
    assert(q==t.to_quat());
}

void testConcat() {
    std::cout << "Test Concat..." << std::endl;
    Point<float,4> p{10,10,10};
    Vector<float,3> translation{10,0,0},scale{5,2,3};
    Direction<float,4> rotation{0,0,1};
    Transform<float> t1(translation,false);
    Transform<float> t2(scale,true);
    Transform<float> t3(90,rotation);
    std::cout << (t1.concat(t2).concat(t3)).apply(p) << std::endl;
}

void testApplyPoint() {
    std::cout << "Test ApplyPoint..." << std::endl;
    Point<float,4> p1{10,10,10},expectedt{20,10,10},expecteds{50,20,30},expectedts{60,20,30};
    Vector<float,3> translation{10,0,0},scale{5,2,3};
    Direction<float,4> rotation{0,0,1};
    Transform<float> t1(translation,false);
    Point<float,4> res1=t1.apply(p1);
    assert(res1==expectedt);
    Transform<float> t2(scale,true);
    Point<float,4> res2=t2.apply(p1);
    assert(res2==expecteds);
    Transform<float> t3=t2.concat(t1);
    Point<float,4> res3=t3.apply(p1);
    assert(res3==expectedts);
    Transform<float> t4(90,rotation);
    Point<float,4> res4=t4.apply(res3);
    std::cout << res4 << std::endl;
}

void testApplyDirection() {
    std::cout << "Test ApplyDirection..." << std::endl;
    Direction<float,4> d1{10,10,10},expectedt{10,10,10},expecteds{50,20,30},expectedts{50,20,30};
    Vector<float,3> translation{10,0,0},scale{5,2,3};
    Direction<float,4> rotation{0,0,1};
    Transform<float> t1(translation,false);
    Direction<float,4> res1=t1.apply(d1);
    assert(expectedt==res1);
    Transform<float> t2(scale,true);
    Direction<float,4> res2=t2.apply(d1);
    assert(res2==expecteds);
    Transform<float> t3=t1.concat(t2);
    Direction<float,4> res3=t3.apply(d1);
    assert(res3==expectedts);
    Transform<float> t4(90,rotation);
    Direction<float,4> res4=t4.apply(res3);
    std::cout << res4 << std::endl;
}

void testApplySphere() {
    std::cout << "Test ApplySphere..." << std::endl;
    Sphere<float,4> s1(Point<float,4>{10,20,8},4);
    Sphere<float,4> expectedt1(Point<float,4>{20,20,8},4);
    Sphere<float,4> expectedt2(Point<float,4>{20,40,16},8);
    Vector<float,3> translation{10,0,0},scale1{5,2,3},scale2{2,2,2};
    Direction<float,4> rotation{0,0,1};
    Transform<float> t1(translation,false);
    Sphere<float,4> res1=t1.apply(s1);
    assert(res1.getCenter()==expectedt1.getCenter()&&res1.getRadius()==expectedt1.getRadius());
    Transform<float> t2(scale1,true);
    Sphere<float,4> res2=t2.apply(s1);
    assert(res2.getCenter()==s1.getCenter()&&res2.getRadius()==s1.getRadius());
    Transform<float> t3(scale2,true);
    Sphere<float,4> res3=t3.apply(s1);
    assert(res3.getCenter()==expectedt2.getCenter()&&res3.getRadius()==expectedt2.getRadius());
    Transform<float> t4(90,rotation);
    Sphere<float,4> res4=t4.apply(res3);
    std::cout << res4 << std::endl;
}

int main() {
    testToQuat();
    testConcat();
    testApplyPoint();
    testApplyDirection();
    testApplySphere();
}

