#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "scene_interface.h"
#include "gui.h"
#include "gui_interface.h"
#include "camera.hpp"
#include "object3d.hpp"
#include "triangle.hpp"
#include "point.hpp"

using namespace libgeometry;

class Scene : public SceneInterface {
    private:
        gui::Gui *gui;
        Camera camera;
        std::vector<Object3D *> objects;

    public:
        Scene() {}
        Scene(gui::Gui *g, Camera c) : gui(g), camera(c) {}

        // Draws all objects in the field of vision of the camera.
        virtual void draw() const {
            Transform<float> transform=camera.get_transform();
            for(size_t i=0;i<objects.size();++i) {
                //Transform<float> o_transform=objects[i]->getTransform();
                transform = transform.concat(objects[i]->getTransform());
                Sphere<float,4> bs=transform.apply(objects[i]->bsphere());
                //bs=transform.apply(objects[i]->bsphere());
                if(!camera.outside_frustum(bs))
                    draw_object(objects[i]);
            }
        }

        virtual void press_up() {camera.move_up();};
        virtual void press_down() {camera.move_down();};
        virtual void press_left() {camera.move_left();};
        virtual void press_right() {camera.move_right();};
        virtual void press_space() {camera.reset();};
        virtual void press_w() {camera.turn_x_up();};
        virtual void press_s() {camera.turn_x_down();};
        virtual void press_a() {camera.turn_y_up();};
        virtual void press_d() {camera.turn_y_down();};
        virtual void press_q() {camera.turn_z_up();};
        virtual void press_e() {camera.turn_z_down();};
        virtual void press_z() {camera.zoom_in();};
        virtual void press_x() {camera.zoom_out();};

        virtual void release_updown() {camera.stop_move_updown();};
        virtual void release_leftright() {camera.stop_move_leftright();};
        virtual void release_space() {};
        virtual void release_ws() {camera.stop_turn_x();};
        virtual void release_ad() {camera.stop_turn_y();};
        virtual void release_qe() {camera.stop_turn_z();};
        virtual void release_zx() {camera.stop_zoom();};

        virtual void update() {
            camera.update();
            draw();
        };

        // Draws all sides of the object given as argument that are facing the camera.
        void draw_object(const Object3D *o) const {
            Triangle<float,4> tmp;
            Transform<float> o_transform=o->getTransform();
            Transform<float> transform=o_transform.concat(camera.get_transform());
            for(size_t i=0;i<o->num_faces();++i) {
                Triangle<float,4> t=o->face(i);
                tmp=Triangle<float,4>(transform.apply(t.get_p0()),transform.apply(t.get_p1()),transform.apply(t.get_p2()));
                if(camera.sees(tmp)) draw_wire_triangle(tmp);
            }
        }

        // Draws the face given as argument (the three edges of the triangle).
        void draw_wire_triangle(const Triangle<float,4> &t1) const {
            draw_edge(t1.get_p0(),t1.get_p1());
            draw_edge(t1.get_p0(),t1.get_p2());
            draw_edge(t1.get_p1(),t1.get_p2());
        }

        // Draws the segment given as argument.
        void draw_edge(const Point<float,4> &p1, const Point<float,4> &p2) const {
            LineSegment<float,4> ls(p1,p2);
            ls=camera.visible_part(ls);
            if(!ls.is_null()) {
                Point<float,2> p1_2=perspective_projection(ls.get_begin());
                Point<float,2> p2_2=perspective_projection(ls.get_end()); 
                gui->render_line((Vector<float,2>) p1_2,(Vector<float,2>) p2_2,gui::white);
            }
        }

        // Projects the point given as argument on the screen (“near plane”).
        Vec2r perspective_projection(const Point<float,4> &p) const { 
            float w=(p.at(3)==0)?1:p.at(3);
            return Vec2r{p.at(0)/w,p.at(1)/w};
        }

        // Adds an object in the scene.
        void addObject3D(Object3D *o) {
            objects.push_back(o);
        }

        ~Scene() {
            for(size_t i=0;i<objects.size();++i)
                delete objects[i];
        }
};

#endif