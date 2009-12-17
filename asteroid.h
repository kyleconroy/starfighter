#ifndef _asteroid_h_included_
#define _asteroid_h_included_

#include "ball.h"
#include "enemy.h"
#include "objgeometry.h"

#define NUM_ASTEROIDS 18

/** Asteroid object, basic enemy that moves. */
class Asteroid : public Ball, Enemy {

public:
	Asteroid();
	Asteroid(int num);
	//Asteroid::Asteroid(Eigen::Vector3f pos, Eigen::Vector3f vel);
	void updateEnemy(Level *);
    static void loadMeshes();

private:
    static ObjGeometry *asteroidMesh[NUM_ASTEROIDS];
};

#endif
