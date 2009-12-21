#include "projectile.h"


#define METER 0.00168

ObjGeometry *Missile::missileMesh;

ObjGeometry *Bomb::bombMesh;

Projectile::Projectile(Model *src) {
    source = src;
    
	/* Calculate projectile location and velocity. */
	Eigen::Vector3f center = source->position;
	float offset = source->radius;
	float rotationY = source->rotate.y();
	float rotationX = source->rotate.x();
	
	rotationY = DEGREES_TO_RADIANS(-rotationY);
	rotationX = DEGREES_TO_RADIANS(-rotationX);
	
	/* Basically start with unit vector centered at origin
	 * Rotate it, then scale it based on radius of our ship
	 * finally translate its center to be the center of the ship */
	Eigen::Vector3f projPos = center;
	Eigen::Vector3f unitPos = Eigen::Vector3f(1.0, 0.0, 0.0);

	Eigen::Matrix3f rotationMatrixY, rotationMatrixX;
	rotationMatrixY << cos(rotationY), 0, -sin(rotationY), 0, 1, 0, sin(rotationY), 0, cos(rotationY);
	rotationMatrixX << 1, 0, 0, 0, cos(rotationX), sin(rotationX), 0, -sin(rotationX), cos(rotationX);

	unitPos = rotationMatrixX * rotationMatrixY * unitPos;
	unitPos *= offset;
	projPos += unitPos;

	Eigen::Vector3f bombVel = projPos - center;
	
	velocity = bombVel;
	position = projPos;
	rotate = source->rotate;
}

Projectile::Projectile(Eigen::Vector3f pos, Eigen::Vector3f vel) : ActiveModel(pos, vel) {
    source = NULL;
}

Missile::Missile(Model *src) : Projectile(src) {
    init();
    playAudio("missile");
}

Missile::Missile(Eigen::Vector3f pos, Eigen::Vector3f vel) : Projectile(pos, vel) {
    init();
    scale = Eigen::Vector3f(.01, .01, .01);
    velocity *= 5;
}

void Missile::init() {
    mesh = missileMesh;
    radius = 0.050;
    velocity *= 2.5;
    shipPart = true;
    scale = Eigen::Vector3f(3, 3, 2);
    //BS INIT
    calculateTransform();
    bs = new BoundingSphere(this);
    bs->update(mesh);
    bs->transform(&transM, scale[1]);
    t = 0;
    color = Eigen::Vector3f(0, 1.0, 0);
}

void Missile::die() {
    if (dead) {
        return;
    }
    mesh = NULL;
    dead = true;
}

void Missile::loadMeshes(){
    missileMesh = new ObjGeometry("geometry/laser.obj");
    debug("Loaded missile mesh");
}

Bomb::Bomb(float power, Model *src) : Projectile(src) {
    mesh = bombMesh;
    t = 0;
    color = Eigen::Vector3f(1.0, 0, 0);
    radius = 0.050;
    velocity += 2*velocity*power;
    scale = Eigen::Vector3f(.05, .05, .05);
    //BS INIT
    calculateTransform();
    bs = new BoundingSphere(this);
    bs->update(mesh);
    bs->radius = radius;
    bs->transform(&transM, scale[0]);
}

void Bomb::applyForce(Force *force) {
    if (dead) return;
    
    force->affect(this);
}

void Bomb::hit(Model *model){
    model->bs->center = model->position;
    model->bs->bounce(&position, &velocity, radius);
}

void Bomb::loadMeshes(){
    bombMesh = new ObjGeometry("geometry/ball.obj");
    debug("Loaded bomb mesh");
}

MissileList *Bomb::explode() {
    MissileList *specks = new MissileList();
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < 2; j++) {
            for (unsigned int k = 0; k < 2; k++) {
                specks->push_back(new Missile(position,
                    Eigen::Vector3f((i?1:-1)*0.033, (j?1:-1)*0.033, (k?1:-1)*0.033)));
            }
        }
    }
    specks->push_back(new Missile(position, Eigen::Vector3f(0, 0.033, 0)));
    specks->push_back(new Missile(position, Eigen::Vector3f(0, -0.033, 0)));
    
    die();
    
    return specks;
}
