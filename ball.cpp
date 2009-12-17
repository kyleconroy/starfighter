#include "ball.h"
#include "objgeometry.h"

Ball::Ball() {
    mesh = new ObjGeometry("geometry/ball.obj");
    scale = Eigen::Vector3f(.05,.05,.05);
    calculateTransform();
    t = 0;
}

void Ball::move(Direction dir) {

}
