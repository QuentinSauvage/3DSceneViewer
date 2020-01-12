#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include "libmatrix.h"

using namespace std;

namespace libgeometry {
    using namespace libmatrix;
    
    template<typename T, int N>
    class Direction : public Vector<T,N> {
        public:
            Direction() {
                this->array[N-1]=0;
            }

            Direction(const Vector<T,N> v) {
                for(size_t i=0;i<N;++i)
                    this->array[i]=v.at(i);
            }

            Direction(const Vector<T,N-1> v) {
                for(size_t i=0;i<N-1;++i)
                    this->array[i]=v.at(i);
                this->array[N-1]=0;
            }

            Direction(std::initializer_list<T> l) {
                for(size_t i=0;i<l.size();++i)
                    this->array[i]=l.begin()[i];
                this->array[N-1]=0;
            }

            ~Direction(){}
    };
}

#endif