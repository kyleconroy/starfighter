#ifndef _game_h_included_
#define _game_h_included_

#include "level.h"
#include "main.h"
#include "ship.h"
#include "crosshair.h"
#include <Eigen/Core>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

enum gameState {startMenu, inGame, endGame};

class Game {
public:
	Game();

	void start() {}
	void stop() {}

    /** Updates models in the level. */
 	void update();
 	void draw();
	void loadCurrLevel();
	void returnToStartScreen();
    void updateShipDirection();
    void doABarrelRoll();
	void fireMissile();
	void fireBomb(float seconds);
	void explodeBombs();
	void selUp();
	void selDown();
	void selLeft();
	void selRight();
    void addShiptoLevel();
    
	float width, height;
    unsigned int fps;
    bool isPaused;
	Eigen::Vector3f camera;
    double farClippingPlane;
	Level *level;
	unsigned int mouseX, mouseY;
	Ship *myShip;
	gameState currGameState;
	StringList levels;
	IntList highScores;
	Crosshair *myCrosshair;
	unsigned int currIndex;
	
private:
    void drawInGame();
	void drawStartMenu();
	void drawEndScreen();
    void drawCrossHair();
};

#endif
