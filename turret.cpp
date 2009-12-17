#include "turret.h"
#include "main.h"
#include "projectile.h"
#include <math.h>

Turret::Turret() : PassiveModel() {}

Turret::Turret(Eigen::Vector3f pos) : PassiveModel(pos) {
	health = 2;
	score = 10;
	mesh = new Teapot();
}

void Turret::updateEnemy(Level* currLev) {
	//TODO rotate turret and fire at enemey
	// TODO calculate rotation of turret

	Eigen::Vector3f shipPos = currLev->ship->position;
	cooldown++;
	//debug ("count is %d\n", count);
	//rotate teapot to face ship:
	Eigen::Vector3f dirToShip = shipPos - position;
	dirToShip.normalize();

	float rotateY = atan(dirToShip.z() /dirToShip.x());
	this->rotate.y() = rotateY;

	// if we're sufficiently in front of the ship and allow time for cooldown
	if (position.z() < shipPos.z() - 0.1 && cooldown == 20) {
		//create bullet fire at ship
		Missile* missle = new Missile(this);
		missle->shipPart = false;

		currLev->amodels->push_back(missle);
		cooldown = 0;
	}
}
