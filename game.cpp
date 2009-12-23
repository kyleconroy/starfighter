#include "game.h"
#include "parser.h"
#include "projectile.h"
#include <math.h>
#include <string>

bool kyleMode;
bool loopBGMusic;
SystemSoundID bgMusic;

Game::Game() {
    farClippingPlane = INIT_CLIP;
	width = INIT_WIDTH;
	height = INIT_HEIGHT;
    mouseX = INIT_WIDTH / 2;
    mouseY = INIT_HEIGHT / 2;
    fps = INIT_FPS;
    isPaused = false;
    level = NULL;
	myShip = new Ship();
	currGameState = startMenu;
	currIndex = 0;
	myCrosshair = NULL;
	
	// global vars
    kyleMode = false;
    loopBGMusic = false;
    bgMusic = 0;
}

void Game::loadCurrLevel() {
    if (level) {
        delete level;
    }
    
	std::string lvlName("./levels/");
	lvlName.append(levels.at(currIndex));
	lvlName.append(".level");
	Parser gp(lvlName);
	level = gp.parse();
	
	delete myShip;
	addShiptoLevel();
	
	currGameState = inGame;
    playAudio("soundtrack");
    
    level->dump();
}

void Game::returnToStartScreen() {
	isPaused = false;
	currGameState = startMenu; 
	delete myShip;
	myShip = new Ship();
}

void Game::update() {
	if (currGameState == inGame) {
		updateShipDirection();
		myShip->update();
		level->update();
		
		if (myShip->health <= 0) {
            currGameState = endGame;
            loopBGMusic = false;
            audioCompletedForce(bgMusic);
            playAudio("lose");
		} else if (myShip->position.z() < level->worldDepth) {
		    currGameState = endGame;
            loopBGMusic = false;
            audioCompletedForce(bgMusic);
            playAudio("win");
		}
	}
}

void Game::fireMissile() {
	Missile* missle = new Missile(myShip);
	level->amodels->push_back(missle);
}

void Game::fireBomb(float seconds) {
    float power = std::min(seconds, (float)3.0)/3.0;
    Bomb* bomb = new Bomb(power, myShip);
    level->amodels->push_back(bomb);
    level->bombs->push_back(bomb);
}

void Game::explodeBombs() {
    BombList *bombs = level->bombs;
    MissileList *bombSpecks;
    Bomb *bomb;
    for (size_t i = 0; i < bombs->size(); i++) {
        bomb = bombs->at(i);
        bombSpecks = bomb->explode();
        for (size_t j = 0; j < bombSpecks->size(); j++) {
            level->amodels->push_back(bombSpecks->at(j));
        }
        delete bombSpecks;
    }
    delete level->bombs;
    level->bombs = new BombList();
}

void Game::draw() {
	switch (currGameState) {
        case startMenu: drawStartMenu(); break;
        case inGame: drawInGame(); break;
        case endGame: drawEndScreen(); break;
    }
}

void Game::drawInGame() {
    myShip->draw();
    level->draw(farClippingPlane);
    drawCrossHair();
	level->drawHealthBar();
	level->drawScore();
}

int rotateAmt = 0;
void Game::drawStartMenu() {
	// draw STARFIGHTER ascii art
	// (it looks kinda funny here because I have to escape backslashes)
	void *font = GLUT_BITMAP_9_BY_15;
	char *starfighter1 = (char *) "   _________ __              ___________.__       .__     __                 ";
	char *starfighter2 = (char *) "  /   _____//  |______ ______\\_   _____/|__| ____ |  |___/  |_  ___________  ";
	char *starfighter3 = (char *) "  \\_____  \\\\   __\\__  \\\\_  __ \\    __)  |  |/ ___\\|  |  \\   __\\/ __ \\_  __ \\ ";
	char *starfighter4 = (char *) "  /        \\|  |  / __ \\|  | \\/     \\   |  / /_/  >   Y  \\  | \\  ___/|  | \\/ ";
	char *starfighter5 = (char *) " /_______  /|__| (____  /__|  \\___  /   |__\\___  /|___|  /__|  \\___  >__|    ";
	char *starfighter6 = (char *) "         \\/           \\/          \\/      /_____/      \\/          \\/        ";
	int txtWidth = glutBitmapWidth(font, '_') * strlen(starfighter1);
	renderText(width/2 - txtWidth/2 - 30,  30, font, starfighter1, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30,  45, font, starfighter2, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30,  60, font, starfighter3, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30,  75, font, starfighter4, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30,  90, font, starfighter5, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30, 105, font, starfighter6, Color(1.0,1.0,1.0));

	// draw level list
	int LevelWidth;
	LevelWidth = glutBitmapWidth(font, 'L') * strlen("LEVELS");
	font = GLUT_BITMAP_HELVETICA_18;
	renderText(width/2 + width/8 - LevelWidth/2 - 10, 140, font, "LEVELS", Color(1.0, 1.0, 1.0));
	// now render text for each of the possible levels
	int currHeight = 170;
	for (unsigned int i = 0; i < levels.size(); i++) {
		char* levelName = (char *) levels.at(i).c_str();
		char highScore [10];
		itoa(highScores.at(i), highScore);
		
		Color textColor = Color(1.0, 1.0, 1.0);
		
		if (currIndex == i) {
			textColor.b = 0;
		}

		txtWidth = glutBitmapWidth(font, 'e') * strlen(levelName);
		renderText(width/2 + width/8 - LevelWidth/2, currHeight, font, levelName, textColor);
		currHeight += 20;
	}
	txtWidth = glutBitmapWidth(font, 'e') * strlen("Press Enter to select a level");
	renderText(width/2 + width/8 - txtWidth/2 + 20, currHeight + 10,font, "Press Enter to select a level", Color(1, 1, 1));

	// draw rotating ship
	glPushMatrix();
	rotateAmt += 5;
	if (rotateAmt == 360) {
		rotateAmt = 0;
	}
	glTranslatef(-0.18, 0.12, -0.1);
    glRotatef(rotateAmt, 0, 1, 0);
	myShip->mesh->draw();
    glPopMatrix();
	
	// draw controls info
	renderControlsInfo();

	// draw creators/project info
	char * creatorsText = (char *) "Kyle Conroy, Wei Yeh, Long Cheng, Jillian Moore";
	font = GLUT_BITMAP_HELVETICA_12;
	txtWidth = glutBitmapWidth(font, 'e') * strlen(creatorsText);
	renderText(width/2 - txtWidth/2, height-40, font, creatorsText, Color(1.0,1.0,1.0));
	char * infoText = (char *) "CS 184 Fall 2009, UC Berkeley";
	txtWidth = glutBitmapWidth(font, 'e') * strlen(infoText);
	renderText(width/2 - txtWidth/2, height-20, font, infoText, Color(1.0,1.0,1.0));
}

