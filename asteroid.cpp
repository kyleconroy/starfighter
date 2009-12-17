#include "asteroid.h"
#include "objgeometry.h"
#include "model.h"

Asteroid::Asteroid() : Ball() {
	damage = 20;
	health = 3;
	score = 20;
	mesh = new ObjGeometry("geometry/asteroid1.obj");
    ObjGeometry *ast;
    ast = (ObjGeometry*) mesh;
	ast->calcBoundingSphere();
    bs = new BoundingSphere(this);
    bs->update(mesh);
	color = Eigen::Vector3f(0.5,0.5,0.5);
    isAsteroid = true;
}

Asteroid::Asteroid(int num) : Ball() {
	damage = 20;
	health = 1;
	score = 20;
	char filename [50];
	sprintf(filename, "geometry/asteroid%d.obj", num);
	mesh = new ObjGeometry(filename);
    ObjGeometry *ast;
    ast = (ObjGeometry*) mesh;
	ast->calcBoundingSphere();
    bs = new BoundingSphere(this);
    bs->update(mesh);
	color = Eigen::Vector3f(0.5,0.5,0.5);
	isAsteroid = true;
}

//Asteroid::Asteroid(Eigen::Vector3f pos, Eigen::Vector3f vel) : Ball(pos, vel) {
//	damage = 100;
//	health = 3;
//	score = 20;
//}

void Asteroid::updateEnemy(Level * currLev) {
	//rotate.y() += 3;
	//rotate.z() += 3;
	if (!dead) {
		rotate.x() += 3;
	}
}