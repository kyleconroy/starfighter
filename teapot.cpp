#include "teapot.h"
#include "log.h"

Teapot::Teapot() : Geometry("teapot") {
}

void Teapot::dump() {
    LOG("\tType:\tTeapot",1);
}

void Teapot::draw(){
    glPushMatrix();
	glutSolidTeapot(.25);
	glPopMatrix();
};
