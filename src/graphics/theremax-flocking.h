//-----------------------------------------------------------------------------
// name: theremax-flocking.h
// desc: entities for visualization
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __THEREMAX_FLOCKING_H__
#define __THEREMAX_FLOCKING_H__

#include "y-entity.h"
#include "theremax-entity.h"

using namespace std;

//-----------------------------------------------------------------------------
// name: class boid
// desc: ...
//-----------------------------------------------------------------------------
class THEREMAXBoid : public YEntity
{
public:
    // constructor
    THEREMAXBoid();
    
public:
    //set
    void set();
    
public:
    // update
    void update( YTimeInterval dt);
    // void render();
    
public:
    // alpha ramp
    THEREMAXSpark * spark;
    Vector3D ALPHA;
};

//-----------------------------------------------------------------------------
// name: class flock
// desc: ...
//-----------------------------------------------------------------------------
class THEREMAXFlock : public YEntity
{
public:
    // constructor
    THEREMAXFlock() {} ;
    
public:
    //set
    void set();
    void init(int count);
    
public:
    Vector3D centerMass(THEREMAXBoid * boid);
    Vector3D collisionDetect(THEREMAXBoid * boid);
    Vector3D potentialVelocity(THEREMAXBoid * boid);
    Vector3D tendToPlace(THEREMAXBoid * boid);
    Vector3D boundPosition(THEREMAXBoid * boid);
    void boundVelocity(THEREMAXBoid * boid);
    // update
    // void update( YTimeInterval dt);
    // void render();
    
public:
    // alpha ramp
    Vector3D ALPHA;
};
#endif