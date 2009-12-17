// Found some helpful code at http://www.gamedev.net/community/forums/topic.asp?topic_id=552068

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <string>

bool DEBUG;

float distToPlane(Eigen::Vector3f p, Eigen::Vector3f A, Eigen::Vector3f B, Eigen::Vector3f C){
    Eigen::Vector3f v = (B - A).cross(C - A);
    return v.normalized().dot(p - A);
}

bool sphereIntersectRect(Eigen::Vector3f P, float r){
    // THIS FUNCTION IS UGLY
    Eigen::Vector3f uln,lln,urn,lrn,ulf,llf,urf,lrf;
    uln = Eigen::Vector3f(-1,1,-1);
    lln = Eigen::Vector3f(-1,-1,-1);
    lrn = Eigen::Vector3f(1,-1,-1);
    urn = Eigen::Vector3f(1,1,-1);
    
    ulf = Eigen::Vector3f(-1,1,1);
    llf = Eigen::Vector3f(-1,-1,1);
    lrf = Eigen::Vector3f(1,-1,1);
    urf = Eigen::Vector3f(1,1,1);
    
    float posZ, negZ, posY, negY, posX, negX;
    
    posZ = distToPlane(P, lrf, urf, ulf);
    negZ = distToPlane(P, lrn, uln, urn);
    
    posY = distToPlane(P, urf, urn, uln);
    negY = distToPlane(P, lrf, lln, lrn);
    
    posX = distToPlane(P, lrn, urn, urf);
    negX = distToPlane(P, llf, uln, lln);
    
    if (DEBUG){
        std::cout << " posZ: " << posZ << " negZ: " << negZ << " posY: " << posY;
        std::cout << " negY: " << negY << " posX: " << posX << " negX: " << negX << std::endl;
    }
    if (posZ <= 0 && negZ <= 0 && posY <= 0 && negY <= 0 && posX <= 0 && negX <= 0){
        return true;
    }
    
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
    return false;
    
}

bool sphereIntersectSphere(Eigen::Vector3f p1, float r1, Eigen::Vector3f p2, float r2){
    return (p1 - p2).norm() <= (r1 + r2);
}

void testSR(Eigen::Vector3f P, float r, bool should){
    
    std::string pmsg, fmsg; 
    if (should) {
        pmsg = "intersect";
        fmsg = "don't intersect";
    } else {
        pmsg = "don't intersect";
        fmsg = "intersect";
    }
    
    bool collide = sphereIntersectRect(P, r);
    if (should == collide) {
        std::cout << "PASS: P = [" << P[0] << " " << P[1] << " " << P[2] << "] and R = " << r << " " << pmsg << std::endl;
    } else {
        std::cout << "FAIL: P = [" << P[0] << " " << P[1] << " " << P[2] << "] and R = " << r << " " << fmsg << std::endl;    
    }
}

void testSS(Eigen::Vector3f s1, float r1, Eigen::Vector3f s2, float r2, bool should){
    
    std::string pmsg, fmsg; 
    if (should) {
        pmsg = "intersect";
        fmsg = "don't intersect";
    } else {
        pmsg = "don't intersect";
        fmsg = "intersect";
    }
    
    bool collide = sphereIntersectSphere(s1, r1, s2, r2);
    if (should == collide) {
        std::cout << "PASS: s1 = [" << s1[0] << " " << s1[1] << " " << s1[2] << "] , r1 = " << r1;
        std::cout << " s2 = [" << s2[0] << " " << s2[1] << " " << s2[2] << "] , r2 = " << r2;
        std::cout << " " << pmsg << std::endl;
    } else {
        std::cout << "FAIL: s1 = [" << s1[0] << " " << s1[1] << " " << s1[2] << "] , r1 = " << r1;
        std::cout << " s2 = [" << s2[0] << " " << s2[1] << " " << s2[2] << "] , r2 = " << r2;
        std::cout << " " << fmsg << std::endl;   
    }
}

int test(int argc, char const *argv[])
{
    
    if (argc > 1)
        DEBUG = true;
    // Rectangle
    std::cout << "Sphere Rectangle " << std::endl;
    Eigen::Vector3f P, s1, s2;
    P = Eigen::Vector3f(0,0,0);
    testSR(P, 1, true);
    
    P = Eigen::Vector3f(2,0,0);
    testSR(P, 1, true);
    
    P = Eigen::Vector3f(2.1,0,0);
    testSR(P, 1, false);
    
    P = Eigen::Vector3f(0,-2.1,0);
    testSR(P, 1, false);
    
    P = Eigen::Vector3f(1.77,1.77,0);
    testSR(P, 1, false);
    
    P = Eigen::Vector3f(1.68,1.72,0);
    testSR(P, 1, true);
    
    P = Eigen::Vector3f(1.68,1.72,-1.08);
    testSR(P, 1, true);
    
    P = Eigen::Vector3f(1.4,1.4,0);
    testSR(P, 1, true);
    
    std::cout << "Sphere Sphere" << std::endl;
    s1 = Eigen::Vector3f(0,0,0);
    s2 = Eigen::Vector3f(0,0,0);
    testSS(s1, 1, s2, 1, true);

    s1 = Eigen::Vector3f(1,0,0);
    s2 = Eigen::Vector3f(-1,0,0);
    testSS(s1, 1, s2, 1, true);
    
    s1 = Eigen::Vector3f(1,0,0);
    s2 = Eigen::Vector3f(-1,.01,0);
    testSS(s1, 1, s2, 1, false);
    
    s1 = Eigen::Vector3f(1,0,0);
    s2 = Eigen::Vector3f(-2,0,0);
    testSS(s1, 1, s2, 2, true);
    
    s1 = Eigen::Vector3f(1,0,5);
    s2 = Eigen::Vector3f(-2,0,0);
    testSS(s1, 1, s2, 2, false);
    
    return 0;
}