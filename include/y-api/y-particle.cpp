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
#include "y-particle.h"
#include "x-fun.h"
#include <iostream>
using namespace std;




//-----------------------------------------------------------------------------
// name: YPartcile()
// desc: ...
//-----------------------------------------------------------------------------
YParticle::YParticle( GLfloat width )
: YFlare(width), m_isFree(false)
{
    X.slew = 1;
    Y.slew = 1;
    Z.slew = 1;
    ALPHA.set( 1, 1, 1 );
    oriX.slew = 1;
    oriY.slew = 1;
    oriZ.slew = 1;
    origLoc.slew = 1;
    origOri.slew = 1;
    multiplier.set( 1, 1, 1 );

//    velX.slew = 1;
//    velY.slew = 1;
//    velZ.slew = 1;
}




//-----------------------------------------------------------------------------
// name: setLocAsOrig()
// desc: set location as the origin
//-----------------------------------------------------------------------------
void YParticle::setLocAsOrig( const Vector3D & v )
{
    // set loc
    loc = v;
    // set ori
    origLoc = v;
    // set slew X
    X.updateSet( v.x );
    Y.updateSet( v.y );
    X.slew = 1;
    Y.slew = 1;
}




//-----------------------------------------------------------------------------
// name: update()
// desc: update the quantities associated with the frag
//-----------------------------------------------------------------------------
void YParticle::update( YTimeInterval dt )
{
    // update super
    // YFlare::update( dt );
    
    // update
    X.interp( dt );
    Y.interp( dt );
    Z.interp( dt );
    dX.interp( dt );
    dY.interp( dt );
    dZ.interp( dt );
    ALPHA.interp( dt );
    oriX.interp( dt);
    oriY.interp( dt );
    oriZ.interp( dt );
    multiplier.interp( dt );

//    velX.interp( dt );
//    velY.interp( dt );
//    velZ.interp( dt );
//    radius.interp( dt );

    // update
    loc.x = X.value * multiplier.value + dX.value;
    loc.y = Y.value * multiplier.value + dY.value;
    loc.z = Z.value * multiplier.value + dZ.value;
    alpha = ALPHA.value;
    ori.x = oriX.value;
    ori.y = oriY.value;
    ori.z = oriZ.value;

//    velocity.x = velX.value;
//    velocity.y = velY.value;
//    velocity.z = velZ.value;
}




//-----------------------------------------------------------------------------
// name: render()
// desc: dummy function
//-----------------------------------------------------------------------------
void YParticle::render()
{
    // super
    YFlare::render();
}




//-----------------------------------------------------------------------------
// name: addParticle()
// desc: ...
//-----------------------------------------------------------------------------
void YParticleSystem::addParticle( YParticle * particle )
{
    // particle
    m_particles.push_back( particle );
    // activate
    // particle->activate();
}




//-----------------------------------------------------------------------------
// name: spawn()
// desc: ...
//-----------------------------------------------------------------------------
YParticle * YParticleSystem::spawn()
{
    // check
    if( m_numActive >= m_particles.size() )
        return NULL;

    // get it
    YParticle * particle = m_particles[m_numActive++];
    
//    // reset an children fx
//    for( int i = 0; i < particle->children.size(); i++ )
//    {
//        // HACK:
//        OkeRibbon * ribbon = dynamic_cast<OkeRibbon *>( particle->children[i] );
//        if( ribbon != NULL )
//        {
//            ribbon->reset();
//        }
//    }

    // activate
    particle->activate();

    // return it
    return particle;
}




