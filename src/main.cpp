#include <iostream>
#include <fstream>
#include <string>
#include "gui.h"
#include "scene.hpp"
#include "object3d.hpp"

int x = 0;

// Opens a file in .geo format and inserts the object in the scene.
void load_geo_file(const char *file, Scene &scene) {
    ifstream f(file);
    std::string line;
    int nb,i1,i2,i3;
    float f1,f2,f3;
    while(f.good()) {
        Object3D *o=new Object3D(x);
        f >> nb;
        for(int i=0;i<nb;++i) {
            f >> f1 >> f2 >> f3;
            o->add_vertex(f1,f2,f3);
        }
        f >> nb;
        for(int i=0;i<nb;++i) {
            f >> i1 >> i2 >> i3;
            o->add_face(i1-1,i2-1,i3-1);
        }
        scene.addObject3D(o);
        x=(x<=0)?(x*-1)+1:x*-1;
    }
}

// Initialises the GUI, reads the file (or files) in .geo format given as argument,
// executes the main_loop and closes the GUI.
// The function must also capture eventual exceptions and treat them, if possible.
int main(int argc, const char *argv[]) {
    gui::Gui *g = new gui::Gui();
    Camera c(g->get_win_height(),g->get_win_width());
    Scene *scene = new Scene(g,c);
    for(int i=1;i<argc;++i)
        load_geo_file(argv[i],*scene);
    g->start();
    g->main_loop(scene);
    g->stop();
    free(g);
    free(scene);
    return 0;
}
