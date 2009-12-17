#ifndef _turret_h_included_
#define _turret_h_included_

#include "passivemodel.h"
#include "enemy.h"
#include "level.h"

/** Asteroid object, basic enemy that moves. */
class Turret : public PassiveModel, Enemy {

public:
	Turret();
	Turret(Eigen::Vector3f pos);
	void updateEnemy(Level *);
	
    int cooldown;
};

#endif
