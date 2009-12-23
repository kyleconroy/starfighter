#include "level.h"
#include "log.h"
#include "activemodel.h"
#include "projectile.h"
#include "asteroid.h"
#include "kamikaze.h"
#include "turret.h"
#include "boundingsphere.h"
#include "boundingshape.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

Level::Level() {
    amodels = new ModelList();
    pmodels = new ModelList();
    geometries = new GeometryList();
    bombs = new BombList();
    walls = new WallList();
    forces = new ForceList();
    ship = new Ship();
    ptree = kd_create(3);
    atree = kd_create(3);
    myScore = 0;
    maxRadius = 0;
    maxARadius = 0;
    worldDepth = -10.0;
    frame = 0;
    enemieson = true;
}

Level::~Level() {
    size_t i;
    for (i = 0; i < amodels->size(); i++) delete amodels->at(i);
    delete amodels;
    for (i = 0; i < pmodels->size(); i++) delete pmodels->at(i);
    delete pmodels;
    for (i = 0; i < bombs->size(); i++) delete bombs->at(i);
    delete bombs;
    for (i = 0; i < walls->size(); i++) delete walls->at(i);
    delete walls;
    for (i = 0; i < forces->size(); i++) delete forces->at(i);
    delete forces;
    for (i = 0; i < geometries->size(); i++) delete geometries->at(i);
    delete geometries;
    
    kd_free(ptree);
    kd_free(atree);
}

void Level::update() {
    frame++;
    if (frame > 90) {
        frame = 0;
    }

    if (enemieson) {
        generateEnemies();
    }
    
    updateActiveModels();
    checkActiveCollisions();
    checkShipCollisions();
    checkWallCollisions();
}

void Level::updateActiveModels() {
    ActiveModel *am;
    for (size_t i = 0; i < amodels->size(); i++) {
        am = (ActiveModel *) amodels->at(i);
        if (!behindShip(am)) {
            for (size_t j = 0; j < forces->size(); j++) {
                am->applyForce(forces->at(j));
            }
            am->update();
        } 
    }
    updateATree();
}

void Level::checkActiveCollisions() {
    ActiveModel *am;
    Model *model;
    BoundingSphere *bs;
    BoundingShape *bsh;
    struct kdres *presults;
    Eigen::Vector3f pos;
    
    for (size_t i = 0; i < amodels->size(); i++) {
        am = (ActiveModel*) amodels->at(i);
        pos = am->position;
        
        // check against passive objects
        presults = kd_nearest_range3f(ptree, pos.x(), pos.y(), pos.z(), maxRadius + am->bs->radius);
        if (kd_res_size(presults) > 0) {
            /* For each bounding sphere */
            while (!kd_res_end(presults)) {
                bsh =  (BoundingShape *) kd_res_item_data(presults);
                // checks intersection
                if (bsh->intersect((BoundingSphere *) am->bs)) {
                    model = bsh->model;
                    if (!model->dead && !am->dead) {
                        //debug("Passive Hit");
                        
                        if(!am->isAsteroid){
                            model->hit(am);
                            am->hit(model);
                        }
                        
                        if (model->dead && am->shipPart) {
                            myScore += model->getScore(); // sets the score to 0 after reading it
                        }
                    }
                }
                kd_res_next(presults);
            }
        }
        
        // check against active objects
        presults = kd_nearest_range3f(atree, pos.x(), pos.y(), pos.z(), am->radius + maxARadius);
        while (!kd_res_end(presults)) {
            bs = (BoundingSphere *) kd_res_item_data(presults);
            // check intersection
            if (am->bs->intersect(bs)) {
                model = bs->model;                   
                if (model != am && !model->dead && !am->dead) {
                    debug("Active Hit");
                    //debug("%f", am->radius + maxARadius);
                    model->hit(am);
                    am->hit(model);
                    //SCORE WON"T WORK
                    if (model->dead) {
                        myScore += model->getScore(); // sets the score to 0 after reading it
                    }
                }
                
            }
            kd_res_next(presults);
        }
            
        kd_res_free(presults);
    }
}

