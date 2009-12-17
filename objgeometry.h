#ifndef _objgeometry_h_included_
#define _objgeometry_h_included_

#include "geometry.h"
#include <vector>
#include <string>
#include <Eigen/Core>
#include "kdtree/kdtree.h"


class ObjGeometry : public Geometry {
public:
    ObjGeometry();
    ObjGeometry(std::string);
    void dump();
    void draw();
    void update();
    void load(std::string);
    bool parseLine(std::string);
    void calcVertexTree();
    void calcBoundingSphere();
    void calcBoundingBox();
    void calcBoundingSphere(float,float,float,float);
    void calcBoundingBox(float,float,float,float,float,float);
    bool intersect(Eigen::Vector3f, float);
    
    std::string objpath;
    kdtree * vtree;
    
    std::vector<Eigen::Vector3f> vertices;
    std::vector< std::vector<int> > vertfaces;
    std::vector<Eigen::Vector3f> verticesn;
    std::vector<Eigen::Vector3f> faces;
    std::vector<Eigen::Vector3f> facesn;

private:
    void addVertex(float, float, float);
    void addFace(int, int, int);
    void calcVertexNormals();
};

#endif
