#ifndef _activemodel_h_included_
#define _activemodel_h_included_

#ifdef _WIN32
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define NOMINMAX
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "model.h"

class Force;

/**
 * ActiveModel - a moveable object.
 */
class ActiveModel : public Model {
public:
	ActiveModel();
	ActiveModel(Eigen::Vector3f pos);
	ActiveModel(Eigen::Vector3f pos, Eigen::Vector3f vel);
	virtual void update();
	void dump() {};
    void hit(Model *otherModel);
    void addSelf(kdtree*);
    virtual void applyForce(Force *force) {};

	float t;
	Eigen::Vector3f velocity, acceleration;
};

#endif
