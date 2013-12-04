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
    ALPHA.set(1,1,1);
    this->spark = new THEREMAXSpark;
    spark->set(0, 0.1, 0.5);
    
    this->addChild(spark);
}

void THEREMAXBoid::update( YTimeInterval dt )
{
    Vector3D v1, v2, v3, v4;

    // slew
    ALPHA.interp( dt );
    this->alpha = ALPHA.value;
    
    v1 = ((THEREMAXFlock *)parent)->centerMass(this);
    v2 = ((THEREMAXFlock *)parent)->collisionDetect(this);
    v3 = ((THEREMAXFlock *)parent)->potentialVelocity(this);
    v4 = ((THEREMAXFlock *)parent)->tendToPlace(this);
    // // 
    this->vel = this->vel + v1 + v2 + v3 + v4;
    this->loc = this->loc + this->vel;
    
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
    return (perceivedCenter - boid->loc) * 0.01;
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
            if(magnitude < 0.2)
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
    Vector3D place(0,3,0);
    return (place - boid->loc) * 0.0001;
}

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