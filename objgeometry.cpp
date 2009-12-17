#include "objgeometry.h"
#include "model.h"
#include "log.h"

#include <Eigen/Geometry>

#include <fstream>
#include <sstream>
#include <float.h>

ObjGeometry::ObjGeometry() : Geometry("objmodel") {
    vtree = kd_create(3);
};

ObjGeometry::ObjGeometry(std::string path) : Geometry("objmodel") {
    objpath = path;
    vtree = kd_create(3);
    load(path);
    update();
};

void ObjGeometry::draw() {
	for (unsigned int i = 0; i < faces.size(); i++) {
		glBegin(GL_POLYGON);
		Eigen::Vector3f x1 = vertices[faces[i][0]];
		Eigen::Vector3f x2 = vertices[faces[i][1]];
		Eigen::Vector3f x3 = vertices[faces[i][2]];
        Eigen::Vector3f n1 = verticesn[faces[i][0]];
        Eigen::Vector3f n2 = verticesn[faces[i][1]];
        Eigen::Vector3f n3 = verticesn[faces[i][2]];
        
		//Eigen::Vector3f n = facesn[i];
        glNormal3f(n1[0], n1[1], n1[2]);
        glVertex3f(x1[0], x1[1], x1[2]);
        glNormal3f(n2[0], n2[1], n2[2]);
        glVertex3f(x2[0], x2[1], x2[2]);
        glNormal3f(n3[0], n3[1], n3[2]);
        glVertex3f(x3[0], x3[1], x3[2]);
		glEnd();
	}
}

void ObjGeometry::update() {
}

bool ObjGeometry::intersect(Eigen::Vector3f v, float r){
    struct kdres *presults;
    presults = kd_nearest_range3f(vtree, v.x(), v.y(), v.z(), r);
    int results = kd_res_size(presults);
    kd_res_free(presults);
    return results > 0;
}

void ObjGeometry::calcVertexTree() {
     for (size_t i = 0; i < vertices.size(); i++) {
         Eigen::Vector3f v = vertices.at(i);
         kd_insert3f(vtree,v.x(), v.y(), v.z(), 0);
     }
}

void ObjGeometry::calcBoundingBox() {
    boundingbox = true;
     for (size_t i = 0; i < vertices.size(); i++) {
         bmax[0] = max(vertices[i][0], bmax[0]);
         bmax[1] = max(vertices[i][1], bmax[1]);
         bmax[2] = max(vertices[i][2], bmax[2]);
         bmin[0] = min(vertices[i][0], bmin[0]);
         bmin[1] = min(vertices[i][1], bmin[1]);
         bmin[2] = min(vertices[i][2], bmin[2]);
     }
     center = (bmax + bmin) / 2;
     radius = (bmax - center).norm();
}

void ObjGeometry::calcBoundingSphere() {
    boundingbox = false;
    float maxr = 0;
     for (size_t i = 0; i < vertices.size(); i++) {
         bmax[0] = max(vertices[i][0], bmax[0]);
         bmax[1] = max(vertices[i][1], bmax[1]);
         bmax[2] = max(vertices[i][2], bmax[2]);
         bmin[0] = min(vertices[i][0], bmin[0]);
         bmin[1] = min(vertices[i][1], bmin[1]);
         bmin[2] = min(vertices[i][2], bmin[2]);
     }
     center = (bmax + bmin) / 2;
     
     for (size_t i = 0; i < vertices.size(); i++) {
         maxr = max((vertices[i] - center).norm(), maxr);
     }
     
     radius = maxr;
}

void ObjGeometry::calcBoundingBox(float x1, float y1, float z1, float x2, float y2, float z2) {
    boundingbox = true;
    bmin = Eigen::Vector3f(x1,y1,z1);
    bmax = Eigen::Vector3f(x2,y2,z2);
    center = (bmax + bmin) / 2;
    radius = (bmax - center).norm();
}

void ObjGeometry::calcBoundingSphere(float x, float y, float z, float r) {
    boundingbox = false;
    bmax = Eigen::Vector3f(-r,-r,-r);
    bmax = Eigen::Vector3f(r,r,r);
    center = Eigen::Vector3f(x,y,z);
    radius = r;
}

void ObjGeometry::dump() {
    LOG("\tType:\tObjGeometry",1);
    LOG("\t\tPath:\t" << objpath ,1);
    LOG("\t\tFaces:\t" << faces.size(),1);
    LOG("\t\tVerts:\t" << vertices.size(),1);
    LOG("\t\tRadius:\t" << radius, 1);
    LOG("\t\tCenter:\t [" << center.x() << " " << center.y() << " " << center.z() << "]",1);
    LOG("\t\tMin:\t [" << bmin.x() << " " << bmin.y() << " " << bmin.z() << "]",1);
    LOG("\t\tMax:\t [" << bmax.x() << " " << bmax.y() << " " << bmax.z() << "]",1);
}

void ObjGeometry::load(std::string filename) {
    objpath = filename;
    debug("loaded %s", objpath.c_str());
    char line[1024];
    std::ifstream inFile(filename.c_str(), std::ifstream::in);
    if (!inFile) { 
        LOG("[ObjGeometery] \t ERROR: Could not open object file" << filename, 1); exit (1);
    } 
    while (inFile.good()) {
        inFile.getline(line, 1023);
        if (!parseLine(std::string(line))) {
            LOG("[ObjGeometery] \t ERROR: Failed on object line: " << line, 1);
            exit(1); 
        }
    }
    calcVertexNormals();
    inFile.close();
}

bool ObjGeometry::parseLine(std::string line) { 
    
	std::string oper;
	if (line.empty()) 
		return true;
	std::stringstream ss(std::stringstream::in | std::stringstream::out); 
	ss.str(line); 
	ss >> oper; 
	if (oper[0] == '#') {
		return true; 
	} else if (oper.compare("v") == 0) {
		float x,y,z;
		ss >> x >> y >> z;
        addVertex(x,y,z);
	} else if (oper.compare("f") == 0) {
		int i,j,k,m;
		ss >> i >> j >> k;
        addFace(i-1,j-1,k-1);
        ss >> m;
        if (ss.fail()) {
            return true;
        }else {
           addFace(i-1,k-1,m-1);
        }
	}
	if (ss.fail()) {
	    return false;
	}
	return true;
}

void ObjGeometry::addVertex(float x, float y, float z) {
    std::vector<int> m;
    Eigen::Vector3f vert(x, y, z);
    vertices.push_back(vert);
    vertfaces.push_back(m);
};

void ObjGeometry::addFace(int v1, int v2, int v3) {
    Eigen::Vector3f x1 = vertices[v1];
    Eigen::Vector3f x2 = vertices[v2];
    Eigen::Vector3f x3 = vertices[v3];
    Eigen::Vector3f n = (x1 - x2).cross(x1 - x3).normalized();
    int j = faces.size();
    facesn.push_back(n);
    vertfaces[v1].push_back(j);
    vertfaces[v2].push_back(j);
    vertfaces[v3].push_back(j);
    faces.push_back(Eigen::Vector3f(v1, v2, v3));
}

void ObjGeometry::calcVertexNormals() {
    for (size_t i = 0; i < vertices.size(); i++) {
        Eigen::Vector3f n(0,0,0);
        for (size_t j = 0; j < vertfaces[i].size(); j++) {
            n += facesn[vertfaces[i][j]];
        }
        n.normalize();
        verticesn.push_back(n);
    }
    vertfaces.clear();
}
