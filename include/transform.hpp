#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <iostream>
#include <math.h>
#include "quaternion.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "direction.hpp"
#include "sphere.hpp"

using namespace std;

namespace libgeometry {
    
    template<typename T>
    class Transform {
        private:
            Matrix<T,4,4> m;
            Quaternion<T> q;

        public:
            Transform() {}

            Transform(const Matrix<T,4,4> &_m) : m(_m) {}

            Transform(Quaternion<T> _q) {
                q=_q;
                m[0][0]=1-2*(_q[1]*_q[1])-2*(_q[2]*_q[2]);
                m[0][1]=2*(_q[0]*_q[1])-2*(_q[3]*_q[2]);
                m[0][2]=2*(_q[0]*_q[2])+2*(_q[3]*_q[1]);
                m[1][0]=2*(_q[0]*_q[1])+2*(_q[3]*_q[2]);
                m[1][1]=1-2*(_q[0]*_q[0])-2*(_q[2]*_q[2]);
                m[1][2]=2*(_q[1]*_q[2])-2*(_q[3]*_q[0]);
                m[2][0]=2*(_q[0]*_q[2])-2*(_q[3]*_q[1]);
                m[2][1]=2*(_q[1]*_q[2])+2*(_q[3]*_q[0]);
                m[2][2]=1-2*(_q[0]*_q[0])-2*(_q[1]*_q[1]);
                m[0][3]=0;
                m[1][3]=0;
                m[2][3]=0;
                m[3][0]=0;
                m[3][1]=0;
                m[3][2]=0;
                m[3][3]=1;
            }

            Transform(float angle,const Direction<T,4> &axis) {
                q=Quaternion<T>(angle,axis);
                *this=Transform<T>(q);
            }

            Transform(Vector<T,3> v, bool scale=false) {
                m=m.identity();
                if(scale) {
                    for(int i=0;i<3;++i)
                        m[i][i]=v[i];
                } else {
                    m[0][3]=v[0];
                    m[1][3]=v[1];
                    m[2][3]=v[2];
                }
            }

            // Returns the concatenation of two transforms.
            Transform<T> concat(const Transform<T> &tr) const {
                return Transform<float>(tr.getM()*m);
            }

            // Returns the quaternion corresponding to a rotation stored in the transform.
            inline Quaternion<T> to_quat() {
                if(q.is_null()) {
                    T w,x,y,z,s,trace;
                    trace=1+m.at(0,0)+m.at(1,1)+m.at(2,2);
                    if(trace>0) {
                        s=2*sqrt(trace);
                        x=(m.at(1,2)-m.at(2,1))/s;
                        y=(m.at(2,0)-m.at(0,2))/s;
                        z=(m.at(0,1)-m.at(1,0))/s;
                        w=0.25*s;
                    } else if(m.at(0,0)>m.at(1,1)&&m.at(0,0)>m.at(2,2)) {
                        s=sqrt(1+m.at(0,0)-m.at(1,1)-m.at(2,2))*2;
                        x=0.25*s;
                        y=(m.at(1,0)+m.at(0,1))/s;
                        z=(m.at(2,0)+m.at(0,2))/s;
                        w=(m.at(1,2)-m.at(2,1))/s;
                    } else if(m.at(1,1)>m.at(2,2)) {
                        s=sqrt(1+m.at(1,1)-m.at(0,0)-m.at(2,2))*2;
                        x=(m.at(0,1)+m.at(1,0))/s;
                        y=0.25*s;
                        z=(m.at(2,1)+m.at(1,2))/s;
                        w=(m.at(2,0)-m.at(0,2))/s;
                    } else {
                        s=sqrt(1+m.at(0,0)-m.at(1,1)-m.at(2,2))*2;
                        x=(m.at(2,0)+m.at(0,2))/s;
                        y=(m.at(2,1)+m.at(1,2))/s;
                        z=0.25*s;
                        w=(m.at(0,1)-m.at(1,0))/s;
                    }
                    q=Quaternion<T>{x,y,z,w};
                }
                return q;
            }

            // Returns a new point corresponding to the transform applied to the point given as argument.
            Point<T,4> apply(Point<T,4> p) const {
                return Point<T,4>(m*p);
            }

            // Returns a new direction corresponding to the transform applied to the direction given as argument.
            Direction<T,4> apply(Direction<T,4> &d) const {
                return Direction<T,4>(m*d);
            }

            // Returns a new sphere corresponding to the transform of the sphere given as argument.
            Sphere<T,4> apply(const Sphere<T,4> &s) const {
                Sphere<T,4> res = s;
                Point<T,4> p1,p2;
                p1=p2=s.getCenter();

                // should only use uniform-scale
                if(m.at(0,0)==m.at(1,1)&&m.at(0,0)==m.at(2,2)) {
                    T scaleValue=m.at(0,0);
                    res.setRadius(res.getRadius()*scaleValue);
                    for(int i=0;i<3;++i)
                        p1[i]=p2[i]*scaleValue+m.at(i,3);
                } else {
                    p1[0]+=m.at(0,3);
                    p1[1]+=m.at(1,3);
                    p1[2]+=m.at(2,3);
                }
                res.setCenter(p1);
                return res;
            }

            template<typename U>
            friend std::ostream &operator <<(std::ostream &out, Transform<U> t);

            // Returns the matrix corresponding to the transform.
            inline Matrix<T,4,4> getM() const { return m; }

            ~Transform() {}
    };

    template<typename T>
    std::ostream &operator <<(std::ostream &out, Transform<T> t) {
        out << t.m;
        return out;
    }
}

#endif