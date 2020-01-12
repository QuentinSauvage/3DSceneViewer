#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <iostream>
#include <point.hpp>

using namespace std;

namespace libgeometry {
    
    template<typename T,int N>
    class Rectangle {
        private:
            Point<T,N> p;
            float width,height;
        public:
            Rectangle(){}

            // Returns true if the rectangle contains an invalid value, and false otherwise.
            bool is_null() { return p.is_null(); }

            template<typename U,int M>
            friend std::ostream &operator <<(std::ostream &out, const Rectangle<U,M> r);

            ~Rectangle(){}
    };

    template<typename T,int N>
    std::ostream &operator <<(std::ostream &out, const Rectangle<T,N> r) {
        out << '(' << r.p << ',' << r.width << ',' << r.height << ')';
        return out;
    }
}

#endif