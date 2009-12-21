#ifndef _model_h_included_
#define _model_h_included_

#ifdef _WIN32
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define NOMINMAX
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "main.h"
#include "boundingshape.h"
#include "boundingsphere.h"
#include "teapot.h"
#include "explosion.h"
#include <Eigen/Core>
#include <Eigen/LU>
#include "kdtree/kdtree.h"

#define PI 3.14159265

class Model {
public:
    virtual ~Model() {}
    
    virtual void draw() {
        if (!mesh || mesh->dead) return;
    
    	glPushMatrix();
    
        glTranslatef(position.x(), position.y(), position.z());
        glRotatef(rotate.x(), 1, 0, 0);
        glRotatef(rotate.y(), 0, 1, 0);
        glRotatef(rotate.z(), 0, 0, 1);
        glScalef(scale.x(), scale.y(), scale.z());
    
        float mcolor[] = { color.x(), color.y(), color.z(), 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	
        mesh->draw();

    	glPopMatrix();
    }
    
    virtual void dump() =0;
    virtual void update() =0;
    virtual void hit(Model *otherModel) =0;
    virtual void addSelf(kdtree*) =0;
    virtual bool collision(Model *otherModel) {
        return false;
    }
    
    void calculateTransform() {
        transM = Eigen::Matrix4f::Identity();
        scaleM(scale.x(), scale.y(), scale.z());
        rotateM(rotate.x(), rotate.y(), rotate.z());
        translateM(position.x(), position.y(), position.z());
    }
    
    virtual void die() {
        if (dead) {
            return;
        }
		color = Eigen::Vector3f(1.0,1.0,1.0);
		scale = Eigen::Vector3f(1.0,1.0,1.0);
		rotate = Eigen::Vector3f(0.0,0.0,0.0);
		//mesh = new Explosion();
        mesh = NULL;
        dead = true;
	}

	// ensures we only accumulate the score once
	int getScore() {
		int oldScore = score;
		score = 0;
		return oldScore;
	}

	
    Eigen::Matrix4f transM;
    Geometry* mesh;
    BoundingShape* bs;
    Eigen::Vector3f position;
    Eigen::Vector3f scale;
    Eigen::Vector3f rotate;
    Eigen::Vector3f color;
	int health, score, damage;
    float radius;
    bool dead, indestructable;
    bool shipPart, isAsteroid;

protected:
    Model() {
        init();
    };
    
    Model(Eigen::Vector3f pos) {
        init();
        position = pos;
    };
    
    void translateM(float x, float y, float z) {
        Eigen::Matrix4f sc = Eigen::Matrix4f::Identity();
        sc(0,3) = x;
        sc(1,3) = y;
        sc(2,3) = z;
        transM = sc * transM;
    }
    
    void scaleM(float x, float y, float z) {
        Eigen::Matrix4f sc = Eigen::Matrix4f::Identity();
        sc(0,0) = x;
        sc(1,1) = y;
        sc(2,2) = z;
        transM = sc * transM;
    }
    
    void rotateM(float x, float y, float z) {
        Eigen::Matrix4f rx,rz,ry;
        float radx = x*PI/180;
        rx = rotatex(radx);
        ry = rotatey(y*PI/180);
        rz = rotatez(z*PI/180);
        transM = rx * ry * rz * transM;
    }
    
    Eigen::Matrix4f rotatez(float rad) {
        Eigen::Matrix4f rv = Eigen::Matrix4f::Zero();
        rv(0,0) = cos(rad);
        rv(0,1) = -sin(rad);
        rv(1,0) = sin(rad);
        rv(1,1) = cos(rad);
        rv(2,2) = 1;
        rv(3,3) = 1;
        return rv;
    }
    
    Eigen::Matrix4f rotatex(float rad) {
        Eigen::Matrix4f rv = Eigen::Matrix4f::Zero();
        rv(1,1) = cos(rad);
        rv(1,2) = -sin(rad);
        rv(2,1) = sin(rad);
        rv(2,2) = cos(rad);
        rv(0,0) = 1;
        rv(3,3) = 1;
        return rv;
    }
    
    Eigen::Matrix4f rotatey(float rad) {
        Eigen::Matrix4f rv = Eigen::Matrix4f::Zero();
        rv(0,0) = cos(rad);
        rv(2,0) = -sin(rad);
        rv(0,2) = sin(rad);
        rv(2,2) = cos(rad);
        rv(1,1) = 1;
        rv(3,3) = 1;
        return rv;
    }
    
    void dumpTransM() {
        int depth = 2;
        std::string tab = "";
        for (int j = 0; j < depth; j++)
            tab += "\t";
        std::cout << tab << "Transformation Matrix" << std::endl;
        tab += "\t";
        for(int j = 0; j < 4; j++){
            std::cout << tab;
            for(int i = 0; i < 4; i++){
                std::cout << transM(j, i) << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    void init() {
        position = Eigen::Vector3f(0, 0, 0);
        rotate = Eigen::Vector3f(0, 0, 0);
        scale = Eigen::Vector3f(1, 1, 1);
        transM = Eigen::Matrix4f::Identity();
        mesh = NULL;
        color = Eigen::Vector3f(1.0, 1.0, 1.0);
        health = 1;
        damage = 0;
        score = 0;
        dead = false;
        bs = new BoundingSphere(this);
        shipPart = false;
        isAsteroid = false;
        indestructable = false;
    }

    void bsInit() {
    }
};

#endif
