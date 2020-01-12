#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <iostream>
#include "point.hpp"

using namespace std;

namespace libgeometry {
    template<typename T,int N>
    class Point;
    
    template<typename T,int N>
    class Sphere {
        private:
            Point<T,N> center;
            float radius;
        public:
            Sphere(){}
            Sphere(const Point<T,N> &_p, float _r) : center(_p),radius(_r){}

            inline Point<T,N> getCenter() const { return center; }
            inline void setCenter(Point<T,N> p) { center=p; }
            inline float getRadius() const { return radius; }
            inline void setRadius(float f) { radius=f; }

            // Returns true if the sphere is behind a plane given as argument, and false otherwise.
            bool behind(const Plane<T,N> &p) const {
                if(this->is_null()) return true;
                float dist_center = center.at(0)*p.at(0)+center.at(1)*p.at(1)+center.at(2)*p.at(2)+p.at(3);
                if(dist_center<0)
                    return dist_center + radius < 0;
                return false;
            }

            bool is_null() const { return center.is_null(); }

            template<typename U,int M>
            friend std::ostream &operator <<(std::ostream &out, Sphere<U,M> s);

            ~Sphere(){}
    };

    template<typename T,int N>
    std::ostream &operator <<(std::ostream &out, Sphere<T,N> s) {
        out << '(' << s.center << ',' << s.radius << ')';
        return out;
    }
}

#endif