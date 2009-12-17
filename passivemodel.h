#ifndef _passivemodel_h_included_
#define _passivemodel_h_included_

#include "model.h"

/**
 * PassiveModel - an immoveable object.
 */
class PassiveModel : public Model {
public:
    PassiveModel();
    PassiveModel(Eigen::Vector3f);
    void update();
    void dump();
    void hit(Model *otherModel);
    void addSelf(kdtree*);
};

#endif
