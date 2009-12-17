#ifndef _plane_h_included_
#define _plane_h_included_

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

class Plane : public Geometry {
public:
    Plane();
    void dump();
    void draw();
    void update();
    Eigen::Vector3f ul,ll,ur,lr;
};

#endif
