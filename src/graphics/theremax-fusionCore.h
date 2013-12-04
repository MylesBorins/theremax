//-----------------------------------------------------------------------------
// name: theremax-fusionCore.h
// desc: entities for visualization
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __THEREMAX_FUSIONCORE_H__
#define __THEREMAX_FUSIONCORE_H__

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
    Vector3D ruleA(THEREMAXBoid * boid);
    Vector3D ruleB(THEREMAXBoid * boid);
    Vector3D ruleC(THEREMAXBoid * boid);
    // update
    // void update( YTimeInterval dt);
    // void render();
    
public:
    // alpha ramp
    Vector3D ALPHA;
};
#endif