void Game::drawEndScreen() {
	// draw GAMEOVER/YOUWIN ascii art
	void *font = GLUT_BITMAP_9_BY_15;
	int txtWidth;
	char *endText1;
	char *endText2;
	char *endText3;
	char *endText4;
	char *endText5;
	if (myShip->health <= 0) {
		//you lose
		endText1 = (char *) "   ____    _    __  __ _____    _____     _______ ____  ";
		endText2 = (char *) "  / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\ ";
		endText3 = (char *) " | |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |";
		endText4 = (char *) " | |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ < ";
		endText5 = (char *) "  \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\";
	} else {
		// you win
		endText1 = (char *) " __   _____  _   _  __        _____ _   _   _ ";
		endText2 = (char *) " \\ \\ / / _ \\| | | | \\ \\      / /_ _| \\ | | | |";
		endText3 = (char *) "  \\ V / | | | | | |  \\ \\ /\\ / / | ||  \\| | | |";
		endText4 = (char *) "   | || |_| | |_| |   \\ V  V /  | || |\\  | |_|";
		endText5 = (char *) "   |_| \\___/ \\___/     \\_/\\_/  |___|_| \\_| (_)";
	}
	txtWidth = glutBitmapWidth(font, '_') * strlen(endText1);
	renderText(width/2 - txtWidth/2 - 30,  50, font, endText1, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30,  65, font, endText2, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30,  80, font, endText3, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30,  95, font, endText4, Color(1.0,1.0,1.0));
	renderText(width/2 - txtWidth/2 - 30, 110, font, endText5, Color(1.0,1.0,1.0));

	// draw score
	font = GLUT_BITMAP_HELVETICA_18;
	std::string scoreText = std::string("Final Score: ");
	char finalScore[10];
	itoa(level->myScore, finalScore);
	scoreText.append(finalScore);
	txtWidth = glutBitmapWidth(font, 's') * scoreText.length();
	renderText(width/2 - txtWidth/2, height - 100, font, scoreText.c_str(), Color(1, 1, 1));
	
	// draw rotating ship
	glPushMatrix();
	rotateAmt += 5;
	if (rotateAmt == 360) {
		rotateAmt = 0;
	}
	glTranslatef(0, 0, myShip->position.z());
    //glRotatef(90, 0, 0, 1);
    glRotatef(rotateAmt, 0, 1, 0);
	myShip->mesh->draw();
    glPopMatrix();

	txtWidth= glutBitmapWidth(font, 's') * strlen("Retry (r)");
	renderText(width/4 - txtWidth/2, height - 30, font, "Restart(r)", Color(1, 1, 1));

	txtWidth= glutBitmapWidth(font, 's') * strlen("Main Menu (m)");
	renderText(width -  width/4 - txtWidth/2, height - 30, font, "Main Menu(m)", Color(1, 1, 1));
}

void Game::addShiptoLevel() {
	myShip = level->ship;
	if (myCrosshair) {
        delete myCrosshair;
	}
	myCrosshair = new Crosshair();
}

void Game::drawCrossHair() {
	float mouseZSpace = myShip->position.z() - myShip->radius - 0.05;
	float mouseXSpace =  (float) mouseX/width - 0.5;
	float mouseYSpace = -((float) mouseY/height - 0.5);

	glPushMatrix();
	glTranslatef(mouseXSpace, mouseYSpace, mouseZSpace);
	myCrosshair->draw();
	glPopMatrix();
}

void Game::updateShipDirection() {
    myShip->updateDirection(mouseX, mouseY, width, height);
}

void Game::doABarrelRoll() {
    myShip->doABarrelRoll();
}

void Game::selUp() {
	if (currGameState == startMenu && currIndex > 0) {
		currIndex --;
	}
	playAudio("missile");
}

void Game::selDown(){
	if (currGameState == startMenu && currIndex < levels.size() - 1) {
		currIndex++;
	}
	playAudio("missile");
}

void Game::selLeft(){

}

void Game::selRight(){

}
