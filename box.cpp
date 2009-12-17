#include "box.h"
#include "log.h"

Box::Box() : Geometry("box") {
    uln = Eigen::Vector3f(-1,1,-1);
    lln = Eigen::Vector3f(-1,-1,-1);
    lrn = Eigen::Vector3f(1,-1,-1);
    urn = Eigen::Vector3f(1,1,-1);
    
    ulf = Eigen::Vector3f(-1,1,1);
    llf = Eigen::Vector3f(-1,-1,1);
    lrf = Eigen::Vector3f(1,-1,1);
    urf = Eigen::Vector3f(1,1,1);
    update();
};

void Box::draw(){
    glBegin(GL_POLYGON);
    glVertex3f(urn[0], urn[1], urn[2]);
    glVertex3f(uln[0], uln[1], uln[2]);
    glVertex3f(lln[0], lln[1], lln[2]);
    glVertex3f(lrn[0], lrn[1], lrn[2]);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(uln[0], uln[1], uln[2]);
    glVertex3f(urn[0], urn[1], urn[2]);
    glVertex3f(urf[0], urf[1], urf[2]);
    glVertex3f(ulf[0], ulf[1], ulf[2]);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(urn[0], urn[1], urn[2]);
    glVertex3f(lrn[0], lrn[1], lrn[2]);
    glVertex3f(lrf[0], lrf[1], lrf[2]);
    glVertex3f(urf[0], urf[1], urf[2]);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(lln[0], lln[1], lln[2]);
    glVertex3f(uln[0], uln[1], uln[2]);
    glVertex3f(ulf[0], ulf[1], ulf[2]);
    glVertex3f(llf[0], llf[1], llf[2]);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(lrn[0], lrn[1], lrn[2]);
    glVertex3f(lln[0], lln[1], lln[2]);
    glVertex3f(llf[0], llf[1], llf[2]);
    glVertex3f(lrf[0], lrf[1], lrf[2]);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(ulf[0], ulf[1], ulf[2]);
    glVertex3f(urf[0], urf[1], urf[2]);
    glVertex3f(lrf[0], lrf[1], lrf[2]);
    glVertex3f(llf[0], llf[1], llf[2]);
    glEnd();
    
}

void Box::update(){
    bmin = Eigen::Vector3f(-1,-1,-1);
    bmax = Eigen::Vector3f(1,1,1);
    center = Eigen::Vector3f(0,0,0);
    radius = 1.73205081;
}

void Box::dump(){
    LOG("\tType:\tBox",1);
}
