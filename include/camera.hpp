#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <math.h>
#include <iostream>
#include "transform.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "lineSegment.hpp"
#include "point.hpp"
#include "direction.hpp"
#include "frustum.hpp"
#include "matrix.hpp"

using namespace libgeometry;
using namespace libmatrix;

#define VISION_ANGLE 80
#define FAR_DISTANCE 100.f
#define NEAR_DISTANCE 0.5f
#define O_SPEED 0.005f
#define D_SPEED 0.0001f
#define Z_SPEED 0.08f

class Camera {
    private:
        float alpha,height,width,cz_speed;
        Point<float,4> position,cd_speed;
        Direction<float,4> direction;
        Quaternion<float> orientation;
        Vec3r co_speed;
        Frustum frustum;
        Mat44r proj_matrix;
        Transform<float> transform_matrix;
        bool zooming;
        
        // Updates the projection matrix of the camera.
        void update_proj_matrix() {
            proj_matrix=Mat44r{0};

            //float t,b,l,r,a=((alpha*M_PI)/180);
            //a=1/tan(a/2);
            //r=t=NEAR_DISTANCE/a;
            //l=b=-r;
            
            float t,r,a=((alpha*M_PI)/180);
            frustum.update(a);
            r=t=NEAR_DISTANCE/a;
            /*proj_matrix=Mat44r{0};
            proj_matrix[0][0]=(2*NEAR_DISTANCE)/(r-l);
            proj_matrix[0][2]=(r+l)/(r-l);
            proj_matrix[1][1]=(2*NEAR_DISTANCE)/(t-b);
            proj_matrix[1][2]=(t+b)/(t-b);
            proj_matrix[2][2]=-((FAR_DISTANCE+NEAR_DISTANCE)/(FAR_DISTANCE-NEAR_DISTANCE));
            proj_matrix[2][3]=-((2*NEAR_DISTANCE*FAR_DISTANCE)/(FAR_DISTANCE-NEAR_DISTANCE));
            proj_matrix[3][2]=-1;*/
            proj_matrix=Mat44r{0};
            proj_matrix[0][0]=(NEAR_DISTANCE)/(r);
            proj_matrix[1][1]=(NEAR_DISTANCE)/(t);
            proj_matrix[2][2]=-((FAR_DISTANCE+NEAR_DISTANCE)/(FAR_DISTANCE-NEAR_DISTANCE));
            proj_matrix[2][3]=-((2*NEAR_DISTANCE*FAR_DISTANCE)/(FAR_DISTANCE-NEAR_DISTANCE));
            proj_matrix[3][2]=-1;
        }

    public:
        Camera() {}
        Camera(float h, float w) : zooming(false){
            direction=Direction<float,4>{0.f,0.f,1.f};
            orientation=Quaternion<float>(0,direction);
            height=h;
            width=w;
            alpha=VISION_ANGLE;
            cd_speed=Vec3r{0,0,0};
            co_speed=Vec3r{0,0,0};
            cz_speed=1;
            position=Point<float,4>{0.f,0.f,-1.f};
            frustum=Frustum(FAR_DISTANCE,NEAR_DISTANCE,width/height);
            update_proj_matrix();
        }

        //Resets the camera.
        void reset() {
            direction=Direction<float,4>{0.f,0.f,1.f};
            orientation=Quaternion<float>(0,direction);
            position=Point<float,4>{0.f,0.f,-1.f};
            cd_speed=Vec3r{0,0,0};
            co_speed=Vec3r{0,0,0};
            alpha=VISION_ANGLE;
            update_proj_matrix();
        }

        // Moves up the camera.
        void move_up() {
            cd_speed[1]=D_SPEED;
        }

        // Moves down the camera.
        void move_down() {
            cd_speed[1]=-D_SPEED;
        }

        // Moves the camera on the left.
        void move_left() {
            cd_speed[0]=-D_SPEED;
        }

        // Moves the camera on the right.
        void move_right() {
            cd_speed[0]=D_SPEED;
        }

        // Turns the camera around the x axis (clockwise).
        void turn_x_up() {
            co_speed[0]=O_SPEED;
        }

        // Turns the camera around the x axis (anti-clockwise).
        void turn_x_down() {
            co_speed[0]=-O_SPEED;
        }

        // Turns the camera around the y axis (clockwise).
        void turn_y_up() {
            co_speed[1]=O_SPEED;
        }

        // Turns the camera around the y axis (anti-clockwise).
        void turn_y_down() {
            co_speed[1]=-O_SPEED;
        }

        // Turns the camera around the z axis (clockwise).
        void turn_z_up() {
            co_speed[2]=O_SPEED;
        }

        // Turns the camera around the z axis (anti-clockwise).
        void turn_z_down() {
            co_speed[2]=-O_SPEED;
        }

        // Decreases the focal distance.
        void zoom_in() {
            if(alpha<260) {
                zooming=true;
                alpha+=Z_SPEED;
            }
        }

        // Increases the focal distance.
        void zoom_out() {
            if(alpha>0) {
                zooming=true;
                alpha-=Z_SPEED;
            }
        }
        
        void stop_move_updown() {
            cd_speed[1]=0;
        }

        void stop_move_leftright() {
            cd_speed[0]=0;
        }

        void stop_turn_x() {
            co_speed[0]=0;
        }

        void stop_turn_y() {
            co_speed[1]=0;
        }

        void stop_turn_z() {
            co_speed[2]=0;
        }

        void stop_zoom() {
            zooming=false;
        }

        // Returns the transform corresponding to the viewpoint of the camera.
        Transform<float> get_transform() const {
            return transform_matrix;
        }

        // Returns if the sphere given as argument is outside of the field of view of the camera.
        bool outside_frustum(const Sphere<float,4> &s) const {
            return frustum.outside(s);
        }
        
        // Returns if the camera “sees” the triangular face given as argument.
        bool sees(Triangle<float,4> &t) const {
            Direction<float,4>test=direction;
            test[0]+=position.at(0);
            test[1]+=position.at(1);
            return test.dot(t.normale())>0;
        }

        // Returns the visible part of the segment given as argument.
        LineSegment<float,4> visible_part(const LineSegment<float,4> &ls) const {
            return frustum.inter(ls);
        }

        // Updates the position and orientation of the camera.
        void update() {
            if(zooming)
                update_proj_matrix();
             if(co_speed.at(0)!=0||co_speed.at(1)!=0||co_speed.at(2)!=0) {
                 orientation*=Quaternion<float>(1.f,Direction<float,4>{co_speed.at(0),co_speed.at(1),co_speed.at(2)});
                 Vec4r o_unit=orientation.to_unit();
                 orientation=Quaternion<float>{o_unit.at(0),o_unit.at(1),o_unit.at(2),o_unit.at(3)};
             }
            position+=cd_speed;

            transform_matrix=Transform<float>(Vec3r{position.at(0),position.at(1),position.at(2)}).concat(Transform<float>(orientation));
            transform_matrix=Transform<float>(transform_matrix.getM().inverse()).concat(Transform<float>(proj_matrix));
        }

        ~Camera() {}
};

#endif