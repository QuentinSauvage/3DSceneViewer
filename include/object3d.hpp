#ifndef OBJECT_3D_HPP
#define OBJECT_3D_HPP

#include <string>
#include <vector>
#include "point.hpp"
#include "triangle.hpp"
#include "sphere.hpp"

#define OFFSET 0.5f

class Object3D {
    private:
        std::string name;
        Point<float,4> position;
        std::vector<Point<float,4>> vertices;
        std::vector<Triangle<float,4>> faces;

    public:
        Object3D(int x=0,int y=0, int z=0) {
            position=Point<float,4>{x*OFFSET,y*OFFSET,z*OFFSET};
        }

        // Returns the bounding sphere.
        Sphere<float,4> bsphere() {
            float tmp,max=0;
            for(size_t i=0;i<vertices.size();++i) {
                tmp=(position.length_to(vertices[i])).norm();
                if(tmp>max) max=tmp;
            }
            return Sphere<float,4>(position,max);
        }

        // Returns the n-th face of the object, where n is given as argument.
        Triangle<float,4> face(unsigned int n) const {
            if(n<faces.size()) return faces[n];
            return Triangle<float,4>();
        }

        // Returns the number of faces of the object.
        unsigned int num_faces() const {
            return faces.size();
        }

        // Adds a face to the object. The three integers given as arguments correspond to three vertices.
        void add_face(unsigned int i1, unsigned int i2, unsigned int i3) {
            faces.push_back(Triangle<float,4>(vertices[i1],vertices[i2],vertices[i3]));
        }

        // Deletes a face from the object. The integer given as argument refers to the list of faces.
        void remove_face(unsigned int i) {
            faces.erase(faces.begin()+i);
        }

        // Adds a vertex to the object. The three float given as arguments correspond to the coordinates of the vertex.
        void add_vertex(float f1, float f2, float f3) {
            vertices.push_back(Point<float,4>{f1,f2,f3});
        }

        // Deletes a vertex from the object. The integer given as argument refers to the list of vertices.
        void remove_vertex(unsigned int i) {
            for(size_t j=0;j<faces.size();++j)
                if(faces[j].get_p0()==vertices[i]||faces[j].get_p1()==vertices[i]||faces[j].get_p2()==vertices[i])
                    faces.erase(faces.begin()+j);
            vertices.erase(vertices.begin()+i);
        }

        // Returns the model matrix.
        inline Transform<float> getTransform() const {return Transform<float>(Vec3r{position.at(0),position.at(1),position.at(2)});}

        ~Object3D() {}
};

#endif