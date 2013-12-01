/*----------------------------------------------------------------------------
  MCD-Y: higher-level objects for audio/graphics/interaction programming
         (sibling of MCD-X API)

  Copyright (c) 2013 Ge Wang
    All rights reserved.
    http://ccrma.stanford.edu/~ge/

  Music, Computing, Design Group @ CCRMA, Stanford University
    http://ccrma.stanford.edu/groups/mcd/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// name: y-particle.cpp
// desc: (not so smart) particle related implementation
//
// name: Ge Wang (ge@ccrma.stanfor.edu)
//       parts adapted from Jieun Oh and Ge Wang's "Converge"
// date: summer 2011
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_PARTICLE_H__
#define __MCD_Y_PARTICLE_H__

#include <vector>
#include "y-entity.h"

// forward reference
class YParticleRenderer;
class YParticleInfluence;
class YParticleSystem;




//-----------------------------------------------------------------------------
// name: class YParticle
// desc: ...
//-----------------------------------------------------------------------------
class YParticle : public YFlare
{
public:
    YParticle( GLfloat width = 1.0f );
    virtual ~YParticle() { }

public:
    void setLocAsOrig( const Vector3D & v );

public:
    virtual void update( YTimeInterval dt );
    virtual void render();

public:
    // radius
    // Vector3D radius;
    // multiplier
    Vector3D multiplier;

public:
    Vector3D X;
    Vector3D Y;
    Vector3D Z;
    Vector3D dX;
    Vector3D dY;
    Vector3D dZ;
    Vector3D ALPHA;
    Vector3D oriX;
    Vector3D oriY;
    Vector3D oriZ;

//    Vector3D velX;
//    Vector3D velY;
//    Vector3D velZ;

    Vector3D origLoc;
    Vector3D origOri;

protected:
    bool m_isFree;
};




//-----------------------------------------------------------------------------
// name: class YParticleInfluence
// desc: ...
//-----------------------------------------------------------------------------
class YParticleInfluence
{
public:
    virtual void update( YParticle * frag, YTimeInterval dt ) { }
};




//-----------------------------------------------------------------------------
// name: class YParticleRenderer
// desc: ...
//-----------------------------------------------------------------------------
class YParticleRenderer
{
public:
    virtual void render( YParticle * frag ) { }
};




//-----------------------------------------------------------------------------
// name: class YParticleSystem
// desc: ...
//-----------------------------------------------------------------------------
class YParticleSystem : public YFlare
{
public:
    YParticleSystem() : m_numActive(0) { this->active = true; }

public:
    virtual void addParticle( YParticle * particle );
    virtual void addInfluence( YParticleInfluence * influence );
    
//public:
//    virtual void updateAllPostRender( YTimeInterval dt );
//    void buildAll( YFXRender * fx );

public:
    // get and activate the next particle (if available)
    YParticle * spawn();
    // get all particles in system
    std::vector<YParticle *> & getParticles() { return m_particles; }

public:
    void explode( GLfloat xUpBound, GLfloat yzBound, GLfloat oBound,
                  GLfloat slewLoc, GLfloat slewOri );
    void converge( GLfloat slewLoc, GLfloat slewOri );

public:
    virtual void update( YTimeInterval dt );
    virtual void render();

protected:
    void packActiveParticles();
    std::vector<YParticle *> m_particles;
    std::vector<YParticleInfluence *> m_influences;
    // num active
    unsigned long m_numActive;
};




#endif
