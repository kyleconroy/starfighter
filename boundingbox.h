#ifndef _bbox_h_included_
#define _bbox_h_included_

#include "boundingshape.h"
#include "boundingsphere.h"
#include <Eigen/Core>

class BoundingBox : public BoundingShape {
public:
    BoundingBox(Model*);
    void transform(Eigen::Matrix4f*, float);
    void transformVector(Eigen::Matrix4f*, Eigen::Vector3f*);
    bool intersect(BoundingSphere*);
    float distToPlane(Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f);
    void addSelf(kdtree*);
    void update(Geometry*);
    void dump();
    void bounce(Eigen::Vector3f*, Eigen::Vector3f*, float);
    Eigen::Vector3f uln,lln,urn,lrn,ulf,llf,urf,lrf;
};

class PlaneCollision {
public:
    Eigen::Vector3f P, A, B, C;
    float d;
    float distToPlane(Eigen::Vector3f p, Eigen::Vector3f A, Eigen::Vector3f B, Eigen::Vector3f C){
        Eigen::Vector3f v = (B - A).cross(C - A);
        v.normalize();
        return -v.dot(p - A);
    };
    Eigen::Vector3f normal(){
        return (B - A).cross(C - A).normalized();
    }
    PlaneCollision(){};
    PlaneCollision(Eigen::Vector3f p, Eigen::Vector3f a, Eigen::Vector3f b, Eigen::Vector3f c){
        P = p;
        A = a;
        B = b;
        C = c;
        d = distToPlane(P, A, B, C);
    };
};


#endif
