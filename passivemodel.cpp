#include "passivemodel.h"
#include "boundingsphere.h"
#include "boundingbox.h"
#include "log.h"

PassiveModel::PassiveModel() : Model() {
	damage = 10;
	score = 5;
}

PassiveModel::PassiveModel(Eigen::Vector3f pos) : Model(pos) {
	damage = 10;
	score = 5;
}

void PassiveModel::update() {
}

void PassiveModel::dump() {
    if (DEBUG) {
        dumpTransM();
    }
    LOG("\tType:\tPassiveModel",1);
    if (mesh == NULL) {
        LOG("\tMesh:\tNULL",1);
    } else {
        LOG("\tMesh:\t" << mesh->name,1);
    }
    LOG("\t\tPosition:\t" << position.x() << ", " << position.y()<< ", " << position.z() ,1);
    LOG("\t\tRotation:\t" << rotate[0] << ", " << rotate[1] << ", " << rotate[2] ,1);
    LOG("\t\tScale:\t" << scale[0] << ", " << scale[1] << ", " << scale[2] ,1);
    bs->dump();
}

void PassiveModel::addSelf(kdtree *tree) {
    calculateTransform();
    
    if(mesh->boundingbox)
        bs = new BoundingBox(this);
    else
        bs = new BoundingSphere(this);
    
    bs->update(mesh);
    bs->transform(&transM, scale[0]);
    bs->addSelf(tree);
};

void PassiveModel::hit(Model *otherModel) {
	// instant kill, no health
    if (dead) {
        return;
    }
    if(!indestructable){
        die();
    }
}
