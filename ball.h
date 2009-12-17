#ifndef _ball_h_included_
#define _ball_h_included_

#include "activemodel.h"

class Ball : public ActiveModel {
    
public:
	Ball();
    void move(Direction dir);
	
private:
    Direction direction;
};

#endif
