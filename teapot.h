#ifndef _teapot_h_included_
#define _teapot_h_included_

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

class Teapot : public Geometry {
public:
    Teapot();
    void dump();
    void draw();
	void update() {}
};

#endif
