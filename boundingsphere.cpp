#include "boundingsphere.h"
#include "log.h"
#include <Eigen/Core>

BoundingSphere::BoundingSphere(Model *m) {
    model = m;
    radius = 1;
    center = Eigen::Vector3f(0,0,0);
};

BoundingSphere::BoundingSphere(Model *m, Eigen::Vector3f p, float rad) {
    model = m;
    radius = rad;
    center = p;
};

bool BoundingSphere::intersect(BoundingSphere* s){
    Eigen::Vector3f p2 = s->center;
    float r2 = s->radius;
    // std::cout << " [" << center.x() << " " << center.y() << " " << center.z() << "] " << radius << " | ";
    // std::cout << " [" << s->center.x() << " " << s->center.y() << " " << s->center.z() << "] " << s->radius << std::endl;
    return (center - p2).norm() <= (radius + r2);
};

void BoundingSphere::transform(Eigen::Matrix4f *t, float scale) {
    radius = radius * scale;
    Eigen::Matrix4f tm = *t;
    Eigen::Vector4f newc(center.x(), center.y(), center.z(), 1);
    newc = tm * newc;
    center = Eigen::Vector3f(newc.x(),newc.y(),newc.z());
};


void BoundingSphere::addSelf(kdtree *tree){
    kd_insert3f(tree,center.x(), center.y(), center.z(),this);
    //std::cout << " [" << center.x() << " " << center.y() << " " << center.z() << "] " << std::endl;
};

void BoundingSphere::update(Geometry *mesh){
    radius = mesh->radius;
    center = mesh->center;
};

void BoundingSphere::bounce(Eigen::Vector3f* pi, Eigen::Vector3f* ve, float r){
    Eigen::Vector3f normal = *pi - center;
    normal.normalize();
    *pi = center + (radius + r + (.1 * r)) * normal;
    //debug("%f", radius + model->bs->radius);
    *ve = (*ve - 2 * ve->dot(normal) * normal);//.normalized();
};

void BoundingSphere::dump(){
    LOG("\t\tBoundingShape:\t Sphere",1);
    BoundingShape::dump();
};