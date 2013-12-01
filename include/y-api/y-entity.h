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
// name: y-entity.h
// desc: base entity
//
// name: Ge Wang (ge@ccrma.stanford.edu)
// date: spring 2011
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_ENTITY_H__
#define __MCD_Y_ENTITY_H__

#include <vector>
#include <string>
#include "x-gfx.h"


// forward references
class YEntity;

// A block that does something with an entity and returns true if it succeeds
typedef void (^EntityBlock)( YEntity * );

// the data type to use for elapsed time
typedef double YTimeInterval;





//-----------------------------------------------------------------------------
// name: class YEntity
// desc: a node in a scene graph
//-----------------------------------------------------------------------------
class YEntity
{
public:
    // constructor
    YEntity() : parent(NULL), sca(1, 1, 1), col(1, 1, 1), alpha(1), 
            active(true), selected(false), hidden(false) { }

public:
    // use this for anything that even remotely effects the world state.
    virtual void update( YTimeInterval dt ) { }
    // renders entity (assumes transforms, etc have been applied)
    virtual void render() { }
    // updates you need to do after render; this is somewhat of a hack,
    // needed by FX to get GL state in render before it can update
    virtual void updatePostRender( YTimeInterval dt ) {}
    
public:
    // updates with all children
    virtual void updateAll( YTimeInterval dt );
    // draws with all children
    virtual void drawAll();
    // all post-render updates
    virtual void updateAllPostRender( YTimeInterval dt );
    
public:
    // add child
    void addChild( YEntity * child );
    // remove all children (NOTE: this must be done BEFORE any child is deleted)
    void removeAllChildren();
    // self or any parent selected?
    bool anyParentSelected();

    // apply a block to this entire subtree
    void apply( EntityBlock block );
    // print out a scene graph to the console for your amusement
    void dumpSceneGraph( int depth = 0 );
    // set the color of this and all children
    void recursiveSetColor( const Vector3D & newCol );
    // overwrite this if you want to do something special, like color masking
    virtual void setColor( const Vector3D & newCol );

    // description
    virtual std::string desc() const;

public:
    // location
    Vector3D loc;
    // orientation
    Vector3D ori;
    // scaling
    Vector3D sca;
    // color
    Vector3D col;
    // alpha
    GLfloat alpha;
    // velocity
    Vector3D vel;

public:
    // active -- setting false disables update and display
    // of this node and all it's children
    bool active;
    // hidden -- setting false disables display of 
    // this node only
    bool hidden;
    // selected?
    bool selected;
    // name
    std::string name;

protected:
    // applies translation, rotation, etc
    void applyTransforms();
    // pop
    void popTransforms();
    
protected: // never set these directly, always use addChild
    // parent in the scene graph
    YEntity * parent;
    // child nodes in the scene graph
    std::vector<YEntity *> children;
    
private:
    // make sure no subclasses are using the old
    // update function
    virtual void update() { assert(0); }
};




//-----------------------------------------------------------------------------
// name: class YText
// desc: simple text rendering
//-----------------------------------------------------------------------------
class YText : public YEntity
{
public:
    YText( GLfloat initialAlpha = 1.0f );
    
public:
    // set text
    void set( const std::string & text );
    // set width
    void setWidth( GLfloat width );
    // set stretch (horizontal)
    void setStretch( GLfloat factor );
    // get the length in gl units
    GLfloat getLength() const { return m_length; }
    // set location with centering
    void setCenterLocation( const Vector3D & v );

public:
    // fade to a particular alpha
    virtual void fade( GLfloat _alpha, GLfloat slew = 1 );
    
public:
    virtual void update( YTimeInterval dt );
    virtual void render();

public:
    // static draw method
    static void drawString( const std::string & text );

protected:
    // the text
    std::string m_text;
    // the width
    GLfloat m_width;
    // the length
    GLfloat m_length;
    // stretch
    GLfloat m_stretch;
    // alpha slew
    Vector3D m_iAlpha;
};




//-----------------------------------------------------------------------------
// name: class YCube
// desc: abstraction for a cube entity
//-----------------------------------------------------------------------------
class YCube : public YEntity
{
public:
    YCube() : size( 1, 1, 1.0f ) { }

public:
    virtual void update( YTimeInterval dt );
    virtual void render();

public:
    Vector3D size;
};




//-----------------------------------------------------------------------------
// name: class YCubeOutline
// desc: abstraction for a cube entity
//-----------------------------------------------------------------------------
class YCubeOutline : public YCube
{
public:
    virtual void update( YTimeInterval dt );
    virtual void render();

public:
    Vector3D outlineColor;
};




