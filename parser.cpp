#include "parser.h"
#include "log.h"

Parser::Parser(std::string ff) : filename(ff) {
    level = new Level();
};

Level * Parser::parse() {
    addBasicShapes();
    std::ifstream inFile(filename.c_str(), std::ifstream::in);
    LOG("[Parser] \t Parsing " << filename << "...", 1);
    if (!inFile) { 
        LOG("[Parser] \t ERROR: Could not open given game file" << filename, 1);
        exit(1);
    }
    
    char cline[1024];
    bool result = true;
    while(inFile.good()){
        inFile.getline(cline,1023);
        std::string mline = "";
        switch (mode) {
            case MULTI:
                mline = std::string(cline);
                while(inFile.good()){
                    inFile.getline(cline,1023);
                    std::string line = std::string(cline);
                    if (line.empty() || !strcmp(line.c_str(), "}"))
                        break;
                    mline = mline + line;
                }
                result = parseLine(mline);
                mode = DEFAULT;
                break;
            default:
                result = parseLine(std::string(cline));
                break;
        }
        if (!result){
            LOG("[Parser] \t ERROR: Failed on line: " << cline ,1);
            exit (1);
        }
    }
    
    inFile.close();
	LOG("[Parser] \t Finished Parsing " << filename, 1);
    level->createTree();
    level->calculateMaxRadius();
    
    return level;
}

bool Parser::parseLine(std::string line) {
    if (line.empty()) 
        return true;
    std::stringstream ss(std::stringstream::in | std::stringstream::out); 
    ss.str(line);
    std::string oper;
    ss >> oper; 
    if (oper[0] == '#') {
        return true; 
    } else if (oper.compare("{") == 0) {
        mode = MULTI;
    } else if (oper.compare("objgeometry") == 0) {
        parseObjGeometry(line);
    } else if (oper.compare("passivemodel") == 0 ||
               oper.compare("environmentmodel") == 0 ||
               oper.compare("turret") == 0) {
        parsePassiveModel(line, oper);
    } else if (oper.compare("wall") == 0) {
        parseWall(line);
    } else if (oper.compare("gravity") == 0) {
        LOG("[Parser] \t Parsing Gravity: " << line, 1);
        float x, y, z;
        ss >> x >> y >> z;
        Force *g = new Gravity(x,y,z);
        level->forces->push_back(g);
    } else if (oper.compare("world_depth") == 0) {
        LOG("[Parser] \t World Depth: " << line, 1);
        ss >> level->worldDepth;
    } else if (oper.compare("enemies") == 0) {
        std::string swtc;
        ss >> swtc;
        if (swtc.compare("off") == 0)
            level->enemieson = false;
        else
            level->enemieson = true;
    }else if (oper.compare("ship") == 0) {
        parseShip(line);
    }
    if (ss.fail())
        return false;
    return true;
}

bool Parser::parseObjGeometry(std::string line) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out); 
    LOG("[Parser] \t Parsing line: " << line, 1);
    ss.str(line);
    std::string oper;
    ss >> oper; // get rid of first argument
    
    ObjGeometry *objg = new ObjGeometry();
    bool sphere = false;
    bool setbs = true;
    while (ss >> oper) {
        if (oper.compare("name") == 0) {
            std::string name;
            ss >> name;
            objg->name = name;
        } else if (oper.compare("path") == 0) {
            std::string filename;
            ss >> filename;
            objg->load(filename);
        } else if (oper.compare("boundingbox") == 0) {
            float x,y,z,n,m,p;
            ss >> x >> y >> z >> n >> m >> p;
            setbs = false;
            objg->calcBoundingBox(x,y,z,n,m,p);
        } else if (oper.compare("boundingsphere") == 0) {
            float x,y,z,r;
            ss >> x >> y >> z >> r;
            setbs = false;
            objg->calcBoundingSphere(x,y,z,r);
        } else if (oper.compare("bs") == 0) {
            std::string type;
            ss >> type;
            if (type.compare("sphere") == 0)
                sphere = true;
        }
    }
    if (setbs && sphere) {
        objg->calcBoundingSphere();
    } else if (setbs) {
        objg->calcBoundingBox();
    }

    objg->calcVertexTree();
    level->geometries->push_back(objg);
    return true;
}

