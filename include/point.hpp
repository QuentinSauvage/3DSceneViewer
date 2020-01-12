#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <math.h>
#include "libmatrix.h"
#include "plane.hpp"
#include "sphere.hpp"
#include "quaternion.hpp"
#include "direction.hpp"

using namespace std;

namespace libgeometry {
    using namespace libmatrix;

    template<typename T,int N>
    class Sphere;

    template <typename T, int N>    
    class Point : public Vector<T,N> {
        public:
            Point() {
                this->array[N-1]=1;
            }

            Point(const Vector<T,N> v) {
                for(int i=0;i<N;++i)
                    this->array[i]=v.at(i);
            }

            Point(const Vector<T,N-1> v) {
                for(int i=0;i<N-1;++i)
                    this->array[i]=v.at(i);
                this->array[N-1]=1;
            }

            Point(std::initializer_list<T> l) {
                for(size_t i=0;i<l.size();++i)
                    this->array[i]=l.begin()[i];
                this->array[N-1]=1;
            }

            // Returns true if the point is behind the plane given as argument, and false otherwise.
            bool behind(const Plane<T,N> p) const {
                if(this->is_null()) return true;
                return (this->at(0)*p.at(0)+this->at(1)*p.at(1)+this->at(2)*p.at(2)+p.at(3))<0;
            }

            // Returns a direction representing the vector between the point and another point given as argument.
            Direction<T,N> length_to(const Point &p) const {
                if(this->is_null()) return Direction<T,N>();
                Direction<T,N> d;
                for(int i=0;i<N;++i) {
                    d[i]=p.at(i)-this->at(i);
                }
                return d;
            }

            // Returns true if the point is inside a sphere given as argument.
            bool outside(const Sphere<T,N> &s) { 
                if(this->is_null()) return true;
                Direction<T,N> d=length_to(s.getCenter());
                return d.norm()>s.getRadius();
            }

            // Returns a new point corresponding to the rotation of the point by the quaternion given as argument.
            Point<T,N> rotate(Quaternion<T> q) const {
                if(this->is_null()||N!=4) return *this;
                Quaternion<float> vq{this->at(0),this->at(1),this->at(2),this->at(3)},tmp;
                tmp=q*vq;
                vq=tmp;
                tmp=tmp*q.inverse();
                Point<T,N> res(tmp);
                res[3]=tmp.at(3);
                return res;
            }

            ~Point() {}
    };
}

#endif