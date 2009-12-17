#ifndef _parser_h_included_
#define _parser_h_included_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "model.h"
#include "geometry.h"
#include "level.h"
#include "objgeometry.h"
#include "box.h"
#include "plane.h"
#include "wall.h"
#include "force.h"
#include "envmodel.h"
#include "turret.h"

class Parser {
public:
    enum Mode {
        DEFAULT,
        MULTI
    };
    
    Parser(std::string);
    std::string filename;
    Mode mode;
    Level *level;
    Level *parse();
    bool parseLine(std::string);
private:
    bool parseBox(std::string);
    bool parseShip(std::string);
    bool parseActiveModel(std::string);
    bool parsePassiveModel(std::string, std::string klass);
    bool parseObjGeometry(std::string);
    bool parseWall(std::string);
    void addBasicShapes();
};

#endif
