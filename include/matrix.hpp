#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include <stdexcept>
#include <iostream>

namespace libmatrix {
    template<typename T,int N>
    class Vector;

    template <typename T,int N,int M>
    class Matrix {
        private:
            Vector<T,M> array[N];
        
        public:
            Matrix() {}

            Matrix(std::initializer_list<T> l) {
                if(l.size()==1) {
                    for(int i=0;i<N;++i)
                        for(int j=0;j<M;++j)
                            array[j][i]=l.begin()[0];
                } else {
                    int j=0,k=0;
                    for(size_t i=0;i<l.size();++i,++k) {
                        if(k==M) {
                            k=0;
                            ++j;
                        }
                        array[j][k]=l.begin()[i];
                    }
                }
            }

            // Addresses element (i, j) of the matrix.
            // Raises an exception if i or j is out of range.
            T at(int i, int j) const {
                T t;
                try {
                    if(i<N) t = array[i].at(j);
                    else throw std::out_of_range("Vector::at : i out of range");
                } catch (const std::out_of_range& oor) {
                    throw std::out_of_range("Matrix::at : i or j out of range");
                }
                return t;
            }
            
            // Returns the identity matrix.
            Matrix<T,N,M> identity() const {
                Matrix<T,N,M> id;
                for(int i=0;i<N;++i)
                    for(int j=0;j<M;++j)
                        id[j][i]=(j==i)?1:0;
                return id;
            }

            // returns the inverse of the matrix. Returns a null matrix if the current matrix is not invertible.
            Matrix<T,N,M> inverse() const {
                if(N!=M) return Matrix<T,N,M>();
                Matrix<T,N,M> inv=*this;
                Matrix<T,N,M> id=identity();
                bool invertible;
                for(int i=0;i<N;++i) {
                    invertible=true;
                    for(int j=0;j<N;++j)
                        if(inv[i][j]!=0) invertible=false;
                    if(invertible) return Matrix<T,N,M>();
                }
                for(int j=0;j<N;++j) {
                    int r_ind=j;
                    for(int i=j;i<N;++i)
                        if(inv[i][j]>inv[r_ind][j]&&inv[i][j]!=0)
                            r_ind=i;
                    T v_max=inv[r_ind][j];

                    Vector<T,M> tmp=inv[j];
                    inv[j]=inv[r_ind];
                    inv[r_ind]=tmp;
                    tmp=id[j];
                    id[j]=id[r_ind];
                    id[r_ind]=tmp;

                    T div=1/v_max;
                    for(int i=0;i<N;++i) {
                        inv[j][i]*=div;
                        id[j][i]*=div;
                    }

                    for(int k=0;k<N;++k) {
                        T factor=-inv[k][j];
                        if(k!=j)
                            for(r_ind=0;r_ind<N;++r_ind) {
                                inv[k][r_ind]+=inv[j][r_ind]*factor;
                                id[k][r_ind]+=id[j][r_ind]*factor;
                            }
                    }
                }
                return id;
            }
            
            // Returns true if the matrix contains invalid values, false otherwise.
            bool is_null() const {
                if (std::is_same<T, int>::value) {
                    for(int i=0;i<N;++i)
                        if(array[i].is_null()) return true;
                } else {
                   for(int i=0;i<N;++i)
                        if(array[i].is_null()) return true;
                }
                return false;
            }
            
            // Returns true if the matrix orthogonal, false otherwise.
            bool is_ortho() const {
                Matrix<T,N,M> t = transpose(), i = inverse();
                return t==i;
            }
            
            // Returns the transpose of the matrix.
            Matrix<T,M,N> transpose() const {
                Matrix<T,M,N> res;
                int k=0,l=0;
                for(int i=0;i<N;++i) {
                    for(int j=0;j<M;++j) {
                        res[j][i]=at(i,j);
                        l=(l+1)%M;
                    }
                    k=(k+1)%N;
                }
                return res;
            }
            
            template<typename U, int O, int P>
            friend std::ostream &operator<<(std::ostream &out, Matrix<U,O,P> m);
            
            inline Vector<T,M> &operator[](const int i) { return array[i]; }
            
