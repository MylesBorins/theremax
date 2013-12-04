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
    Vector3D v1(0,0,0);
    Vector3D v2(0,0,0);
    Vector3D v3(0,0,0);

    // slew
    ALPHA.interp( dt );
    this->alpha = ALPHA.value;
    
    v1 = ((THEREMAXFlock *)parent)->ruleA(this);
    v2 = ((THEREMAXFlock *)parent)->ruleB(this);
    // v3 = ((THEREMAXFlock *)parent)->ruleC(this);
    // // 
    this->vel = this->vel + v1 + v2 + v3;
    this->loc = this->loc + this->vel * dt;
    
    return;
};


Vector3D THEREMAXFlock::ruleA(THEREMAXBoid * boid)
{
    Vector3D perceivedCenter;
    
    for( vector<YEntity *>::iterator ei = this->children.begin();
            ei!= this->children.end(); ei++ )
    {
        THEREMAXBoid * iteratedBoid = ((THEREMAXBoid *)*ei);
        if(iteratedBoid != boid)
        {
            perceivedCenter += iteratedBoid->loc;
        }

    }
    double scaler = (1 / ((double)this->children.size() - 1));
    perceivedCenter *= scaler;
    return (perceivedCenter - boid->loc) * 0.01;
};

Vector3D THEREMAXFlock::ruleB(THEREMAXBoid * boid)
{
    Vector3D collision;
    
    for( vector<YEntity *>::iterator ei = this->children.begin();
            ei!= this->children.end(); ei++ )
    {
        THEREMAXBoid * iteratedBoid = ((THEREMAXBoid *)*ei);
        if(iteratedBoid != boid)
        {
            Vector3D diff = (iteratedBoid->loc - boid->loc);
            double magnitude = diff.magnitude();
            if(magnitude < 0.2)
            {
                collision = collision -(iteratedBoid->loc - boid->loc);
            }
        }

    }
    return collision;
};

Vector3D THEREMAXFlock::ruleC(THEREMAXBoid * boid)
{
    Vector3D perceivedVelocity;
    
    for( vector<YEntity *>::iterator ei = this->children.begin();
            ei!= this->children.end(); ei++ )
    {
        THEREMAXBoid * iteratedBoid = ((THEREMAXBoid *)*ei);
        if(iteratedBoid != boid)
        {
            perceivedVelocity += iteratedBoid->vel;
        }

    }
    double scaler = (1 / ((double)this->children.size() - 1));
    perceivedVelocity *= scaler;
    return (perceivedVelocity - boid->vel) * 0.125;
};

void THEREMAXFlock::init(int count)
{
    for (int i = 0; i < count; i++)
    {
        THEREMAXBoid * boid = new THEREMAXBoid;
        boid->loc.set(XFun::rand2f(-1.0,1.0),XFun::rand2f(-1.0,1.0), XFun::rand2f(-1.0,1.0));
        // boid->loc.setXYFromPolar( 0.8 - i * 0.01, (360 * i / count) + i * 10);

        this->addChild(boid);
    }
}