bool Parser::parseWall(std::string line) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out); 
    LOG("[Parser] \t Parsing line: " << line, 1);
    ss.str(line);
    std::string oper;
    ss >> oper; // get rid of first argument
    
    Wall *w = NULL;
    while (ss >> oper) {
        if (oper.compare("type") == 0) {
            std::string name;
            ss >> name;
            if (!name.compare("front")) {
                w = new FrontWall();
            } else if (!name.compare("top")) {
                w = new TopWall();
            } else if (!name.compare("left")) {
                w = new LeftWall();
            } else if (!name.compare("bottom")) {
                w = new BottomWall();
            } else if (!name.compare("right")) {
                w = new RightWall();
            } else if (!name.compare("back")) {
                w = new BackWall();
            }
        } else if (oper.compare("move") == 0) {
            float d;
            ss >> d;
            w->dist = d; 
        } 
    }
    level->walls->push_back(w);
    return true;
}

void Parser::addBasicShapes() {
    Box *b = new Box();
    level->geometries->push_back(b);
    
    Plane *p = new Plane();
    level->geometries->push_back(p);
}

bool Parser::parsePassiveModel(std::string line, std::string klass = "passivemodel") {
    std::stringstream ss(std::stringstream::in | std::stringstream::out); 
    LOG("[Parser] \t Parsing line: " << line, 1);
    ss.str(line);
    std::string oper;
    ss >> oper; // get rid of first argument
    
    PassiveModel *pm;
    if (klass.compare("environmentmodel") == 0) {
        pm = new EnvironmentModel();
    } else if (klass.compare("turret") == 0) {
        pm = new Turret();
    } else {
        pm = new PassiveModel();
    }
    
    while (ss >> oper) {
        if (oper.compare("geometry") == 0) {
            std::string name;
            ss >> name;
            bool found = false;
            for (size_t i = 0; i < level->geometries->size(); i++) {
                if(name.compare(level->geometries->at(i)->name) == 0){
                    found = true;
                    pm->mesh = level->geometries->at(i);
                    break;
                }
            }
            if (!found) {
                LOG("[Parser] \t ERROR: Could not find geometry: " << name ,1);
                exit(1);
            }
        } else if (oper.compare("translate") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            pm->position = Eigen::Vector3f(x,y,z);
        } else if (oper.compare("rotate") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            pm->rotate = Eigen::Vector3f(x,y,z);
        } else if (oper.compare("scale") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            pm->scale = Eigen::Vector3f(x,y,z);
        } else if (oper.compare("color") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            pm->color = Eigen::Vector3f(x,y,z);
        }else if (oper.compare("indestructable") == 0) {
            pm->indestructable = true;
        }
    }
    pm->calculateTransform();
    level->pmodels->push_back(pm);
    return true;
}

bool Parser::parseActiveModel(std::string line) {
    return true;
}

bool Parser::parseShip(std::string line) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out); 
    LOG("[Parser] \t Parsing line: " << line, 1);
    ss.str(line);
    std::string oper;
    ss >> oper; // get rid of first argument
    Ship * ship = new Ship();
    while (ss >> oper) {
        if (oper.compare("translate") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            ship->position = Eigen::Vector3f(x,y,z);
        } else if (oper.compare("rotate") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            ship->rotate = Eigen::Vector3f(x,y,z);
        } else if (oper.compare("scale") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            ship->scale = Eigen::Vector3f(x,y,z);
        } else if (oper.compare("color") == 0) {
            float x,y,z;
            ss >> x >> y >> z;
            ship->color = Eigen::Vector3f(x,y,z);
        } else if (oper.compare("frozen") == 0) {
            ship->frozen = true;
        }
    }
    level->ship = ship;
    return true;
}
