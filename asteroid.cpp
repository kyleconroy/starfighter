#include "asteroid.h"
#include "objgeometry.h"
#include "model.h"

ObjGeometry *Asteroid::asteroidMesh[NUM_ASTEROIDS];

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
	mesh = asteroidMesh[num-1];
    ObjGeometry *ast;
    ast = (ObjGeometry*) mesh;
	ast->calcBoundingSphere();
    bs = new BoundingSphere(this);
    bs->update(mesh);
	color = Eigen::Vector3f(0.5,0.5,0.5);
	isAsteroid = true;
}

void Asteroid::updateEnemy(Level * currLev) {
	if (!dead) {
		rotate.x() += 3;
	}
}

void Asteroid::loadMeshes() {
    char filename[50];
    for (size_t i = 0; i < NUM_ASTEROIDS; i++) {
        sprintf(filename, "geometry/asteroid%lu.obj", i+1);
        asteroidMesh[i] = new ObjGeometry(filename);
    }
}
