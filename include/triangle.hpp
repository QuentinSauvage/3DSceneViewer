#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <iostream>
#include <math.h>
#include "point.hpp"
#include "direction.hpp"

using namespace std;

namespace libgeometry {
    
    template<typename T,int N>
    class Triangle {
        private:
            Point<T,N> p0,p1,p2;
            T a;

        public:
            Triangle(){}
            Triangle(const Point<T,N> &_p0, const Point<T,N> &_p1, const Point<T,N> &_p2) : p0(_p0), p1(_p1), p2(_p2) {
                a=0;
            }

            Direction<T,4> normale() {
                Vector<T,4> v1=p1-p0,v2=p2-p0;
                Vector<T,3> tmp=v1.cross(v2);
                return Direction<T,4>(tmp.to_unit());
            }

            // Returns the area of the triangle.
            T area() { 
                if(a==0) {
                    Vector<T,N> v1 = p1-p0;
                    T n = p2.norm();
                    n*=n;
                    Vector<T,N> v2 = (v1.dot(p2)/n)*p2;
                    v1*=v1;
                    v2*=v2;
                    float h = sqrt(v1.norm()-v2.norm());
                    float b = p0.length_to(p2).norm();
                    a=(b*h)/2;
                }
                return a;
            }

            // Returns true if the triangle contains an invalid value, and false otherwise.
            bool is_null() { return p0.is_null()||p1.is_null()||p2.is_null(); }

            // Returns a point corresponding to triangle’s vertex 0.
            inline Point<T,N> get_p0() const { return p0; }

            // Returns a point corresponding to triangle’s vertex 1.
            inline Point<T,N> get_p1() const { return p1; }

            // Returns a point corresponding to triangle’s vertex 2.
            inline Point<T,N> get_p2() const { return p2; }

            template<typename U,int M>
            friend std::ostream &operator <<(std::ostream &out, Triangle<U,M> t);

            ~Triangle(){}
    };

    template<typename T,int N>
    std::ostream &operator <<(std::ostream &out, Triangle<T,N> t) {
        return out << '(' << t.get_p0() << ',' << t.get_p1() << ',' << t.get_p2() << ')';
    }
}

#endif