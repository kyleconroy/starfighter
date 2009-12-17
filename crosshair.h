#ifndef _crosshair_h_included_
#define _crosshair_h_included_

#include "geometry.h"
#include <string>
#include "soil/SOIL.h"
#include "main.h"

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

class Crosshair : public Geometry {
public:
    Crosshair();
    void dump();
    void draw();
    void update();
    
	GLuint loadImage(char * filename);
	GLuint texture_id;
};

#endif