#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <iostream>
#include <math.h>
#include "direction.hpp"
#include "libmatrix.h"

namespace libgeometry {
    using namespace libmatrix;

    template <typename T>    
    class Quaternion : public Vector<T,4> {

        public:
            Quaternion(std::initializer_list<T> l) : Vector<T,4>(l){}

            Quaternion(T angle,Direction<T,4> axis) {
                angle=((angle*M_PI)/180)/2;
                this->array[0]=axis.at(0)*sin(angle);
                this->array[1]=axis.at(1)*sin(angle);
                this->array[2]=axis.at(2)*sin(angle);
                this->array[3]=cos(angle);
            }

            // Returns the conjugate of the quaternion.
            inline Quaternion<T> conjugate() const { 
                Quaternion<T> c;
                c[0]=-this->at(0);
                c[1]=-this->at(1);
                c[2]=-this->at(2);
                c[3]=this->at(3);
                return c;
            }
            
            // Returns the imaginary part of the quaternion.
            inline Vector<T,3> im() const {
                Vector<T,3> i;
                i[0]=this->at(0);
                i[1]=this->at(1);
                i[2]=this->at(2);
                return i;
            }

            // Returns the inverse of the quaternion.
            inline Quaternion<T> inverse() const {
                T n=this->norm();
                return (1/(n*n))*conjugate();
            }

            // Returns the real part of the quaternion.
            inline float re() const { return this->at(3); }

            Quaternion<T> operator+(float s) { 
                Quaternion<T> q=*this;
                for(int i=0;i<4;++i)
                    q[i]+=s;
                return q; 
            }

            template<typename U>
            friend Quaternion<U> operator+(U s,Quaternion<U> &q);

            Quaternion<T> operator+(Quaternion<T> q1) {
                Quaternion<T> q2=*this;
                for(int i=0;i<4;++i)
                    q2[i]+=q1[i];
                return q2;
            }

            Quaternion<T> &operator+=(T s) {
                for(int i=0;i<4;++i)
                    this->array[i]+=s;
                return *this;
            }

            template<typename U>
            friend Quaternion<U> &operator+=(U s,Quaternion<U> &q);

            Quaternion<T> &operator+=(Quaternion<T> &q) {
                for(int i=0;i<4;++i)
                    this->array[i]+=q[i];
                return *this;
            }

            Quaternion<T> operator-(T s) {
                Quaternion<T> q=*this;
                for(int i=0;i<4;++i)
                    q[i]-=s;
                return q; 
            }

            template<typename U>
            friend Quaternion<U> operator-(U s,Quaternion<U> q);

            Quaternion<T> operator-(Quaternion<T> q1) {
                Quaternion<T> q2=*this;
                for(int i=0;i<4;++i)
                    q2[i]-=q1[i];
                return q2;
            }

            Quaternion<T> &operator-=(T s) {
                for(int i=0;i<4;++i)
                    this->array[i]-=s;
                return *this;
            }

            template<typename U>
            friend Quaternion<U> &operator -=(U s,Quaternion<U> &q);

            Quaternion<T> &operator-=(Quaternion<T> &q) {
                for(int i=0;i<4;++i)
                    this->array[i]-=q[i];
                return *this;
            }

            Quaternion<T> operator*(T s) {
                Quaternion<T> q=*this;
                for(int i=0;i<4;++i)
                    q[i]*=s;
                return q;
            }

            template<typename U>
            friend Quaternion<U> operator*(U s,Quaternion<U> q);

            Quaternion<T> operator*(Quaternion<T> q2) {
                Quaternion q1=*this;
                Vector<float,3> v1=q1.im(),v2=q2.im();
                v1=q1.re()*v2+q2.re()*v1+v1.cross(v2);
                T s=q1.re()*q2.re()-v1.dot(v2);
                return Quaternion<float>{v1[0],v1[1],v1[2],s};
            }

            Quaternion<T> &operator*=(float s) {
                for(int i=0;i<4;++i)
                    this->array[i]*=s;
                return *this;
            }

            template<typename U>
            friend Quaternion<U> &operator *=(U s,Quaternion<U> &q);

            Quaternion<T> &operator*=(Quaternion<T> q) {
                *this=(*this)*q;
                return *this;
            }

            Quaternion(){}
    };

    template <typename T>
    Quaternion<T> operator +(T s,Quaternion<T> q){
        return q+s;
    }

    template <typename T>
    Quaternion<T> &operator +=(T s,Quaternion<T> &q){
        q+=s;
        return q;
    }

    template <typename T>
    Quaternion<T> operator -(T s,Quaternion<T> q){
         return q-s;
    }

    template <typename T>
    Quaternion<T> &operator -=(T s,Quaternion<T> &q){
        q-=s;
        return q;
    }

    template <typename T>
    Quaternion<T> operator *(T s,Quaternion<T> q){
        return q*s;
    }

    template <typename T>
    Quaternion<T> &operator *=(T s,Quaternion<T> &q){
        q*=s;
        return q;
    }
}

#endif