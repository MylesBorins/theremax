//-----------------------------------------------------------------------------
// name: theremax-fusionCore.h
// desc: entities for visualization
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------

#include "theremax-flocking.h"
#include "theremax-globals.h"
#include "x-fun.h"

THEREMAXBoid::THEREMAXBoid()
{
    dtCount = 0;
    ALPHA.set(1,1,1);
    this->spark = new THEREMAXSpark;
    
    spark->ALPHA.value = 0.1;
    spark->ALPHA.goal = XFun::rand2f(0.5, 0.8);
    spark->setSize(XFun::rand2f(0.05, 0.4));
    
    this->addChild(spark);
}

void THEREMAXBoid::update( YTimeInterval dt )
{
    dtCount += dt;
    if (dtCount > (1 / THEREMAX_SRATE) * 30)
    {
        Vector3D v1, v2, v3, v4, v5, v6;

        // slew
        ALPHA.interp( dt );
        this->alpha = ALPHA.value;
    
        v1 = ((THEREMAXFlock *)parent)->centerMass(this);
        v2 = ((THEREMAXFlock *)parent)->collisionDetect(this);
        v3 = ((THEREMAXFlock *)parent)->potentialVelocity(this);
        v4 = ((THEREMAXFlock *)parent)->tendToPlace(this);
        v5 = ((THEREMAXFlock *)parent)->boundPosition(this);
        ((THEREMAXFlock *)parent)->boundVelocity(this);
        // // 

        this->vel = (this->vel + v1 + v2 + v3 + v4 + v5);
        dtCount = 0;
    }
    this->loc = this->loc + this->vel * dt;
    return;
};

Vector3D THEREMAXFlock::centerMass(THEREMAXBoid * boid)
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
    return (perceivedCenter - boid->loc) * 0.1;
};

Vector3D THEREMAXFlock::collisionDetect(THEREMAXBoid * boid)
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
            if(magnitude < 0.5)
            {
                collision = collision -(iteratedBoid->loc - boid->loc);
            }
        }

    }
    return collision;
};

Vector3D THEREMAXFlock::potentialVelocity(THEREMAXBoid * boid)
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

Vector3D THEREMAXFlock::tendToPlace(THEREMAXBoid * boid)
{
    Vector3D place(0,0,0);
    double tend = (Globals::cvIntensity * -1 + 1);
    if (tend > .8)
    {
        return (place - boid->loc) * ((Globals::cvIntensity * -1) + 1);
    }
    return (place - boid->loc) * 0.000001;//(place - boid->loc) * 0.0001 * (Globals::cvIntensity * -1 + 1);
}

Vector3D THEREMAXFlock::boundPosition(THEREMAXBoid * boid)
{
    int xmin = -30, xmax = 30, ymin = -50, ymax = 10, zmin = -150, zmax = 10;
    Vector3D v;
    if(boid->loc.x < xmin)
    {
        v.x = 10 * Globals::cvIntensity;
    }
    else if (boid->loc.x > xmax)
    {
        v.x = -10 * Globals::cvIntensity;
    }
    if (boid->loc.y < ymin)
    {
        v.y = 10 * Globals::cvIntensity;
    }
    else if (boid->loc.y > ymax)
    {
        v.y = -10 * Globals::cvIntensity;
    }
    if (boid->loc.z < zmin)
    {
        v.z = 10 * Globals::cvIntensity;
    }
    else if (boid->loc.z > zmax)
    {
        v.z = -10 * Globals::cvIntensity;
    }
    return v;
};

void THEREMAXFlock::boundVelocity(THEREMAXBoid * boid)
{
    float limit = 5;
    Vector3D v;
    float mag = boid->vel.magnitude();
    
    if (mag > limit)
    {
        boid->vel = (boid->vel * (1/mag)) * limit;
    }
}

void THEREMAXFlock::init(int count)
{
    dtCount = 0;
    for (int i = 0; i < count; i++)
    {
        THEREMAXBoid * boid = new THEREMAXBoid;
        boid->loc.set(XFun::rand2f(-1.0,1.0),XFun::rand2f(-1.0,1.0), XFun::rand2f(-1.0,1.0));
        // boid->loc.setXYFromPolar( 0.8 - i * 0.01, (360 * i / count) + i * 10);

        this->addChild(boid);
    }
}