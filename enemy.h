#ifndef _enemy_h_included_
#define _enemy_h_included_
#include "level.h"

class Enemy {
	public:
		virtual void updateEnemy(Level *) = 0;
};

#endif