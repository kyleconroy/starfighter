#ifndef _asteroid_h_included_
#define _asteroid_h_included_

#include "ball.h"
#include "enemy.h"

/** Asteroid object, basic enemy that moves. */
class Asteroid : public Ball, Enemy {

public:
	Asteroid();
	Asteroid(int num);
	//Asteroid::Asteroid(Eigen::Vector3f pos, Eigen::Vector3f vel);
	void updateEnemy(Level *);

};

#endif
