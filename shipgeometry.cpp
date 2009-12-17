#include "shipgeometry.h"
#include "model.h"
#include "log.h"

#include <Eigen/Geometry>

#include <fstream>
#include <sstream>
#include <float.h>

ShipGeometry::ShipGeometry() : ObjGeometry() {
};

ShipGeometry::ShipGeometry(std::string path) : ObjGeometry(path) {
};

void ShipGeometry::draw() {
    if (displayList != -1) {
        glCallList(displayList);
        return;
    }
    
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
    
	unsigned int division_size = faces.size()/12;
	float red = 1.0f;
	float green = 1.0f;
	float blue = 1.0f;
	float mcolor[] = { red, green, blue, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);

	for (unsigned int i = 0; i < faces.size(); i++) {
		if (i < division_size*10) { 
			red = ((float)(i % division_size/2))/division_size;
			green = ((float)(i % division_size/2))/division_size;
			blue = ((float)(i % division_size/2))/division_size + 0.1;
		} else {
			red = ((float) i)/division_size;	// glowy
			green = ((float) i)/division_size;	// glowy
			blue = ((float) i)/division_size;	// glowy
		}
		mcolor[0] = red;
		mcolor[1] = green;
		mcolor[2] = blue;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);

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
	
	glEndList();
}

void ShipGeometry::dump() {
    LOG("\tType:\tShipGeometry",1);
    LOG("\t\tPath:\t" << objpath ,1);
    LOG("\t\tFaces:\t" << faces.size(),1);
    LOG("\t\tVerts:\t" << vertices.size(),1);
    LOG("\t\tRadius:\t" << radius, 1);
    LOG("\t\tCenter:\t [" << center.x() << " " << center.y() << " " << center.z() << "]",1);
    LOG("\t\tMin:\t [" << bmin.x() << " " << bmin.y() << " " << bmin.z() << "]",1);
    LOG("\t\tMax:\t [" << bmax.x() << " " << bmax.y() << " " << bmax.z() << "]",1);
}
