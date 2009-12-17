#ifndef _box_h_included_
#define _box_h_included_

#include "geometry.h"
#include <vector>
#include <string>
#include <Eigen/Core>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

class Box : public Geometry {
public:
    Box();
    void dump();
    void draw();
    void update();
    Eigen::Vector3f uln,lln,urn,lrn,ulf,llf,urf,lrf;
};

#endif
