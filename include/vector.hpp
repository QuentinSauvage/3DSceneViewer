#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "matrix.hpp"
#include <iostream>
#include <stdexcept>
#include <math.h>
#include<bits/stdc++.h> 

namespace libmatrix {
    
    template<typename T,int N,int M>
    class Matrix;
    
    template <typename T,int N>
    class Vector {
        protected:
            T array[N];

        public:
            Vector() {
                if (std::is_same<T, int>::value)
                    for(int i=0;i<N;++i) array[i]=INT_MAX;
                else
                    for(int i=0;i<N;++i) array[i]=NAN;
            }

            Vector(std::initializer_list<T> l) {
                if(l.size()==1)
                    for(int i=0;i<N;++i) array[i]=l.begin()[0];
                else
                    for(size_t i=0;i<l.size();++i)
                        array[i]=l.begin()[i];
            }

            // Addresses the i-th element of the vector.
            // Raises an exception if i is out of range.
            virtual T at(int i) const {
                T t;
                if(i<N) t = array[i];
                else throw std::out_of_range("Vector::at : i out of range");
                return t;
            }

            // Cross product with another vector.
            // Uses only the first 3 coordinates.
            // Raises an exception if the vector has less than 3 elements.
            template<int M>
            Vector<T,3> cross(const Vector<T,M> &v) const {
                Vector<T,3> res;
                if(is_null()||v.is_null()) return res;
                if(N<3||M<3) throw std::out_of_range("Vector::cross : out of range");
                res[0]=(at(1)*v.at(2))-(at(2)*v.at(1));
                res[1]=(at(2)*v.at(0))-(at(0)*v.at(2));
                res[2]=(at(0)*v.at(1))-(at(1)*v.at(0));
                return res;
            }

            // Dot product with another vector.
            T dot(const Vector<T,N> &v) const {
                T res=0;
                if(is_null()||v.is_null()) return res;
                for(int i=0;i<N;++i)
                    res+=array[i]*v.at(i);
                return res;
            }

            //  Returns true if the vector is orthogonal to another given as an argument, false otherwise.
            bool is_ortho(const Vector<T,N> &v) const {
                Vector<T,N> unit_v1=to_unit(),unit_v2=v.to_unit();
                if(unit_v1.is_null()||unit_v2.is_null()) return false;
                for(int i=0;i<N;++i)
                    if(unit_v1[i]==unit_v2[i])
                        return false;
                return dot(v)==0;
            }

            // Returns true if the vector contains an invalid value, false otherwise.
            // Notably, if the vector contains nan as values.
            bool is_null() const {
                if (std::is_same<T, int>::value) {
                    for(int i=0;i<N;++i) 
                        if(array[i]==INT_MAX)
                            return true;
                } else {
                    for(int i=0;i<N;++i) 
                        if(isnan(array[i]))
                            return true;
                }
                return false;
            }

            // Returns true if the vector is unit, false otherwise.
            bool is_unit() const {
                return norm()==1;
            }

            // Returns the norm of the vector.
            T norm() const {
                T res=0;
                if(is_null()) return res;
                for(int i=0;i<N;++i)
                    res+=array[i]*array[i];
                return sqrt(res);
            }

            // Returns a copy of the vector normalised.
            Vector<T,N> to_unit() const {
                Vector<T,N> res=*this;
                T n=norm();
                if(is_null()||n==0) return res;
                T left = (1/n);
                res *= left;
                return res;
            }

            template<typename U, int M>
            friend std::ostream &operator <<(std::ostream &out, Vector<U,M> v);
            
            inline T &operator[](const int i) { return array[i]; }

            Vector<T,N> operator+(Vector<T,N> v) {
                Vector<T,N> res;
                for(int i=0;i<N;++i)
                    res[i]=array[i]+v[i];
                return res;
            }

            Vector<T,N> operator+(float f) const {
                Vector<T,N> res;
                for(int i=0;i<N;++i)
                    res[i]=array[i]+f;
                return res;
            }
            
            Vector<T,N> &operator+=(const Vector<T,N> &v) {
                for(int i=0;i<N;++i)
                    array[i]+=v.at(i);
                return *this;
            }
            
            Vector<T,N> operator-(Vector<T,N> v) const {
                Vector<T,N> res;
                for(int i=0;i<N;++i)
                    res[i]=array[i]-v[i];
                return res;
            }

            Vector<T,N> operator-() const {
                return *(this)*-1;
            }
            
            Vector<T,N> &operator-=(Vector<T,N> &v) {
                for(int i=0;i<N;++i)
                    array[i]-=v[i];
                return *this;
            }
            
            Vector<T,N> operator*(T s) const {
                Vector<T,N> res;
                for(int i=0;i<N;++i)
                    res[i]=array[i]*s;
                return res;
            }
            
            template<typename U, int M>
            friend Vector<U,M> operator *(U s,const Vector<U,M> v);

            Vector<T,N> operator *(const Vector<T,N> v) const {
                Vector<T,N> res;
                for(int i=0;i<N;++i)
                    res[i]=at(i)*v.at(i);
                return res;
            }

            template<int M>
            Vector<T,N> operator *(Matrix<T,N,M> m) {
                Vector<T,N> res;
                for(int i=0;i<N;++i)
                    res[i]=dot(m[i]);
                return res;
            }
            
            Vector<T,N> &operator *=(T s) {
                for(int i=0;i<N;++i)
                    array[i]*=s;
                return *this;
            }

            template<typename U, int M>
            friend Vector<U,M> &operator *=(U s,Vector<U,M> &v);

            Vector<T,N> &operator *=(Vector<T,N> v) {
                for(int i=0;i<N;++i)
                    array[i]=array[i]*v[i];
                return *this;
            }
            
            template<int M>
            Vector<T,N> &operator *=(Matrix<T,N,M> &m) {
                Vector<T,N> res=(*this)*m;
                *this=res;
                return *this;
            }
            
            bool operator==(const Vector<T,N> &v) const {
                for(int i=0;i<N;++i) {
                    if(v.at(i)!=this->at(i)) {
                        return false;
                    }
                }
                return true;
            }

            bool operator!=(const Vector<T,N> &v) const {
                return !((*this)==v);
            }
            
            ~Vector() {}
    };

    template <typename T, int N>
    Vector<T,N> operator *(T s,const Vector<T,N> v){
        return v*s;
    }

    template <typename T, int N>
    Vector<T,N> &operator *=(T s,Vector<T,N> &v){
        v=v*s;
        return v;
    }

    template <typename T, int N>
    std::ostream &operator <<(std::ostream &out, Vector<T,N> v) {
        out << '<';
        for(int i=0;i<N;++i) {
            if(i)
                out << ',';
            out << v[i];
        }
        out << '>';
        return out;
    }

    template <typename T, int N>
    T dot(Vector<T,N> &v1, Vector<T,N> &v2) {
        return v1.dot(v2);
    }

    template <typename T, int N>
    Vector<T,3> cross(Vector<T,N> &v1, Vector<T,N> &v2) {
        return v1.cross(v2);
    }

    template <typename T, int N>
    void toZero(Vector<T,N> &v) {
        for(int i=0;i<N;++i)
            v[i]=0;
    }

    using Vec2i=Vector<int,2>;
    using Vec3i=Vector<int,3>;
    using Vec4i=Vector<int,4>;
    using Vec2r=Vector<float,2>;
    using Vec3r=Vector<float,3>;
    using Vec4r=Vector<float,4>;
}

#endif