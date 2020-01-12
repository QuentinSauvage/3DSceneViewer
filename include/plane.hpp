#ifndef PLANE_HPP
#define PLANE_HPP

#include <iostream>
#include "libmatrix.h"

using namespace std;

namespace libgeometry {
    using namespace libmatrix;
    
    template<typename T, int N>
    class Plane : public Vector<T,N> {
        public:
            Plane(){}
            Plane(Vector<T,N> v) {
                v=v.to_unit();
                for(int i=0;i<N;++i) this->array[i]=v[i];
            }
            ~Plane(){}
    };
}

#endif