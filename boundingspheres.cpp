#include "boundingspheres.h"

BoundingSpheres::BoundingSpheres() {};

void BoundingSpheres::add(Eigen::Vector3f pt,float r) {
    centers.push_back(pt);
    radi.push_back(r);
};  


