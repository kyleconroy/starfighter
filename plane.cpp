#include "plane.h"
#include "log.h"

Plane::Plane() : Geometry("plane") {
    ul = Eigen::Vector3f(-1,1,0);
    ll = Eigen::Vector3f(-1,-1,0);
    lr = Eigen::Vector3f(1,-1,0);
    ur = Eigen::Vector3f(1,1,0);
    update();
};

void Plane::draw(){
    glBegin(GL_POLYGON);
    glVertex3f(ur[0], ur[1], ur[2]);
    glVertex3f(ul[0], ul[1], ul[2]);
    glVertex3f(ll[0], ll[1], ll[2]);
    glVertex3f(lr[0], lr[1], lr[2]);
    glEnd();    
}

void Plane::dump(){
    LOG("\tType:\tBox",1);
}

void Plane::update(){
    bmin = ll;
    bmax = ur;
}
