#ifndef _kamikaze_h_included_
#define _kamikaze_h_included_

#include "asteroid.h"
#include "enemy.h"
#include "level.h"

/** Asteroid object, basic enemy that moves. */
class Kamikaze : public Asteroid {

public:
	Kamikaze();

	void updateEnemy(Level *);
};

#endif
