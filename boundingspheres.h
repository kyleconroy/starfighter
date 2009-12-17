#ifndef _bsphere_h_included_
#define _bsphere_h_included_

#include <vector>
#include <Eigen/Core>

class BoundingSpheres {
public:
    BoundingSpheres();
    std::vector<float> radi;
    std::vector<Eigen::Vector3f> centers;
    void add(Eigen::Vector3f,float);
};

#endif
