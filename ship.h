#ifndef _ship_h_included_
#define _ship_h_included_

#include "activemodel.h"

#define BARREL_ROLL_FRAMES 30
#define BARREL_ROLL_REVOLUTIONS 2

class Ship : public ActiveModel {

public:
	Ship();
	void draw();
    void updateDirection(unsigned int mouseX, unsigned int mouseY,
        unsigned int width, unsigned int height);
	void update();
	void hit(Model*);
    void doABarrelRoll();
    bool frozen;
	
private:
    float calculateDecceleration(float current);
    void updateBarrelRoll();
    
    Direction direction;
    int barrelRoll;
};

#endif
