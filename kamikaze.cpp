#include "kamikaze.h"
#include "objgeometry.h"

Kamikaze::Kamikaze() : Asteroid() {
	score = 100;
	damage = 20;
	health = 1;
	mesh = new ObjGeometry("geometry/ship.obj");
	color = Eigen::Vector3f(0.7,0.0,0.2);
	velocity = Eigen::Vector3f(0, 0, 0);
	scale = Eigen::Vector3f(1.2, 1.2, 1.2);


	//BS INIT
    calculateTransform();
    bs = new BoundingSphere(this);
    bs->update(mesh);
    bs->transform(&transM, scale[1]);

}

void Kamikaze::updateEnemy(Level * currLev) {
	//todo move at ship;

	if (!dead) {
		Eigen::Vector3f shipLoc = currLev->ship->position;
		Eigen::Vector3f direction = shipLoc - position;
		
		direction.normalize();
		direction /= 1000;
		direction.z() -= currLev->ship->velocity.z();



		Eigen::Vector3f mousePos = Eigen::Vector3f(shipLoc.x(), shipLoc.y(), shipLoc.z());
		float rotateY = RADIANS_TO_DEGREES(atan((shipLoc.x() - position.x()) / (shipLoc.z() - position.z()))) - 90.0;
		float rotateX = -RADIANS_TO_DEGREES(atan((shipLoc.y() - position.y()) / (shipLoc.z() - position.z())));

		rotate.y() = rotateY;
		rotate.x() = rotateX;

		// if we're behind ship, keep going in that direction
		if (direction.z() <= 0) {
			direction[2] *= -1;
			direction[2] += 0.2;
		}
		velocity = direction;
	}
}
