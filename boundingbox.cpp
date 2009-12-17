#include "boundingbox.h"

BoundingBox::BoundingBox(Model* m) {
    model = m;
    uln = Eigen::Vector3f(-1,1,-1);
    lln = Eigen::Vector3f(-1,-1,-1);
    lrn = Eigen::Vector3f(1,-1,-1);
    urn = Eigen::Vector3f(1,1,-1);
    
    ulf = Eigen::Vector3f(-1,1,1);
    llf = Eigen::Vector3f(-1,-1,1);
    lrf = Eigen::Vector3f(1,-1,1);
    urf = Eigen::Vector3f(1,1,1);
    
    center = Eigen::Vector3f(0,0,0);
    radius = 1.73205081;
    
}

float BoundingBox::distToPlane(Eigen::Vector3f p, Eigen::Vector3f A, Eigen::Vector3f B, Eigen::Vector3f C){
    Eigen::Vector3f v = (B - A).cross(C - A);
    v.normalize();
    return -v.dot(p - A);
}


bool BoundingBox::intersect(BoundingSphere* s){
    //std::cout << "out" << std::endl;
    
    Eigen::Vector3f P;
    float posZ, negZ, posY, negY, posX, negX,r;
    
    P = s->center;
    r = s->radius;
    
    // std::cout << P << std::endl;
    // std::cout << r << std::endl;

    posZ = distToPlane(P, lrf, urf, ulf);
    negZ = distToPlane(P, lrn, uln, urn);

    posY = distToPlane(P, urf, urn, uln);
    negY = distToPlane(P, lrf, lln, lrn);

    posX = distToPlane(P, lrn, urn, urf);
    negX = distToPlane(P, llf, uln, lln);
    
    if (posZ <= 0 && negZ <= 0 && posY <= 0 && negY <= 0 && posX <= 0 && negX <= 0){
        return true;
    }
    
    // std::cout << "out1" << std::endl;
    // std::cout << posZ - r << " " << negZ - r << " " << posY - r << " " << negY - r << " " << posX - r << " " << negX - r << std::endl;
    if (posZ - r > 0 || negZ - r > 0 || posY - r > 0 || negY - r > 0 || posX - r > 0 || negX - r > 0){
        return false;
    }


    //THIS SECTION IS HORRIBLE
    float dx, dy, dz, r2;
    r2 = r * r;
    int c = 0;

    dx = -1;
    dy = -1;
    dz = -1;

    if (posX > 0 || negX > 0){
        c++;
        if (posX > 0)
            dx = posX;
        else
            dx = negX;
    }

    if (posY > 0 || negY > 0){
        c++;
        if (posY > 0)
            dy = posY;
        else
            dy = negY;
    }

    if (posZ > 0 || negZ > 0){
        c++;
        if (posZ > 0)
            dz = posZ;
        else
            dz = negZ;
    }

    //std::cout << "out2" << std::endl;

    if (c == 1)
        return true;

    float dx2, dy2, dz2;
    dx2 = dx * dx;
    dy2 = dy * dy;
    dz2 = dz * dz;

    if (dx > 0 && dy > 0 && dz > 0)
        return (dx2 + dy2 + dz2) < r2;
    if (dx > 0 && dy > 0)
        return (dx2 + dy2) < r2;     
    if (dz > 0 && dy > 0)
        return (dy2 + dz2) < r2;
    if (dx > 0 && dz > 0)
        return (dx2 + dz2) < r2;
    
    //std::cout << "out3" << std::endl;    
    
    return false;

};

void BoundingBox::transform(Eigen::Matrix4f *t, float r) {
    radius = radius * r;
    transformVector(t,&uln);
    transformVector(t,&lln);
    transformVector(t,&lrn);
    transformVector(t,&urn);
    transformVector(t,&ulf);
    transformVector(t,&llf);
    transformVector(t,&lrf);
    transformVector(t,&urf);
    transformVector(t,&center); 
    
    // std::cout << uln[0] << " " << uln[1] << " " << uln[2] << std::endl;
    // std::cout << lln[0] << " " << lln[1] << " " << lln[2] << std::endl;
    // std::cout << lrn[0] << " " << lrn[1] << " " << lrn[2] << std::endl;
    // std::cout << urn[0] << " " << urn[1] << " " << urn[2] << std::endl;
    // 
    // std::cout << ulf[0] << " " <<  ulf[1] << " " << ulf[2] << std::endl;
    // std::cout << llf[0] << " " <<  llf[1] << " " << llf[2] << std::endl;
    // std::cout << lrf[0] << " " <<  lrf[1] << " " << lrf[2] << std::endl;
    // std::cout << urf[0] << " " <<  urf[1] << " " << urf[2] << std::endl;
};

