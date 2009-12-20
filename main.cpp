#include "main.h"
#include "parser.h"
#include "log.h"
#include "game.h"
#include "ship.h"
#include "passivemodel.h"
#include "asteroid.h"
// #include "freeimage/FreeImage.h"

using namespace std;

#define RETURN 13
#define ESC 27

static void glInit(int argc, char *argv[]);
static void initScene();
static void display();
static void setUpCamera();
static void setUpLighting();
static void setUpShading();
static void reshape(int width, int height);
static void captureToImageFile();
static void keyPressed(unsigned char key, int x, int y);
static void keyPressedSpecial(int key, int x, int y);
static void mouseClicked(int button, int state, int x, int y);
static void mouseMovedPassive(int x, int y);
static void myFrameMove();
static void initLevelList(const char *);

void setOrthographicProjection();
void resetPerspectiveProjection();
void debug(const char* format, ...);

Game game;
int renderList;
static TIME lastTime, lastTimeMouse;
unsigned int numControls = 9;
string controls[] =  {"MOUSE", "MOUSE BUTTON", "HOLD & RELEASE MOUSE BUTTON", "SPACEBAR", 
						   "p", "k", "r", "q", "ESC"};
string functions[] = {"Movement", "Fire ze missiles", "Shoot ze bombs", "Detonate ze bombs", 
						   "Pause", "Kyle Sound Effects", "???", "Quit to Main Menu", "Exit"};

int main(int argc, char *argv[]) {
    LOG("====================", 1);
    LOG("[MAIN] \t START GAME", 1);
	if (argc == 2) {
		string file = argv[1];
		cout << file << endl;
		initLevelList(argv[1]);
	} else {
		LOG("No startfile specified\n", 1);
        cout << "usage: " << argv[0] << " " << "levelListFile" << endl;
		exit(1);
	}

    glInit(argc, argv);

	return 0;
}

static void glInit(int argc, char *argv[]) {
    glutInit(&argc, argv);
  
    /* tell glut to use a double-buffered window with RGB channels */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    /* size and position of the window */
    glutInitWindowSize(game.width, game.height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("StarFighter");
  	
    initScene();
    setUpLighting();
    setUpShading();
  
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMovedPassive);
    glutPassiveMotionFunc(mouseMovedPassive);
	glutSpecialFunc(keyPressedSpecial);
	glutIdleFunc(myFrameMove);
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    glutFullScreen();

    Explosion::loadAnimationFrames();
	EnvironmentModel::loadImage((char *) "textures/grid.png");
    Asteroid::loadMeshes();
	
    getCurrentTime(&lastTime);
    getCurrentTime(&lastTimeMouse);

    glutMainLoop();
}

static void initLevelList(const char * filename) {
    std::ifstream inFile(filename, std::ifstream::in);
    LOG("[Parser] \t Parsing " << filename << "...", 1);
    if (!inFile) { 
        LOG("[Parser] \t ERROR: Could not open given level list file" << filename, 1);
        exit(1);
    }
    
    char cline[1024];
    while (inFile.good()) {
        inFile.getline(cline, 1023);
        string line = std::string(cline);
		int index = line.find(' ');
		string lvlName = line.substr(0, index);
		int highScore = atoi(line.substr(index+1, line.length() - (lvlName.length() + 1)).c_str());

		game.levels.push_back(new string(lvlName));
		game.highScores.push_back(highScore);
	}
    
    inFile.close();
}

static void initScene() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    reshape(game.width, game.height);
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_FILL);
	glDisable(GL_COLOR_MATERIAL);
    
    glLoadIdentity();

    setUpCamera();
    
    glPushMatrix();
    game.draw();
	if (game.isPaused) {
		renderPausedText(); 
	}
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

