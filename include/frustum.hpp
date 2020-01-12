#ifndef FRUSTRUM_HPP
#define FRUSTRUM_HPP

#include <vector>
#include "plane.hpp"
#include "point.hpp"
#include "lineSegment.hpp"

using namespace libgeometry;

class Frustum {
    private:
        Plane<float,4> near,far,right,left,bottom,top;
        float f_dist,n_dist,ratio;

        //Returns the list of planes for which the point given as argument is behind.
        vector<Plane<float,4>> get_planes_behind(Point<float,4> &p) const {
            vector<Plane<float,4>> v;
            if(p.behind(near)) {
                v.push_back(near);}
            if(p.behind(far)) {
                v.push_back(far);}
            if(p.behind(right)) {
                v.push_back(right);}
            if(p.behind(left)) {
                v.push_back(left);}
            if(p.behind(bottom)) {
                v.push_back(bottom);}
            if(p.behind(top)) {
                v.push_back(top);}
            return v;
        }
        
    public:
        Frustum() {}
        Frustum(float f,float n, float r) : f_dist(f), n_dist(n), ratio(r){}

        // Updates the position of the field of vision, where h is the horizontal resolution, v is the vertical resolution
        // and e is the distance between the projection plane and the camera.
        void update(float e) {
            near=Plane<float,4>(Vector<float,4>{0,0,-1,-n_dist});
            far=Plane<float,4>(Vector<float,4>{0,0,1,f_dist});
            left=Plane<float,4>(Vector<float,4>{e,0,-1,0});
            right=Plane<float,4>(Vector<float,4>{-e,0,-1,0});
            bottom=Plane<float,4>(Vector<float,4>{0,e,-ratio,0});
            top=Plane<float,4>(Vector<float,4>{0,-e,-ratio,0});
        }

        // Returns if the point given as argument is outside the field of vision.
        bool outside(const Point<float,4> &p) const {
            return p.behind(near)||p.behind(far)||p.behind(left)||p.behind(right)||p.behind(bottom)||p.behind(top);
        }

        // Returns if the sphere given as argument is completely outside the field of vision.
        bool outside(const Sphere<float,4> &s) const {
            return s.behind(near)||s.behind(far)||s.behind(left)||s.behind(right)||s.behind(bottom)||s.behind(top);
         }

        // Returns the intersection between the segment and the field of vision (visible part).
        LineSegment<float,4> inter(const LineSegment<float,4> &ls) const {
            Point<float,4> b=ls.get_begin(), e=ls.get_end(),p;
            vector<Plane<float,4>> planes_b=get_planes_behind(b),planes_e=get_planes_behind(e);
            
            if(planes_b.size()==0&&planes_e.size()==0) return ls; //both inside
            if(planes_b.size()==0) { //only b inside frustum
                for(size_t i=0;i<planes_e.size();++i) {
                    p=ls.inter(planes_e[i]);
                    if(!p.is_null())
                        return LineSegment<float,4>(b,p);
                }
            }
            if(planes_e.size()==0) { //only e inside frustum
                for(size_t i=0;i<planes_b.size();++i) {
                    p=ls.inter(planes_b[i]);
                    if(!p.is_null())
                        return LineSegment<float,4>(e,p);
                }
            }

            //both points are outside : search two intersection points (0 if no intersection)
            Point<float,4> ls2[2];
            int cpt=0;
            bool present;
            vector<Plane<float,4>> unique_plans=planes_b; //planes for which at least one point is behind
            for(size_t i=0;i<planes_e.size();++i) {
                present=false;
                for(size_t j=0;j<planes_b.size();++j) {
                    if(planes_e[i]==planes_b[j])
                        present=true;
                }
                if(!present)
                    unique_plans.push_back(planes_e[i]);
            }
            for(size_t i=0;i<unique_plans.size();++i) {
                ls2[cpt]=ls.inter(unique_plans[i]);
                if(!ls2[cpt].is_null())
                    if(++cpt==2)
                        return LineSegment<float,4>(ls2[0],ls2[1]);
            }
            return LineSegment<float,4>();
        }

        ~Frustum() {}
};

#endif