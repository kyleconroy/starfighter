#ifndef _level_h_included_
#define _level_h_included_

#include "main.h"
#include "passivemodel.h"
#include "ship.h"
#include "kdtree/kdtree.h"

#define MAX_SCORE_DIGITS 10

class Level {
public:
    Level();
    ~Level();

    void update();
    void draw(float farClippingPlane);
    void dump();
    void createTree();
    void updateATree();
    void generateEnemies();
    void drawHealthBar();
    void drawScore();
    void calculateMaxRadius();
    void clean();
    
    kdtree *ptree;
    kdtree *atree;
    ModelList *pmodels;
    ModelList *amodels;
    GeometryList *geometries;
    BombList *bombs;
    WallList *walls;
    ForceList *forces;
    Ship *ship;
    int frame;
    unsigned int myScore;
    float maxRadius, maxARadius;
    float worldDepth;
    bool enemieson;
    
private:
    void updateActiveModels();
    void checkActiveCollisions();
    void checkShipCollisions();
    void checkWallCollisions();
    void drawActive(float farClippingPlane);
    void drawPassive(float farClippingPlane);
    bool behindShip(Model *model);
};

#endif
