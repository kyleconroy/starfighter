#ifndef _wall_h_included_
#define _wall_h_included_

#include <Eigen/Core>

class Wall {
public:
    float dist;
    Wall();
    virtual ~Wall() {}
    Wall(float i){
        dist = i;
    };
    virtual void dump() =0;
    virtual bool collide(Eigen::Vector3f, Eigen::Vector3f) =0;
};

class BackWall : public Wall {
public:
    BackWall();
    BackWall(float);
    void dump();
    bool collide(Eigen::Vector3f, Eigen::Vector3f);
};

class TopWall : public Wall {
public:
    TopWall();
    TopWall(float);
    void dump();
    bool collide(Eigen::Vector3f, Eigen::Vector3f);
};

class LeftWall : public Wall {
public:
    LeftWall ();
    LeftWall (float);
    void dump();
    bool collide(Eigen::Vector3f, Eigen::Vector3f);
};

class BottomWall : public Wall {
public:
    BottomWall ();
    BottomWall (float);
    void dump();
    bool collide(Eigen::Vector3f, Eigen::Vector3f);
};

class RightWall : public Wall {
public:
    RightWall ();
    RightWall (float);
    void dump();
    bool collide(Eigen::Vector3f, Eigen::Vector3f);
};

class FrontWall : public Wall {
public:
    FrontWall();
    FrontWall(float);
    void dump();
    bool collide(Eigen::Vector3f, Eigen::Vector3f);
};

#endif
