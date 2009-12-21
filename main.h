#ifndef _main_h_included_
#define _main_h_included_

#include <vector>
#include <iostream>
#include <cmath>
#include "model.h"
#include "geometry.h"
#include "wall.h"
#include "force.h"
#ifdef OSX
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#endif

class Model;
class Geometry;
class Color;
class Level;
class Ship;
class PassiveModel;
class Enemy;
class Force;
class Missile;
class Bomb;
class Parser;

typedef std::vector<Model *> ModelList;
typedef std::vector<Geometry *> GeometryList;
typedef std::vector<Wall *> WallList;
typedef std::vector<Force *> ForceList;
typedef std::vector<Enemy *> EnemeyList;
typedef std::vector<Missile *> MissileList;
typedef std::vector<Bomb *> BombList;
typedef std::vector<std::string *> StringList;
typedef std::vector<int> IntList;

enum Direction {
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

#define INIT_WIDTH  800
#define INIT_HEIGHT 600
#define INIT_FPS 30
#define INIT_CLIP 20.0
#define PI 3.14159265
#define DEGREES_TO_RADIANS(deg) (deg * PI/180.0)
#define RADIANS_TO_DEGREES(rad) (rad * 180.0/PI)
#define DEBUG true
#ifdef _WIN32
    #define TIME DWORD
#else
    #define TIME timeval
#endif
#ifndef OSX
    #define SystemSoundID int
#endif

extern void renderText(float x, float y, void *font, const char *text, Color const& rgb);
extern char *itoa(int i, char *buf);
extern void renderHealthbar(float hp);
extern void renderScore(char * score_str);
extern void renderPausedText();
extern void renderControlsInfo();
extern void setOrthographicProjection();
extern void resetPerspectiveProjection();
extern void debug(const char* format, ...);
extern void getCurrentTime(TIME *currentTime);
extern float getTimeDiff(TIME currentTime, TIME lastTime);
extern bool fequals(float actual, float expected);
extern void playAudio(const char *file);
extern void audioCompleted(SystemSoundID ssid, void *urlRef);
extern void audioCompletedForce(SystemSoundID ssid);

class Color {
public:
    Color(float red, float green, float blue) {
        r = red;
        g = green;
        b = blue;
    }
    
    float r, g, b;
};

/* Global variables for "Kyle mode" and background music. */
extern bool kyleMode;
extern bool loopBGMusic;
extern SystemSoundID bgMusic;

#endif