static void setUpLighting() {
    GLfloat globalAmbient[] = { 0.3, 0.3, 0.0, 1.0 }; 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat light[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat lightPos0[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light);
    GLfloat lightPos1[] = { -1.0, -1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE) ;
}

static void setUpShading() {
	GLfloat matAmbient[] = { 0.2, 0.2, 0.0, 0.0 };
    GLfloat matDiffuse[] = { 1.0, 1.0, 0.0, 0.0 };
    GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat matShininess[] = { 50.0 };

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glEnable(GL_COLOR_MATERIAL);
    // set material properties which will be assigned by glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);   
}

static void setUpCamera() {    	
    Eigen::Vector3f cameraPos = game.myShip->position;
    Eigen::Vector3f rotate = game.myShip->rotate;
    cameraPos.z() += 0.3;
	if (game.currGameState == inGame) {
		rotate.y() = (90.0 - rotate.y())*0.2;
		glRotatef(rotate.y(), 0, 1, 0);
	}
    glTranslatef(0, 0, -cameraPos.z());
}

static void reshape(int width, int height) {
	game.width = width;
	game.height = height;

	glViewport(0, 0, game.width, game.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, game.width / game.height, 0.1, game.farClippingPlane);
}

static void captureToImageFile() {
    // static unsigned int picNum = 0;
    // FreeImage_Initialise();
    // 
    // FIBITMAP *bitmap = FreeImage_Allocate(game.width, game.height, 24);
    // 
    // if (!bitmap) {
    //     debug("screenshot failed!");
    //     return;
    // }
    // 
    // int width = int(game.width);
    // int height = int(game.height);
    // 
    // debug("capturing screen of size (%d, %d)", width, height);
    // GLubyte *pixels = (GLubyte *) malloc(3 * width * height);
    // glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    // int rowWidth = (3 * width);
    // int rowIndex;
    // RGBQUAD color;
    // for (int y = 0; y < height; y++) {
    //     for (int x = 0; x < width; x++) {
    //         rowIndex = (y * rowWidth) + (3 * x);
    //         color.rgbRed = pixels[rowIndex + 0];
    //         color.rgbGreen = pixels[rowIndex + 1];
    //         color.rgbBlue = pixels[rowIndex + 2];
    //         FreeImage_SetPixelColor(bitmap, x, y, &color);
    //     }
    // }
    // 
    // char imageFileName[] = "image-00.png";
    // imageFileName[7] = '0' + picNum;
    // if (FreeImage_Save(FIF_PNG, bitmap, imageFileName, 0)) {
    //     picNum++;
    //     cout << "successfully captured screenshot to " << string(imageFileName) << endl;
    // } else {
    //     cout << "failed to capture screenshot" << endl;
    // }
    // 
    // free(pixels);
    // 
    // FreeImage_DeInitialise();
}

static void keyPressed(unsigned char key, int x, int y) {
	if (game.currGameState == inGame) {
		switch (key) {
			case ' ': game.explodeBombs(); break;
			case 'p': game.isPaused = !game.isPaused; if (game.isPaused) {glutPostRedisplay();} break;
            case 'r': game.doABarrelRoll(); break;
            case 'q': loopBGMusic = false; game.returnToStartScreen(); break;
            case 'b': game.fireBomb(2); break;
		}
	}

	if (game.currGameState == startMenu) {
		switch (key) {
			case RETURN:  game.loadCurrLevel(); break;
		}
	}

	if (game.currGameState == endGame) {
		switch (key) {
			case 'r':  game.loadCurrLevel(); break;
            case 'm':  loopBGMusic = false; game.returnToStartScreen(); break;
		}
	}
	
	switch (key) {
	    case 'k': kyleMode = !kyleMode; break;
		case 'c': captureToImageFile(); break;
		case ESC: exit(0);
	}
}

static void keyPressedSpecial(int key, int x, int y) {
	if (game.currGameState == startMenu) {
		switch (key) {
            case GLUT_KEY_RIGHT:    game.selRight(); break;
            case GLUT_KEY_LEFT:     game.selLeft(); break;
            case GLUT_KEY_UP:       game.selUp(); break;
            case GLUT_KEY_DOWN:     game.selDown(); break;
        }        
	}
    
    glutPostRedisplay();
}

static void mouseClicked(int button, int state, int x, int y) {
	if (game.currGameState != inGame) {
		return;
	}
	if (state == GLUT_DOWN) {
        getCurrentTime(&lastTimeMouse);
    }
    
    if (state == GLUT_UP) {
        TIME currentTime;
        getCurrentTime(&currentTime);
        float dt = getTimeDiff(currentTime, lastTimeMouse);
        
        if (dt < 0.5) {
            game.fireMissile();
        } else {
            game.fireBomb(fabs(dt-0.5));
        }
    }
    
    mouseMovedPassive(x, y);
}

static void mouseMovedPassive(int x, int y) {
    if (x < 0 || y < 0 || x >= game.width || y >= game.height) {
        return;
    }
    game.mouseX = x;
    game.mouseY = y;
}

void setOrthographicProjection() {
    /* Courtesy lighthouse3d.com */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, game.width, 0, game.height);
    /* Invert the y axis so that down is positive. */
	glScalef(1, -1, 1);
	/* Move the origin from the bottom left corner to the upper left corner. */
	glTranslatef(0, -game.height, 0);
    glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderText(float x, float y, void *font, const char *text, Color const& color) {
    /* Courtesy lighthouse3d.com */
    setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	
	glDisable(GL_LIGHTING);
	
	glColor4f(color.r, color.g, color.b, 0.8);
	
    int x1 = x;
    for (const char *c = text; *c != '\0'; c++) {
        glRasterPos2f(x1, y);
        glutBitmapCharacter(font, *c);
        x1 += glutBitmapWidth(font, *c) + 1;
    }
    
    glEnable(GL_LIGHTING);
    
	glPopMatrix();
	resetPerspectiveProjection();
}

void renderHealthbar(float hp) {
    setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	
	glDisable(GL_LIGHTING);
	
	// background
	glColor4f(0.5, 0.5, 0.5, 0.8); // grey
    glBegin(GL_POLYGON);
		glVertex3f(10.0f,10.0f,0.0f); // top left
		glVertex3f(110.0f,10.0f,0.0f); // top right
		glVertex3f(110.0f,20.0f,0.0f); // bottom right
		glVertex3f(10.0f,20.0f,0.0f); // bottom left
	glEnd();

	// health
	if (hp > 60) {
		glColor4f(0.0, 1.0, 0.0, 0.8); // green
	} else if (hp > 30) {
		glColor4f(1.0, 1.0, 0.0, 0.8); // yellow
	} else {
		glColor4f(1.0, 0.0, 0.0, 0.8); // red
	}
	glBegin(GL_POLYGON);
		glVertex3f(10.0f,10.0f,1.0f); // top left
		glVertex3f(10.0f + hp,10.0f,1.0f); // top right
		glVertex3f(10.0f + hp,20.0f,1.0f); // bottom right
		glVertex3f(10.0f,20.0f,1.0f); // bottom left
	glEnd();
    
	glEnable(GL_LIGHTING);
    
	glPopMatrix();
	resetPerspectiveProjection();
}

void renderScore(char * score_str) {
	void *font = GLUT_BITMAP_9_BY_15;
	int score_width = glutBitmapWidth(font, score_str[0]) * strlen(score_str);
	int score_pos_x = game.width - score_width - 20;
	int score_pos_y = 20;

	renderText(score_pos_x, score_pos_y, font, score_str, Color(1.0,1.0,1.0)); 
}

void renderPausedText() {
	char *paused_str = (char *) "-- PAUSED --";
	void *font = GLUT_BITMAP_9_BY_15;
	int txt_width = glutBitmapWidth(font, paused_str[0]) * strlen(paused_str);
	renderText(game.width/2 - txt_width/2, game.height/4, font, paused_str, Color(1, 1, 1));

	renderControlsInfo();
}

void renderControlsInfo() {
	char *controls_str = (char *) "-- CONTROLS --";
	void *font = GLUT_BITMAP_9_BY_15;
	int txt_width = glutBitmapWidth(font, controls_str[0]) * strlen(controls_str);
	renderText(game.width/2 - txt_width/2, game.height/2, font, controls_str, Color(1, 1, 1));

	int currHeight = (game.height/2) + 40;
	for (unsigned int i = 0; i < numControls; i++) {
		char * control_str = (char*) controls[i].c_str();
		char * function_str = (char*) functions[i].c_str();
		Color textColor = Color(1.0, 1.0, 1.0);
		
		renderText(game.width/2 - 230, currHeight, font, control_str, textColor);
		renderText(game.width/2 + 70, currHeight, font, "--", textColor);
		renderText(game.width/2 + 110, currHeight, font, function_str, textColor);
		currHeight += 20;
	}
}

static void myFrameMove() {
	// Compute seconds elapsed since the last time the scene was redrawn
    TIME currentTime;
    getCurrentTime(&currentTime);
	float dt = getTimeDiff(currentTime, lastTime);
	if (!game.isPaused && (dt >= 1.0/game.fps)) {
		game.update();
		lastTime = currentTime;
		glutPostRedisplay(); // will call display, which will call game.draw()
	}
}