////-----------------------------------------------------------------------------
//// name: class YBox
//// desc: abstraction for a box entity
////-----------------------------------------------------------------------------
//class YBox : public YEntity
//{
//public:
//    YBox() : size( 1, 1, 1.0f ) { }
//    
//public:
//    virtual void update( YTimeInterval dt );
//    virtual void render();
//    
//public:
//    Vector3D size;
//    Vector3D dims;
//    Vector3D outlineColor;
//};




//-----------------------------------------------------------------------------
// name: class YSphere
// desc: abstraction for a sphere entity
//-----------------------------------------------------------------------------
class YSphere : public YEntity
{
public:
    YSphere() : size( 1, 1, 1.0f ), stacks(10), slices(10) { }
    
public:
    virtual void update( YTimeInterval dt );
    virtual void render();
    
public:
    Vector3D size;
    GLint slices;
    GLint stacks;
};




//-----------------------------------------------------------------------------
// name: class YCone
// desc: abstraction for a cone entity
//-----------------------------------------------------------------------------
class YCone : public YEntity
{
public:
    YCone() : size( 1, 1, 1.0f ), base(1.0f, 1.0f, 1.0f), 
              height(1.0f, 1.0f, 1.0f), slices(10), stacks(10)
              { }

public:
    virtual void update( YTimeInterval dt );
    virtual void render();
    
public:
    Vector3D size;
    Vector3D base;
    Vector3D height;
    GLint slices;
    GLint stacks;
};




//-----------------------------------------------------------------------------
// name: class YFlare
// desc: entity to represent a flare in the graphics
//-----------------------------------------------------------------------------
class YFlare : public YEntity
{
public:
    YFlare( GLfloat width = 1.0f );

public:
    // set flare attributes
    void set( GLfloat _scale, GLfloat _alpha, GLfloat _scale_factor, 
              GLfloat _alpha_factor, GLuint _texture );
    // activate!
    virtual void activate() { active = true; }
    // is currently active, a question
    virtual bool isActive() const { return active; }
    // set alpha
    virtual void setAlpha( GLfloat _alpha ) { alpha = _alpha; }
    
public:
    virtual void update( YTimeInterval dt );
    virtual void render();
    
public:
    GLfloat scale;
    GLfloat scale_factor;
    GLfloat scale_lowerBound;
    GLfloat alpha_factor;
    GLuint texture;
    GLboolean use_depth;

public:
    GLfloat half_width;
    GLfloat vertices[8];
	
public: // HACK:
	GLboolean oscillate;
	GLfloat osc_t;
	GLfloat osc_inc;
	GLfloat osc_amplitude;
    GLboolean osc_axisX;
};




//-----------------------------------------------------------------------------
// name: class YBokeh
// desc: entity to represent a bokeh in the graphics
//-----------------------------------------------------------------------------
class YBokeh : public YFlare
{
public:
    YBokeh( GLfloat width = 1.0f ) : YFlare(width),
      scale_actual(1), alpha_actual(1), t(0), f(12), t_step(.005) { }

    // set bokeh params
    void setBokehParams( GLfloat time, GLfloat freq, GLfloat time_step,
                         const Vector3D & xyz, const Vector3D & rgb );
    // set flare attributes
    void set( GLfloat _scale, GLfloat _alpha, GLfloat _scale_factor, 
              GLfloat _alpha_factor, GLuint _texture );
    
    // set alpha
    virtual void setAlpha( GLfloat _alpha ) { alpha = alpha_actual = _alpha; }
    // virtual void setAlpha( GLfloat _alpha ) { iAlpha.update( _alpha ); alpha_actual = _alpha; }

public:
    virtual void update( YTimeInterval dt );
    virtual void render();

    // slew
    iSlew3D iRGB;
    iSlew3D iLoc;
    
protected:
    // scale
    float scale_actual;
    // alpha
    float alpha_actual;
    // time
    double t;
    // time step
    double t_step;
    // freq
    double f;
    // slew
    Vector3D iAlpha;
};




//-----------------------------------------------------------------------------
// name: class YFlareInfluence
// desc: an influence to be applied to flares
//-----------------------------------------------------------------------------
class YFlareInfluence
{
public:
    virtual ~YFlareInfluence() { }
    virtual void update( YTimeInterval dt ) { }
    virtual void apply( YFlare * flare, YTimeInterval dt ) = 0;
};




//-----------------------------------------------------------------------------
// name: class YFlarePool
// desc: ...
//-----------------------------------------------------------------------------
class YFlarePool : public YEntity
{
public:
    YFlarePool( unsigned long capacity );
    virtual ~YFlarePool();
    
public:
    // add a flare to pool (subject to capacity)
    // (the flare will be memory-managed by the pool)
    bool add( YFlare * flare );
    
public:
    // spawn a flare (if available)
    YFlare * spawn();
    
