#ifndef _geometery_h_included_
#define _geometery_h_included_

#include <Eigen/Core>
#include <string>
#include <vector>

class Geometry {
public:
    virtual ~Geometry() {};
    virtual void draw() =0;
    virtual void dump() =0;
    virtual void update() =0;
    Eigen::Vector3f bmin;
    Eigen::Vector3f bmax;
    Eigen::Vector3f center;
    float radius;
    std::string name;
    bool dead;
    int displayList;
    bool boundingbox;
    
    bool intersect(Eigen::Vector3f, float){
        return true;
    };

protected:
    Geometry() {
        name = "geometry";
        bmin = Eigen::Vector3f(0,0,0);
        bmax = Eigen::Vector3f(0,0,0);
        center = Eigen::Vector3f(0,0,0);
        radius = 0;
        dead = false;
        displayList = -1;
        boundingbox = true;
    };
    
    Geometry(std::string nm) {
        name = nm;
        bmin = Eigen::Vector3f(0,0,0);
        bmax = Eigen::Vector3f(0,0,0);
        center = Eigen::Vector3f(0,0,0);
        radius = 0;
        dead = false;
        displayList = -1;
        boundingbox = true;
    };
};

#endif
