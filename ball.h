#ifndef _ball_h_included_
#define _ball_h_included_

#include "objgeometry.h"
#include "activemodel.h"


class Ball : public ActiveModel {
    
public:
	Ball();
    void move(Direction dir);
	static void loadMeshes();
private:
    Direction direction;
    static ObjGeometry *ballMesh;
};

#endif
