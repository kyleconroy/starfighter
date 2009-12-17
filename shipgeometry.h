#ifndef _shipgeometry_h_included_
#define _shipgeometry_h_included_

#include "objgeometry.h"
#include <vector>
#include <string>
#include <Eigen/Core>


class ShipGeometry : public ObjGeometry {
public:
    ShipGeometry();
    ShipGeometry(std::string);
    void dump();
    void draw();
};

#endif