    // add influence to be applied to each flare
    // (the influence will be memory-managed by the pool)
    void addInfluence( YFlareInfluence * influence )
    { influences.push_back( influence ); }
    
public:
    // get the capacity
    unsigned long getCapacity() const { return m_capacity; }
    // get the size
    unsigned long getSize() const { return m_size; }
    // get actives
    unsigned long getActives() const { return m_numActive; }

public:
    virtual void update( YTimeInterval dt );
    virtual void render();
    
public:
    // num active
    unsigned long m_numActive;
    // size
    unsigned long m_size;
    // capacity
    unsigned long m_capacity;
    // the pool
    YFlare ** m_pool;
    // vector of influences
    std::vector<YFlareInfluence *> influences;
};




//-----------------------------------------------------------------------------
// name: class YColumn
// desc: entity to represent a flare in the graphics
//-----------------------------------------------------------------------------
class YColumn : public YFlare
{
public:
    YColumn( GLfloat _height = 1.0f, GLfloat width = 1.0f ) : YFlare( width ),
        numLayers(8), rotation(0), rotationRate(1.0f), taper(1), taper2(1)
    {
        // calculate half width
        half_width = width / 2.0f;
        // height
        height = _height;
        // compute
        compute();
                
        reactivating = false;
    }
    
    // update rotation rate
    virtual void updateRotationRate( GLfloat rate )
    {
        // set
        rotationRate = rate;
    }
    
    // update rotation value
    virtual void updateRotation( GLfloat r )
    {
        // set
        rotation = r;
    }
    
    // update
    virtual void updateHeight( GLfloat _height )
    {
        // set
        height = _height;
        // compute
        compute();
    }
    
    // update
    virtual void updateWidth( GLfloat width )
    {
        // calculate half width
        half_width = width / 2.0f;
        // compute
        compute();
    }
    
    // update
    virtual void updateTaper( GLfloat _taper, GLfloat _taper2 )
    {
        // set
        taper = _taper;
        taper2 = _taper2;
        // compute
        compute();
    }
    
public:
    virtual void update( YTimeInterval dt );
    virtual void render();
    virtual void activate();
    void reactivate();
    
public:
    // num layers
    int numLayers;
    // rotate
    GLfloat rotation;

    // rotate rate
    GLfloat rotationRate;
    // height
    GLfloat height;
    // taper
    GLfloat taper;
    // taper 2 (other end)
    GLfloat taper2;
    // color array
    GLfloat colorVals[16];
    
protected:
    virtual void compute()
    { 
        // generate vertices
        vertices[0] = -half_width * taper;
        vertices[1] = 0;
        vertices[2] = half_width * taper;
        vertices[3] = 0;
        vertices[4] = -half_width * taper2;
        vertices[5] = height;
        vertices[6] = half_width * taper2;
        vertices[7] = height;
    }
    bool reactivating;
};




//-----------------------------------------------------------------------------
// name: class YPitchVortex
// desc: ...
//-----------------------------------------------------------------------------
class YPitchVortex : public YEntity
{
public:
    YPitchVortex();
    virtual ~YPitchVortex();
    
public:
    virtual void init( GLuint texture );
    virtual void setRadius( GLfloat radius ) { m_radius = radius; }
    virtual GLfloat getRadius() const { return m_radius; }

public:
    // map pitch
    virtual void map( int pitch, float intensity );
    virtual void setSinglePitchMode( bool singlePitchMode=true );

public:
    void show();
    void hide();

public:
    virtual void update( YTimeInterval dt );
    virtual void render();
    
public:
    static const Vector3D & pitch2color( int pitch );
    static const Vector3D & pitch2color_f( float pitch ); // rounds to nearest int
    
protected:
    // array of light columns
    std::vector<YColumn *> m_lightColumns;
    // current column index
    int m_currentIndex;
    // current alpha goal
    Vector3D m_currentAlphaGoal;
    // radius
    GLfloat m_radius;
    
    // in single pitch mode, only one column is 
    // active at a time for gameplay
    bool m_singlePitchMode;
    // on
    bool m_on;
    
protected:
    // color map for pitches
    static std::vector<Vector3D> ourColorMap;
};




//-----------------------------------------------------------------------------
// name: class YIris
// desc: ...
//-----------------------------------------------------------------------------
class YIris : public YEntity
{
public:
    YIris();
    virtual ~YIris();
    
public:
    void init( unsigned int N, GLfloat outlineWidth = 1.0f );
    
public:
    virtual void update( YTimeInterval dt );
    virtual void render();
    
public:
    // number of blades
    GLuint m_numBlades;
    // position (0 == closed : 1 == open)
    Vector3D m_position;
    // 2d rotation matrix
    GLfloat m_R[4];
    // verts
    GLfloat m_verts[9];
    // outline width
    GLfloat m_outlineWidth;
};




#endif
