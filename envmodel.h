#ifndef _envmodel_h_included_
#define _envmodel_h_included_

#include "passivemodel.h"
#include "soil/SOIL.h"
#include <string>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif
/**
 * EnvironmentModel - an immoveable, indestructible object.
 */
class EnvironmentModel : public PassiveModel {
public:
    EnvironmentModel();
	EnvironmentModel(Eigen::Vector3f);
	static GLuint loadImage(char * filename);
	static GLuint texture_id;
    void hit(Model *otherModel);
    void draw();
};

#endif