//-----------------------------------------------------------------------------
// name: addInfluence()
// desc: ...
//-----------------------------------------------------------------------------
void YParticleSystem::addInfluence( YParticleInfluence * influence )
{
    // append
    m_influences.push_back( influence );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YParticleSystem::update( YTimeInterval dt )
{    
    // update super
    // YFlare::update( dt );

    // go through influences
    for( int i = 0; i < m_influences.size(); i++ )
    {
        // go through particles
        for( int j = 0; j < m_particles.size(); j++ )
        {
            // check
            if( m_particles[j]->active )
            {
                // apply influence
                m_influences[i]->update( m_particles[j], dt );
            }
        }
    }

    // go through particles
    for( int i = 0; i < m_particles.size(); i++ )
    {
        // check
        if( m_particles[i]->active )
        {
            // update
            m_particles[i]->updateAll( dt );
        }
    }
    
    // pack active
    packActiveParticles();

    // cerr << "numActive: " << m_numActive << endl;
}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YParticleSystem::render()
{
    // the particle
    YParticle * e = NULL;

    // go through particles
    for( int i = 0; i < m_particles.size(); i++ )
    {
        // current
        e = m_particles[i];
        // check
        if( !e->active ) continue;
        // draw all
        e->drawAll();

//        // push
//        glPushMatrix();
//        // translate
//        glTranslatef( e->loc.x, e->loc.y, e->loc.z );
//        // rotate
//        glRotatef( e->ori.z, 0.0f, 0.0f, 1.0f );
//        glRotatef( e->ori.y, 0.0f, 1.0f, 0.0f );
//        glRotatef( e->ori.x, 1.0f, 0.0f, 0.0f );
//        // scale
//        glScalef( e->sca.x, e->sca.y, e->sca.z );
//        // color
//        glColor4f( e->col.x, e->col.y, e->col.z, e->alpha );
//        // render
//        m_renderer->render( m_particles[i] );
//        // pop
//        glPopMatrix();
    }
}




//-----------------------------------------------------------------------------
// name: explode()
// desc: ...
//-----------------------------------------------------------------------------
void YParticleSystem::explode(
    GLfloat xUpBound, GLfloat yzBound, GLfloat oBound,
    GLfloat slewLoc, GLfloat slewOri )
{
    // the particle
    YParticle * e = NULL;

    // go through particles
    for( int i = 0; i < m_particles.size(); i++ )
    {
        // current
        e = m_particles[i];
        // set goal
        e->X.update( XFun::rand2f(-yzBound, yzBound), slewLoc );
        e->Y.update( XFun::rand2f(-yzBound, yzBound), slewLoc );
        e->Z.update( XFun::rand2f(0, xUpBound), slewLoc );
        e->oriX.update( XFun::rand2f(-oBound, oBound), slewOri );
        e->oriY.update( XFun::rand2f(-oBound, oBound), slewOri );
        e->oriZ.update( XFun::rand2f(-oBound, oBound), slewOri );
    }
}




//-----------------------------------------------------------------------------
// name: converge()
// desc: ...
//-----------------------------------------------------------------------------
void YParticleSystem::converge( GLfloat slewLoc, GLfloat slewOri )
{
    // the particle
    YParticle * e = NULL;
    
    // go through particles
    for( int i = 0; i < m_particles.size(); i++ )
    {
        // current
        e = m_particles[i];
        // set goal
        e->X.update( e->origLoc.x, slewLoc );
        e->Y.update( e->origLoc.y, slewLoc );
        e->Z.update( e->origLoc.z, slewLoc );
        e->oriX.update( e->origOri.x, slewOri );
        e->oriY.update( e->origOri.y, slewOri );
        e->oriZ.update( e->origOri.z, slewOri );
    }
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//void YParticleSystem::buildAll( YFXRender *fxRender )
//{
//    for (int i=0; i < m_particles.size(); i++)
//    {
//        fxRender->buildAll( m_particles[i] );
//    }
//}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//void YParticleSystem::updateAllPostRender( YTimeInterval dt )
//{
//    for (int i=0; i < m_particles.size(); i++)
//    {
//        m_particles[i]->updateAllPostRender( dt );
//    }    
//}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YParticleSystem::packActiveParticles()
{    
    // pack
    int count = 0;
    for( int i = 0; i < m_particles.size(); i++ )
    {
        // check
        if( m_particles[i]->active == true )
        {
            if( i != count )
            {
                // swap
                YParticle * swap = m_particles[i];
                m_particles[i] = m_particles[count];
                m_particles[count] = swap;
            }
            // increment count
            count++;
        }
    }
    
    // update
    m_numActive = count;
}