void Level::checkShipCollisions() {
    Model *model;
    BoundingSphere *bs;
    Eigen::Vector3f pos = ship->position;
    struct kdres *presults = kd_nearest_range3f(ptree, pos.x(), pos.y(), pos.z(), ship->radius + maxRadius);

    if (kd_res_size(presults) > 0) {
        // for each bounding sphere
        while (!kd_res_end(presults)) {
            bs = (BoundingSphere *) kd_res_item_data(presults);
            // check intersection
            if (ship->bs->intersect(bs)) {
                model = bs->model;
                if (!model->dead) {
                    model->hit(ship);
                    ship->hit(model);
                }
            }
            kd_res_next(presults);
        }
    }
    
    presults = kd_nearest_range3f(atree, pos.x(), pos.y(), pos.z(), ship->radius + maxARadius);
    if (kd_res_size(presults) > 0) {
        /* For each bounding sphere */
        while( !kd_res_end( presults ) ) {
            bs = (BoundingSphere*) kd_res_item_data(presults);
            //Checks intersection
            if(ship->bs->intersect(bs)){
                model = bs->model;
                if(!model->dead){
                    model->hit(ship);
                    ship->hit(model);
                }
            }
            kd_res_next( presults );
        }
    }
    
    kd_res_free(presults);
}

void Level::checkWallCollisions() {
    ActiveModel *am;
    Wall *w;
    for (size_t i = 0; i < walls->size(); i++) {
        w = walls->at(i);
        for (size_t j = 0; j < amodels->size(); j++) {
            am = (ActiveModel *) amodels->at(j);
            if (w->collide(am->position, ship->position)) {
                am->die();
                debug("Wall Hit");
            }
        }
    }
}

void Level::clean() {
    Model *model;
    for (size_t i = 0; i < amodels->size(); i++) {
        model = amodels->at(i);
        if(model->dead){
            amodels->erase(amodels->begin()+i);
            if (model->isAsteroid)
                debug("Deleted an asteroid");
            else
                debug("Deleted an acitve model");
            delete model;
            i--;
        }
    }
    for (size_t i = 0; i < bombs->size(); i++) {
        model = (Model*) bombs->at(i);
        if(model->dead){
            bombs->erase(bombs->begin()+i);
            i--;
            debug("Removed a bomb");
        }
    }
}

void Level::calculateMaxRadius() {
    Model *model;
    for (size_t i = 0; i < pmodels->size(); i++) {
        model = pmodels->at(i);
        maxRadius = max(model->bs->radius, maxRadius);
    }
    for (size_t i = 0; i < amodels->size(); i++) {
        model = amodels->at(i);
        maxRadius = max(model->bs->radius, maxRadius);
    }
    debug("%f", maxRadius);
}

void Level::createTree() {
    LOG("[Level] \tMaking tree ",1);
    Model *model;
    for (size_t i = 0; i < pmodels->size(); i++) {
        model = pmodels->at(i);
        model->addSelf(ptree);
        LOG("[Level] \tInserting model " << i,1);
    }
    LOG("[Level] \tDone with tree ",1);
}

void Level::updateATree() {
    kd_free(atree);
    atree = kd_create(3);
    Model *model;
    for (size_t i = 0; i < amodels->size(); i++) {
        model = amodels->at(i);
        maxARadius = max(model->bs->radius, maxARadius);
        model->addSelf(atree);
    }
    //debug("%f", maxARadius);
}

void Level::dump() {
    LOG("[Level] \t Dump" ,1);
    LOG("Max Radius" << maxRadius,1);
    LOG("Max A Radius" << maxARadius,1);
    LOG("Passive Models:" ,1);
    for (size_t i = 0; i < pmodels->size(); i++) {
        pmodels->at(i)->dump();
    }
    LOG("Active Models:" ,1);
    for (size_t i = 0; i < amodels->size(); i++) {
        amodels->at(i)->dump();
    }
    LOG("Geometries:" ,1);
    for (size_t i = 0; i < geometries->size(); i++) {
        geometries->at(i)->dump();
    }
    LOG("Walls:" ,1);
    for (size_t i = 0; i < walls->size(); i++) {
        walls->at(i)->dump();
    }
}

