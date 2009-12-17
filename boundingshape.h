#ifndef _bshape_h_included_
#define _bshape_h_included_

#include <Eigen/Core>
#include "geometry.h"
#include "kdtree/kdtree.h"
#include "log.h"

class Model;
class BoundingSphere;

class BoundingShape {
public:
    BoundingShape(){};
    virtual ~BoundingShape(){};
    virtual void transform(Eigen::Matrix4f*, float) =0;
    virtual bool intersect(BoundingSphere*) =0;
    virtual void addSelf(kdtree*) =0;
    virtual void update(Geometry*) =0;
    virtual void bounce(Eigen::Vector3f*, Eigen::Vector3f*, float) =0;
    Model* model;
    Eigen::Vector3f center;
    float radius;
    void dump(){
        LOG("\t\tBoundingShape:\t",1);
        LOG("\t\tCenter:\t" << center.x() << ", " << center.y()<< ", " << center.z() ,1);
        LOG("\t\tRadius:\t" << radius,1);
    };
};

#endif