            Matrix<T,N,M> operator +(const Matrix<T,N,M> m) const {
                Matrix<T,N,M> res;
                for(int i=0;i<N;++i)
                    res[i]=array[i]+m[i];
                return res;
            }
            
            Matrix<T,N,M> &operator +=(Matrix<T,N,M> &m) {
                for(int i=0;i<N;++i)
                    array[i]+=m[i];
                return *this;
            }
            
            Matrix<T,N,M> operator -(const Matrix<T,N,M> m) const {
                Matrix<T,N,M> res;
                for(int i=0;i<N;++i)
                    res[i]=array[i]-m[i];
                return res;
            }
            
            Matrix<T,N,M> &operator -=(Matrix<T,N,M> &m) {
                for(int i=0;i<N;++i)
                    array[i]-=m[i];
                return *this;
            }
            
            Matrix<T,N,M> operator *(float s) {
                Matrix<T,N,M> res;
                for(int i=0;i<N;++i)
                    for(int j=0;j<M;++j)
                        res[i][j]=at(i,j)*s;
                return res;
            }
            
            template<typename U,int V,int W>
            friend Matrix<U,V,W> operator *(Matrix<U,V,W> m,float s);
            
            Vector<T,N> operator *(const Vector<T,N> v) const {
                Vector<T,N> res;
                for(int i=0;i<N;++i)
                    res[i]=v.dot(array[i]);
                return res;
            }
            
            template<int W>
            Matrix<T,N,W> operator *(const Matrix<T,M,W> m) const {
                Matrix<T,N,W> res;
                T tmp;
                for(int i=0;i<N;++i)
                    for(int j=0;j<W;++j) {
                        tmp=0;
                        for(int k=0;k<M;++k)
                            tmp+=at(i,k)*m.at(k,j);
                        res[i][j]=tmp;
                    }
                return res;
            }

            Matrix<T,N,M> operator *=(float s) {
                Matrix<T,N,M> res;
                for(int i=0;i<N;++i)
                    for(int j=0;j<M;++j)
                        res[i][j]=at(i,j)*s;
                (*this)=res;
                return *this;
            }

            template<typename U,int V,int W>
            friend Matrix<U,V,W> &operator *=(Matrix<U,V,W> &m,float s);

            Matrix<T,N,1> &operator *=(Vector<T,N> v) {
                Vector<T,N> resv = (*this)*v;
                Matrix<T,N,1> resm;
                for(int i=0;i<N;++i)
                    resm[i][0]=v[i];
                *this=resm;
                return *this;
            }

            template<int W>
            Matrix<T,N,W> &operator *=(Matrix<T,M,W> &m) {
                Matrix<T,N,W> res;
                T tmp;
                for(int i=0;i<N;++i)
                    for(int j=0;j<W;++j) {
                        tmp=0;
                        for(int k=0;k<M;++k)
                            tmp+=array[i][k]*m[k][j];
                        res[i][j]=tmp;
                    }
                (*this)=res;
                return *this;
            }

            bool operator==(const Matrix<T,N,M> &m) const {
                for(int i=0;i<N;++i)
                    for(int j=0;j<M;++j)
                        if(at(i,j)!=m.at(i,j))
                            return false;
                return true;
            }

            bool operator!=(const Matrix<T,N,M> &m) const {
                return !((*this)==m);
            }
            
            ~Matrix(){}
    };

    template <typename T, int N, int M>
    Matrix<T,N,M> operator *(Matrix<T,N,M> m,T s){
        return s*m;
    }

    template <typename T, int N, int M>
    Matrix<T,N,M> &operator *=(Matrix<T,N,M> &m,T s){
        m=m*s;
        return m;
    }

    template <typename T, int N, int M>
    std::ostream &operator<<(std::ostream &out, Matrix<T,N,M> m) {
        out << '[';
        for(int i=0;i<N;++i) { 
            out << m.array[i];
            if(i!=N-1) out << '\n';
        }
        out << ']';
        return out;
    }

    using Mat44i=Matrix<int,4,4>;
    using Mat44r=Matrix<float,4,4>;
}

#endif