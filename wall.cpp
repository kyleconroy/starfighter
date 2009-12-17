#include "wall.h"
#include <cmath>
#include "log.h"

BackWall::BackWall() : Wall(0) {}

BackWall::BackWall(float i) : Wall(i) {}

bool BackWall::collide(Eigen::Vector3f vec, Eigen::Vector3f ship){
    float z = vec.z();
    return z < (dist + ship.z());
}

void BackWall::dump() {
    LOG("\tType:\tBackWall",1);
    LOG("\t\tMove:\t" << dist ,1);
}

FrontWall::FrontWall() : Wall(0) {}

FrontWall::FrontWall(float i) : Wall(i) {}

bool FrontWall::collide(Eigen::Vector3f vec, Eigen::Vector3f ship){
    float z = vec.z();
    return z > dist;
}

void FrontWall::dump() {
    LOG("\tType:\tFrontWall",1);
    LOG("\t\tMove:\t" << dist ,1);
}

BottomWall::BottomWall() : Wall(0) {}

BottomWall::BottomWall(float i) : Wall(i) {}

bool BottomWall::collide(Eigen::Vector3f vec, Eigen::Vector3f ship){
    float y = vec.y();
    return y < dist;
}

void BottomWall::dump() {
    LOG("\tType:\tBottomWall",1);
    LOG("\t\tMove:\t" << dist ,1);
}

TopWall::TopWall() : Wall(0) {}

TopWall::TopWall(float i) : Wall(i) {}

bool TopWall::collide(Eigen::Vector3f vec, Eigen::Vector3f ship){
    float y = vec.y();
    return y > dist;
}

void TopWall::dump() {
    LOG("\tType:\tTopWall",1);
    LOG("\t\tMove:\t" << dist ,1);
}

RightWall::RightWall() : Wall(0) {}

RightWall::RightWall(float i) : Wall(i) {}

bool RightWall::collide(Eigen::Vector3f vec, Eigen::Vector3f ship){
    float x = vec.x();
    return dist < x;
}

void RightWall::dump() {
    LOG("\tType:\tRightWall",1);
    LOG("\t\tMove:\t" << dist ,1);
}

LeftWall::LeftWall() : Wall(0) {}

LeftWall::LeftWall(float i) : Wall(i) {}

bool LeftWall::collide(Eigen::Vector3f vec, Eigen::Vector3f ship){
    float x = vec.x();
    return x < dist;
}

void LeftWall::dump() {
    LOG("\tType:\tLeftWall",1);
    LOG("\t\tMove:\t" << dist ,1);
}
