//-----------------------------------------------------------------------------
// name: theremax-fusionCore.h
// desc: entities for visualization
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------

#include "theremax-fusionCore.h"
#include "theremax-globals.h"
#include "x-fun.h"

THEREMAXBoid::THEREMAXBoid()
{
    ALPHA.set(1,1,1);
    this->spark = new THEREMAXSpark;
    spark->set(0, 0.1, 0.5);
    
    this->addChild(spark);
}

void THEREMAXBoid::update( YTimeInterval dt )
{
    Vector3D v1, v2, v3;

    // slew
    ALPHA.interp( dt );
    this->alpha = ALPHA.value;
    
    v1 = ((THEREMAXFlock *)parent)->ruleA(this, dt);
    v2 = ((THEREMAXFlock *)parent)->ruleB(this, dt);
    v3 = ((THEREMAXFlock *)parent)->ruleC(this, dt);
    // 
    this->vel = this->vel + v1 + v2 + v3;
    this->loc = this->loc + this->vel;
    
    return;
};


Vector3D THEREMAXFlock::ruleA(THEREMAXBoid * boid, YTimeInterval dt)
{
    Vector3D value(0,1,2);
    return value;
};

Vector3D THEREMAXFlock::ruleB(THEREMAXBoid * boid, YTimeInterval dt)
{
    Vector3D value(0,1,2);
    return value;
};

Vector3D THEREMAXFlock::ruleC(THEREMAXBoid * boid, YTimeInterval dt)
{
    Vector3D value(0,1,2);
    return value;
};

void THEREMAXFlock::init(int count)
{
    for (int i = 0; i < count; i++)
    {
        THEREMAXBoid * boid = new THEREMAXBoid;
        boid->loc.setXYFromPolar( 0.5, 360 * i / count);
        // boid->loc.set(i*0.5,i*0.5,i*0.5);
        this->addChild(boid);
    }
}