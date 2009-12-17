#include "force.h"
#include "activemodel.h"

Gravity::Gravity() {
    accel = Eigen::Vector3f(0, -9.8, 0);
}

Gravity::Gravity(float x, float y, float z) {
    accel = Eigen::Vector3f(x, y, z);
}

void Gravity::affect(ActiveModel *model) {
    model->velocity += METER * model->t * accel;
};
