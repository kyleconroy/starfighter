#ifndef _explosion_h_included_
#define _explosion_h_included_

#include "geometry.h"
#include "soil/SOIL.h"
#include <string>

#define FPP 1           // frames per picture, must be an integer
#define NUM_IMAGES 31    // explosion animation has 31 pictures (includes blank)

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

class Explosion : public Geometry {
public:
    Explosion();
    void dump();
    void draw();
    void update();
    
	static void loadAnimationFrames();
	static GLuint loadImage(char * filename);

	static GLuint texture_ids[NUM_IMAGES];
	unsigned int current_texture_index;
	unsigned int frame_counter;

};

#endif