void Level::draw(float farClippingPlane) {    
    drawActive(farClippingPlane);
    drawPassive(farClippingPlane);
    clean();
}

void Level::drawActive(float farClippingPlane) {
    Model *model;
    for (size_t i = 0; i < amodels->size(); i++) {
        model = amodels->at(i);
        if (model->position.z() < farClippingPlane && !behindShip(model)) {
            model->draw();
        }
    }
}

void Level::drawPassive(float farClippingPlane) {
    Model *model;
    for (size_t i = 0; i < pmodels->size(); i++) {
        model = pmodels->at(i);
        if (model->position.z() < farClippingPlane && !behindShip(model)) {
            model->draw();
        }
    }
}

bool Level::behindShip(Model *model) {
    return model->position.z() > ship->position.z() + 5.0;
}

void Level::generateEnemies() {
	// randomly generate enemies to attack the ship
	int random_integer = rand() % 20 + 1;

	//debug("random number %d", random_integer);
            //      if (random_integer == 3) { // 1 in 20 chance we make a kamikaze
            //            Eigen::Vector3f entryPos = ship->position;
            //            entryPos[2] -= 5;
            //            entryPos[0] = -0.2 + (rand() %50) / 100.0;
            //            Kamikaze * suicide = new Kamikaze();
            //            suicide->position = entryPos;
            //           
            //            enemies->push_back((Enemy *)suicide);
            // 
            //          //BS INIT
            // suicide->bs->transform(&suicide->transM, suicide->scale[0]);
            //            amodels->push_back(suicide);
            //     }
    //     
	if (random_integer == 20) { // 1 in 10 chance we make an asteroid
		Eigen::Vector3f entryPos = ship->position;
		entryPos[2] -= 5;
		entryPos[1] = -0.5 + (rand() %100) /100.0;
		entryPos[0] = -0.5 + (rand() %100) / 100.0;
		Eigen::Vector3f vel = (ship->position - entryPos);
		vel.normalize();
		vel /= 30;
		unsigned int astNum = rand() % 18 + 1;
		Asteroid * rock = new Asteroid(astNum);
		rock->position = entryPos;
		rock->velocity = -vel;
        rock->scale = Eigen::Vector3f(0.3,0.3,0.3);
        rock->calculateTransform();
		
		//BS INIT
        rock->bs->transform(&rock->transM, rock->scale[0]);
		amodels->push_back(rock);
	}

	//if (done != 0) {
	//	debug ("creating new turret");
	//	Turret * newTurret = new Turret();

	//	newTurret->position = ship->position;
	//	newTurret->position.z() -= 0.1;
	//	
	//	enemies->push_back((Enemy *) newTurret);
	//	pmodels->push_back(newTurret);
	//	done = 0;
	//}
}

void Level::drawHealthBar() {
	unsigned int myHealth = ship->health;
	if (myHealth < 0 || myHealth > 100) {
        myHealth = 0;
	}
	renderHealthbar((float)myHealth);

	// draw the current health number
	char health_str[4];
	itoa(myHealth, health_str);
	void *font = GLUT_BITMAP_9_BY_15;
	int health_pos_x = 112;
	int health_pos_y = 20;
	Color health_color = Color(1.0,1.0,1.0);
	if (myHealth > 60) {
		health_color = Color(0.0,1.0,0.0);
	} else if (myHealth > 30) {
		health_color = Color(1.0,1.0,0.0);
	} else {
		health_color = Color(1.0,0.0,0.0);
	}
	renderText(health_pos_x, health_pos_y, font, health_str, health_color); 
}

void Level::drawScore() {
	char score_str[MAX_SCORE_DIGITS+1];
	itoa(myScore, score_str);

	renderScore(score_str);
}
