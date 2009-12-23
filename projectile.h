#ifndef _projectile_h_included_
#define _projectile_h_included_

#include "activemodel.h"
#include "objgeometry.h"
/**
 * A projectile launched from some source.
 */
class Projectile : public ActiveModel {    
public:
    Projectile(Model *src);
    Projectile(Eigen::Vector3f pos, Eigen::Vector3f vel);
    ~Projectile();
private:
    Model *source;
};

class Missile : public Projectile {
public:
    Missile(Model *src);
    Missile(Eigen::Vector3f pos, Eigen::Vector3f vel);
    void init();
    void die();
    static void loadMeshes();
private:
    static ObjGeometry *missileMesh;
};

class Bomb : public Projectile {
public:
    Bomb(float p, Model *src);
    void applyForce(Force *force);
    /** Detonate this bomb by returning the result of
     *  fracturing it into many missile pieces. */
    void hit(Model *model);
    static void loadMeshes();
    MissileList *explode();
private:
    static ObjGeometry *bombMesh;
};

#endif