void BoundingBox::transformVector(Eigen::Matrix4f *t, Eigen::Vector3f *v) {
    Eigen::Matrix4f tm = *t;
    Eigen::Vector4f newc(v->x(), v->y(), v->z(), 1);
    newc = tm * newc;
    *v = Eigen::Vector3f(newc.x(),newc.y(),newc.z());
};

void BoundingBox::addSelf(kdtree *tree){
    kd_insert3f(tree,center.x(), center.y(), center.z(),this);
    // std::cout << " [" << center.x() << " " << center.y() << " " << center.z() << "] " << std::endl;
};

void BoundingBox::update(Geometry *mesh){
    Eigen::Vector3f bmax = mesh->bmax;
    Eigen::Vector3f bmin = mesh->bmin;
    
    // std::cout << bmin[0] << " " <<  bmin[1] << " " << bmin[2] << std::endl;
    // std::cout << bmax[0] << " " <<  bmax[1] << " " << bmax[2] << std::endl;
    // std::cout << "" << std::endl;
    
    radius = mesh->radius;
    center = mesh->center;
    
    uln[2] = bmax[2];
    lln[2] = bmax[2];
    lrn[2] = bmax[2];
    urn[2] = bmax[2];
    
    ulf[2] = bmin[2];
    llf[2] = bmin[2];
    lrf[2] = bmin[2];
    urf[2] = bmin[2];
    
    uln[0] = bmin[0];
    lln[0] = bmin[0];
    ulf[0] = bmin[0];
    llf[0] = bmin[0];
    
    urn[0] = bmax[0];
    lrn[0] = bmax[0];
    urf[0] = bmax[0];
    lrf[0] = bmax[0];
    
    uln[1] = bmax[1];
    ulf[1] = bmax[1];
    urf[1] = bmax[1];
    urn[1] = bmax[1];
    
    lln[1] = bmin[1];
    llf[1] = bmin[1];
    lrf[1] = bmin[1];
    lrn[1] = bmin[1];
    

};

void BoundingBox::bounce(Eigen::Vector3f* pi, Eigen::Vector3f* ve, float r){
    Eigen::Vector3f P = *pi;
    PlaneCollision posZ, negZ, posY, negY, posX, negX;
    float d = r + 1;
    int j = 0;
    
    std::vector<PlaneCollision *> pl;
    
    pl.push_back(new PlaneCollision(P, lrf, urf, ulf));
    pl.push_back(new PlaneCollision(P, lrn, uln, urn));
    pl.push_back(new PlaneCollision(P, urf, urn, uln));
    pl.push_back(new PlaneCollision(P, lrf, lln, lrn));
    pl.push_back(new PlaneCollision(P, lrn, urn, urf));
    pl.push_back(new PlaneCollision(P, llf, uln, lln));
    
    for(size_t i = 0; i < pl.size(); ++i)
    {
        if(fabs(pl[i]->d) < d){
            d = fabs(pl[i]->d);
            j = i;
        }
    }
    
    PlaneCollision * pc = pl.at(j);
    Eigen::Vector3f normal = pc->normal();
    
    if(pc->d > 0){
        *pi = *pi + (r - pc->d + (.1 * r)) * normal;
    }else{
        *pi = *pi + (r + pc->d + (.1 * r)) * normal;
    }
    
    *ve = (*ve - 2 * ve->dot(normal) * normal);
    
    for(size_t i = 0; i < pl.size(); ++i)
    {
        delete pl[i];/* code */
    }
    pl.clear();

    //std::cout << d << std::endl;
    
};

void BoundingBox::dump(){
    LOG("\t\tBoundingShape:\t Box",1);
    LOG("\t\tLower Left Far:\t" << llf.x() << ", " << llf.y()<< ", " << llf.z() ,1);
    LOG("\t\tUpper Right Near:\t" << urn.x() << ", " << urn.y()<< ", " << urn.z() ,1);
    BoundingShape::dump();
};
