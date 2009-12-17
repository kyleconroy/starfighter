#include "ship.h"
#include "shipgeometry.h" 

Ship::Ship() {
    position = Eigen::Vector3f(0, 0, 0);
    velocity = Eigen::Vector3f(0, 0, -0.2);
    acceleration = Eigen::Vector3f(0, 0, 0);
    rotate = Eigen::Vector3f(0, 90.0, 0);
    mesh = new ShipGeometry("geometry/new_LGS2.obj");
    bs = new BoundingSphere(this);
    bs->center = position;
    bs->radius = 0.084;    
    direction = NONE;
    radius = 0.084;
    health = 100;
    score = -1;
    damage = -1;
    barrelRoll = -1;
    shipPart = true;
    frozen = false;
}

void Ship::hit(Model *m) {
    health -= m->damage;
}

void Ship::update() {
	if (direction != NONE) {
        unsigned int v = (direction == LEFT || direction == RIGHT) ? 0 : 1;
        float decceleration = calculateDecceleration(velocity[v]);
        if (direction == RIGHT || direction == UP) {
            decceleration = -decceleration;
        }
        velocity[v] += decceleration;
        if (fequals(velocity[v], 0.0)) {
            direction = NONE;
        }
    }
    
    updateBarrelRoll();

    if (!frozen){
        velocity.z() = -0.05;
    	position += velocity;
    }
	bs->center = position;
}

void Ship::updateDirection(unsigned int mouseX, unsigned int mouseY,
		unsigned int width, unsigned int height) {
	float mouseZSpace = position.z() - radius - 0.05;
	float mouseXSpace =  (float) mouseX/width - 0.5;
	float mouseYSpace = -((float) mouseY/height - 0.5);

	Eigen::Vector3f mousePos = Eigen::Vector3f(mouseXSpace, mouseYSpace, mouseZSpace);
	float rotateY = RADIANS_TO_DEGREES(atan((mouseXSpace - position.x()) / (mouseZSpace - position.z()))) + 90.0;
	float rotateX = -RADIANS_TO_DEGREES(atan((mouseYSpace - position.y()) / (mouseZSpace - position.z())));

	rotate.y() = rotateY;
	rotate.x() = rotateX;

	Eigen::Vector3f direction = mousePos - position;
	velocity.normalize();
	velocity = direction / 20.0;
	velocity.z() -= 0.01;
}

void Ship::draw() {
    if (!mesh) return;
    glPushMatrix();
    
    glTranslatef(position.x(), position.y(), position.z());
    
    glRotatef(rotate.z(), 0, 0, 1);
    glRotatef(rotate.x(), 1, 0, 0);
    glRotatef(rotate.y(), 0, 1, 0);
    glScalef(scale.x(), scale.y(), scale.z());

    mesh->draw();
    
    glPopMatrix();
}

float Ship::calculateDecceleration(float current) {
    current = fabs(current);
    if (current > 0.0005) {
        return 0.0002;
    }
    return current;
}

void Ship::doABarrelRoll() {
    playAudio("barrelroll");
    if (barrelRoll >= 0) {
        // barrel roll in progress, cannot start a new one
        return;
    } else {
        // start a barrel roll
        barrelRoll = 0;
    }
}

void Ship::updateBarrelRoll() {
    if (barrelRoll < 0) {
        return;
    } else if (barrelRoll >= BARREL_ROLL_FRAMES) {
        // barrel roll completed
        barrelRoll = -1;
        return;
    }
    
    rotate.z() += 360.0*BARREL_ROLL_REVOLUTIONS / BARREL_ROLL_FRAMES;
    barrelRoll++;
}
