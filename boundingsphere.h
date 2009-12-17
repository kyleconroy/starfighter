#ifndef _bsphere_h_included_
#define _bsphere_h_included_

#include "boundingshape.h"
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/LU>

class BoundingSphere : public BoundingShape {
public:
    BoundingSphere(Model*);
    BoundingSphere(Model*,Eigen::Vector3f, float);
    void transform(Eigen::Matrix4f*, float);
    bool intersect(BoundingSphere*);
    void addSelf(kdtree*);
    void update(Geometry*);
    void dump();
    void bounce(Eigen::Vector3f*, Eigen::Vector3f*, float);
};

#endif
