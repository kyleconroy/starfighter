#include "ball.h"

ObjGeometry *Ball::ballMesh;

Ball::Ball() {
    mesh = ballMesh;
    scale = Eigen::Vector3f(.05,.05,.05);
    calculateTransform();
    t = 0;
}

void Ball::move(Direction dir) {

}

void Ball::loadMeshes() {
    ballMesh = new ObjGeometry("geometry/ball.obj");
    debug("Loaded ball.obj");
}