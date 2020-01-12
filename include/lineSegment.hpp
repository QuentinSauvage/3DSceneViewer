#ifndef LINESEGMENT_HPP
#define LINESEGMENT_HPP

#include <iostream>
#include "point.hpp"
#include "plane.hpp"

using namespace std;

namespace libgeometry {
    
    template<typename T,int N>
    class LineSegment {
        private:
            Point<T,N> p1,p2;
            Direction<T,N> d;
            T length;
        public:
            LineSegment(){}
            LineSegment(const Point<T,N> &_p1, const Point<T,N> &_p2) : p1(_p1),p2(_p2){
                d=p1.length_to(p2);
                length=d.norm();
            }

            // Returns the starting point of the segment.
            inline Point<T,N> get_begin() const { return p1; }

            // Returns the end point of the segment.
            inline Point<T,N> get_end() const { return p2; }

            // Returns the coefficient of intersection between the segment and a plane given as an argument.
            T inter_coef(const Plane<T,N> &p) const {
                if(p.is_null()||d.is_null()) return -1;
                return d.dot(p);
            }

            // Returns the intersection point between the segment and the plane given as argument.
            Point<T,N> inter(const Plane<T,N> &p) const {
                if(is_null()) return Point<T,N>();
                T coef;
                if(p.dot(d)==0) {
                    coef = inter_coef(p);
                    if(coef!=0) return Point<T,N>();
                    // not important as the whole line is on the plane
                    return p1;
                }
                coef = -(p.dot(p1)/p.dot(d));
                Vector<T,N> resv = coef*d;
                Point<T,N> res(resv);
                res+=p1;

                // only return point if it is part of the segment
                if(p1.length_to(res).norm()<=length&&p2.length_to(res).norm()<=length)
                    return res;
                return Point<T,N>();
            }

            bool is_null() const { return p1.is_null()||p2.is_null(); }

            template<typename U, int M>
            friend std::ostream &operator <<(std::ostream &out, LineSegment<U,M> ls);
            
    };

    template <typename T, int N>
    std::ostream &operator <<(std::ostream &out, LineSegment<T,N> ls) {
        out << '(' << ls.p1 << ',' << ls.p2 << ')';
        return out;
    }
}

#endif