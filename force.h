#ifndef _force_h_included_
#define _force_h_included_

#define METER 0.00168

#include <Eigen/Core>

class ActiveModel;

class Force {
public:
    Force() {};
    virtual ~Force() {};
    virtual void affect(ActiveModel *model) =0;
};

class Gravity : public Force {
public:
    Gravity();
    Gravity(float, float, float);
    void affect(ActiveModel *model);
    
    Eigen::Vector3f accel;
};

#endif
