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
// name: y-entity.cpp
// desc: base entity
//
// name: Ge Wang (ge@ccrma.stanford.edu)
// date: spring 2011
//-----------------------------------------------------------------------------
#include "y-entity.h"
#include "x-fun.h"
#include <iostream>
using namespace std;




//-----------------------------------------------------------------------------
// name: updateAll()
// desc: updates with all children
//-----------------------------------------------------------------------------
void YEntity::updateAll( YTimeInterval dt )
{
    // check
    if( !active ) return;

    // update self
    update( dt );
    
    // update children
    for( vector<YEntity *>::iterator itr = children.begin(); 
         itr != children.end(); itr++ )
    {
        (*itr)->updateAll( dt );
    }
}




//-----------------------------------------------------------------------------
// name: updateAllPostRender()
//
// updates that you need to do after render. This is somewhat of a hack,
// needed by FX because it needs to get GL state in render before it 
// can update
//----------------------------------------------------------------------------
void YEntity::updateAllPostRender( YTimeInterval dt )
{
    // update self
    updatePostRender( dt );
    
    // update children
    for( vector<YEntity *>::iterator itr = children.begin(); 
        itr != children.end(); itr++ )
    {
        (*itr)->updateAllPostRender( dt );
    }
}




//-----------------------------------------------------------------------------
// name: desc()
// desc: Returns a description (usually type and name) of this object. Useful
// for debugging stuff.
//-----------------------------------------------------------------------------
std::string YEntity::desc() const
{
    return "YEntity [unnamed]";
}




//-----------------------------------------------------------------------------
// name: apply
// desc: applies a block of code to this entity and the entire subtree
//-----------------------------------------------------------------------------
void YEntity::apply( EntityBlock block )
{
    // Run the block on ourselves
    block( this );
    
    // Apply to our children
    for( vector<YEntity *>::iterator itr = children.begin(); 
         itr != children.end(); itr++ )
    {
        (*itr)->apply( block );
    }
}




//-----------------------------------------------------------------------------
// name: drawAll()
// desc: draws with all children
//-----------------------------------------------------------------------------
void YEntity::drawAll()
{
    if( !active )
        return;
    
    // apply transforms
    applyTransforms();
    
    // render self if not hidden
    if( !hidden )
    {
        render();
    }
    
    // draw children
    for( vector<YEntity *>::iterator itr = children.begin(); 
         itr != children.end(); itr++ )
    {
        (*itr)->drawAll();
    }

    // pop
    popTransforms();
}




//-----------------------------------------------------------------------------
// name: addChild
// desc: Adds the entity as a child of this one
//-----------------------------------------------------------------------------
void YEntity::addChild( YEntity * child )
{
    child->parent = this;
    children.push_back( child ); 
}




//-----------------------------------------------------------------------------
// name: removeAllChildren()
// desc: remove all children
//       (NOTE: this must be done before any child is deleted)
//-----------------------------------------------------------------------------
void YEntity::removeAllChildren()
{
    // iterate
    for( long i = 0; i < children.size(); i++ )
    {
        // clear
        children[i]->parent = NULL;
    }
    
    // clear
    children.clear();
}




//-----------------------------------------------------------------------------
// name: anyParentSelected()
// desc: returns true if this node or any parent up the chain is selected
//-----------------------------------------------------------------------------
bool YEntity::anyParentSelected()
{
    // check if this node is selected
    if( selected )
    {
        return true; 
    } 
    
    // check parent recursively
    return parent != NULL ? parent->anyParentSelected() : false; 
}




//-----------------------------------------------------------------------------
// name: applyTransforms()
// desc: applies translation, rotation, etc
//-----------------------------------------------------------------------------
void YEntity::applyTransforms()
{
    // push
    glPushMatrix();
    // translate
    glTranslatef( loc.x, loc.y, loc.z );
    // rotate
    glRotatef( ori.z, 0.0f, 0.0f, 1.0f );
    glRotatef( ori.y, 0.0f, 1.0f, 0.0f );
    glRotatef( ori.x, 1.0f, 0.0f, 0.0f );
    // scale
    glScalef( sca.x, sca.y, sca.z );
    // color
    glColor4f( col.x, col.y, col.z, alpha );
}




//-----------------------------------------------------------------------------
// name: dumpSceneGraph()
// desc: ...
//-----------------------------------------------------------------------------
void YEntity::dumpSceneGraph( int depth )
{
    // make padding
    char padding[depth+1];
    memset( padding, ' ', depth );
    padding[depth]=0;
 
    // print current node
    printf( "%s[%s]\n", padding, desc().c_str() );
    
    // print childrens
    for( std::vector<YEntity*>::iterator ei = children.begin();
         ei != children.end(); ++ei )
    {
        (*ei)->dumpSceneGraph( depth+1 );
    }
}




//-----------------------------------------------------------------------------
// name: recursiveSetColor()
// desc: set color on this and children, recursively
//-----------------------------------------------------------------------------
void YEntity::recursiveSetColor( const Vector3D & newCol )
{
    // set color
    setColor( newCol );
    // iterate through children
    for( std::vector<YEntity*>::iterator ei = children.begin();
         ei != children.end(); ++ei )
    {
        (*ei)->recursiveSetColor( newCol );
    }
}




//-----------------------------------------------------------------------------
// name: setColor()
// desc: set the color of this entity
//-----------------------------------------------------------------------------
void YEntity::setColor( const Vector3D & newCol )
{
    col = newCol;
}




//-----------------------------------------------------------------------------
// name: popTransforms()
// desc: pop
//-----------------------------------------------------------------------------
void YEntity::popTransforms()
{
    glPopMatrix();
}




//-----------------------------------------------------------------------------
// name: YText()
// desc: ...
//-----------------------------------------------------------------------------
YText::YText( GLfloat initialAlpha )
{
    // initialize str
    m_text = "";
    // line width
    m_width = 1;
    // length
    m_length = 1;
    // stretch
    m_stretch = 1;
    // set alpha
    alpha = initialAlpha;
    // alpha slew
    m_iAlpha.updateSet( initialAlpha, 1 );
}




//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
void YText::set( const string & text )
{
    // copy
    m_text = text;
    
    // compute length
    m_length = .001 * glutStrokeLength( GLUT_STROKE_ROMAN,
                                        (const unsigned char *)text.c_str() );
}




//-----------------------------------------------------------------------------
// name: setWidth()
// desc: ...
//-----------------------------------------------------------------------------
void YText::setWidth( GLfloat width )
{
    // width
    m_width = width;
}




//-----------------------------------------------------------------------------
// name: setStretch()
// desc: ...
//-----------------------------------------------------------------------------
void YText::setStretch( GLfloat stretch )
{
    // set
    m_stretch = stretch;
}




//-----------------------------------------------------------------------------
// name: YText::
// desc: set location with centering
//-----------------------------------------------------------------------------
void YText::setCenterLocation( const Vector3D & v )
{
    // set loc
    this->loc = v;
    // offset by length
    this->loc.x -= this->getLength() * sca.x * m_stretch / 2;
}




//-----------------------------------------------------------------------------
// name: fade()
// desc: fade to a particular alpha
//-----------------------------------------------------------------------------
void YText::fade( GLfloat _alpha, GLfloat slew )
{
    // check slew; <= 0 means immediate
    if( slew > 0 )
    {
        m_iAlpha.update( _alpha, slew );
    }
    else
    {
        // immediate (slew is unchanged)
        m_iAlpha.updateSet( _alpha );
    }
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YText::update( YTimeInterval dt )
{
    // interpolate
    m_iAlpha.interp( dt );
    
    // set it
    alpha = m_iAlpha.value;
}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YText::render()
{
    // blend
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // lighting
    glDisable( GL_LIGHTING );
    
    // set the linewidth
    glLineWidth( m_width );
    // push
    glPushMatrix();
    // stretch
    glScalef( m_stretch, 1, 1 );
    // draw the string
    drawString( m_text );
    // pop
    glPopMatrix();
}




//-----------------------------------------------------------------------------
// name: drawString()
// desc: string rendering method
//-----------------------------------------------------------------------------
void YText::drawString( const std::string & text )
{
    // get string length
    GLint len = (GLint)text.length();
    // get c string
    const char * str = text.c_str();
    
    // push
    glPushMatrix();
    // scale to be smaller
    glScalef( .001f, .001f, .001f );
    // draw each character
    for( int i = 0; i < len; i++ )
        glutStrokeCharacter( GLUT_STROKE_ROMAN, str[i] );
    // pop
    glPopMatrix();
}




// normals
static const GLfloat g_normals[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
};




// texture coords
static const GLfloat g_texCoords[] = {
    0, 0,
    1, 0,
    0, 1,
    1, 1
};




// texture coords
static const GLfloat g_texCoords2[] = {
    0, 0,
    .99, 0,
    0, .99,
    .99, .99
};




//-----------------------------------------------------------------------------
// name: YFlare()
// desc: ...
//-----------------------------------------------------------------------------
YFlare::YFlare( GLfloat width )
    : scale_lowerBound(0.0f), oscillate(false), osc_axisX(false)
{
    // start active
    active = true;
    // no depth test
    use_depth = false;
    // calculate half width
    half_width = width / 2.0f;
    // generate vertices
    vertices[0] = -half_width;
    vertices[1] = -half_width;
    vertices[2] = half_width;
    vertices[3] = -half_width;
    vertices[4] = -half_width;
    vertices[5] = half_width;
    vertices[6] = half_width;
    vertices[7] = half_width;
    
    // set
    scale = 1.0f;
    scale_factor = 1.0f;
    alpha_factor = 1.0f;
    texture = 0;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: set some paramss
//-----------------------------------------------------------------------------
void YFlare::set( GLfloat _scale, GLfloat _alpha, GLfloat _scale_factor, 
                   GLfloat _alpha_factor, GLuint _texture )
{
    this->scale = _scale;
    this->setAlpha( _alpha );
    this->scale_factor = _scale_factor;
    this->alpha_factor = _alpha_factor;
    this->texture = _texture;
}




//-----------------------------------------------------------------------------
// name: update()
// desc: update a flare entity
//-----------------------------------------------------------------------------
void YFlare::update( YTimeInterval dt )
{
    // alpha
    // alpha *= alpha_factor;
    // scale *= scale_factor;
    
    // fugde: figure out diff
    GLfloat alpha_diff = alpha_factor - 1.0f;
    GLfloat scale_diff = scale_factor - 1.0f;
    // fudge more
    alpha_diff *= 30.0f * dt;
    scale_diff *= 30.0f * dt;

    // alpha
    alpha += alpha_diff;
    scale += scale_diff;
    
    // stop scaling
    if( scale < scale_lowerBound ) scale_factor = 1.0f;
    
    // active
    if( alpha < .01f )
    {
        active = false;
        // HACK:
        oscillate = false;
    }
}




//-----------------------------------------------------------------------------
// name: render()
// desc: render a flare entity
//-----------------------------------------------------------------------------
void YFlare::render()
{
    // disable lighting
    glDisable( GL_LIGHTING );
    // depth
    if( use_depth ) glEnable( GL_DEPTH_TEST );
    else glDisable( GL_DEPTH_TEST );
    // disable writing
    glDepthMask( GL_FALSE );

    // enable texture mapping
    glEnable( GL_TEXTURE_2D );
    // enable blending
    glEnable( GL_BLEND );
    // blend function
    // glBlendFunc( GL_ONE, GL_ONE );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // bind the texture
    glBindTexture( GL_TEXTURE_2D, texture );

    glPushMatrix();

    // enable
    glEnableClientState( GL_VERTEX_ARRAY );
    // glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    // vertex
    glVertexPointer( 2, GL_FLOAT, 0, vertices );
    // normal
    // glNormalPointer( GL_FLOAT, 0, g_normals );
    // texture coordinate
    glTexCoordPointer( 2, GL_FLOAT, 0, g_texCoords );

    // scale
    glScalef( scale, scale, scale );
    // triangle strip
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    // glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

    glPopMatrix();

    // disable
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );
    // enable writing
    glDepthMask( GL_TRUE );
}




//-----------------------------------------------------------------------------
// name: set()
// desc: set some paramss
//-----------------------------------------------------------------------------
void YBokeh::set( GLfloat _scale, GLfloat _alpha, GLfloat _scale_factor, 
                  GLfloat _alpha_factor, GLuint _texture )
{
    // super
    YFlare::set( _scale, _alpha, _scale_factor, _alpha_factor, _texture );
    
    // set for this
    this->scale_actual = _scale;
}




//-----------------------------------------------------------------------------
// name: setBokehParam()
// desc: set bokeh params
//-----------------------------------------------------------------------------
void YBokeh::setBokehParams( GLfloat time, GLfloat freq, GLfloat time_step,
                              const Vector3D & xyz, const Vector3D & rgb )
{
    // set
    t = time;
    f = freq;
    t_step = time_step;
    iRGB.update( rgb, .5 );
    iLoc.update( xyz, .5 );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YBokeh::update( YTimeInterval dt )
{
    // update super
    YFlare::update( dt );

    // interp
    iRGB.interp( dt );
    iLoc.interp( dt );
    
    // update
    col = iRGB.actual();
    loc = iLoc.actual();
    
//    // interp
//    iAlpha.interp( MoGfx::delta() );
//
//    // set
//    this->alpha = iAlpha.value;
//
//    // active
//    if( this->alpha < .01f )
//    this->active = false;
}



#include <iostream>
//-----------------------------------------------------------------------------
// name: render()
// desc: render a flare entity
//-----------------------------------------------------------------------------
void YBokeh::render()
{
    // disable lighting
    glDisable( GL_LIGHTING );
    // depth
    glEnable( GL_DEPTH_TEST );
    // disable writing
    glDepthMask( GL_FALSE );
    
    // enable blending
    glEnable( GL_BLEND );
    // blend function
    glBlendFunc( GL_ONE, GL_ONE );
    // enable texture mapping
    glEnable( GL_TEXTURE_2D );
    // bind the texture
    glBindTexture( GL_TEXTURE_2D, texture );
    
    glPushMatrix();
    
    // enable
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    // modulate the scale
    scale_actual = scale * (.7 + .25*::cos( t*2*M_PI*f ));
    // modulate the alpha
    alpha_actual = alpha * (.7 + .3*::cos( t*2*M_PI*f*4));
    // move time
    t += t_step * XGfx::delta();

    // color
    glColor4f( col.x, col.y, col.z, alpha_actual );
    // vertex
    glVertexPointer( 2, GL_FLOAT, 0, vertices );
    // normal
    glNormalPointer( GL_FLOAT, 0, g_normals );
    // texture coordinate
    glTexCoordPointer( 2, GL_FLOAT, 0, g_texCoords );
    
    // scale
    glScalef( scale_actual, scale_actual, scale_actual );
    // alpha
    alpha *= alpha_factor;
    scale *= scale_factor;
    // triangle strip
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    
    // stop scaling
    if( scale < scale_lowerBound ) scale_factor = 1.0f;
    
    // active
    if( alpha < .05f )
	{
		active = false;
		// HACK:
		oscillate = false;
	}
    
    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    
    glPopMatrix();
    
    // disable
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );
    // enable writing
    glDepthMask( GL_TRUE );
    
}




//-----------------------------------------------------------------------------
// name: YFlarePool()
// desc: constructor
//-----------------------------------------------------------------------------
YFlarePool::YFlarePool( unsigned long capacity )
{
    // zero out
    m_numActive = m_capacity = m_size = 0;
    
    // allocate
    m_pool = new YFlare *[capacity];
    // sanity check
    if( !m_pool ) return;
    // zero
    memset( m_pool, 0, capacity * sizeof(YFlare *) );
    
    // set
    m_capacity = capacity;
}




//-----------------------------------------------------------------------------
// name: YFlarePool()
// desc: destructor
//-----------------------------------------------------------------------------
YFlarePool::~YFlarePool()
{
    // check
    if( m_pool != NULL )
    {
        // delete the flares
        for( int i = 0; i < m_size; i++ )
            SAFE_DELETE( m_pool[i] );
        
        // delete the influences
        for( int i = 0; i < influences.size(); i++ )
            SAFE_DELETE( influences[i] );
        
        // delete
        SAFE_DELETE_ARRAY( m_pool );
        
        // clear
        influences.clear();
        
        // zero out
        m_numActive = 0;
        m_capacity = 0;
        m_size = 0;
    }
}




//-----------------------------------------------------------------------------
// name: add()
// desc: add a flare to the pool, returns whether successful (within capacity?)
//-----------------------------------------------------------------------------
bool YFlarePool::add( YFlare * flare )
{
    // check
    if( m_size >= m_capacity )
        return false;
    
    // deactivate
    flare->active = false;
    
    // append
    m_pool[m_size++] = flare;
    
    // done
    return true;
}




//-----------------------------------------------------------------------------
// name: spawn()
// desc: spawn a flare in the pool
//-----------------------------------------------------------------------------
// YFlare * YFlarePool::spawn( const Vector3D & loc, const Vector3D & color,
//                               GLfloat scale, GLfloat alpha, GLfloat scale_factor, GLfloat alpha_factor,
//                               GLuint texture, GLfloat scale_lowerBound, GLboolean manualDeactivate )
YFlare * YFlarePool::spawn()
{
    // set flare
    if( m_numActive < m_size )
    {
        // get the flare
        YFlare * f = m_pool[m_numActive];
        // set some defaults
        f->loc.set( 0, 0, 0 );
        f->col.set( 1, 1, 1 );
        f->set( 1, 1, 1, 1, 0 );
        f->scale_lowerBound = 0.0f;

        // activate!
        f->activate();
        
        // count
        m_numActive++;
        
        // return
        return f;
    }
    
    // return
    return NULL;   
}




//-----------------------------------------------------------------------------
// name: sortZ()
// desc: sort by z
//-----------------------------------------------------------------------------
//static void sortZ( YFlare * array[], int size )
//{
//    int current = 1;
//    YFlare * temp = NULL;
//    int walker = 0;
//    
//    while( current < size )
//    {
//        temp = array[current];
//        walker = current - 1;
//        
//        while( ( walker >= 0 ) && ( temp->loc.z < array[walker]->loc.z ) )
//        {
//            array[walker+1] = array[walker];
//            walker--;
//        }
//        
//        array[walker+1] = temp;
//        current++;
//    }
//}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YFlarePool::update( YTimeInterval dt )
{    
    // pack flares
    int count = 0;
    // iterate
    for( int i = 0; i < m_size; i++ )
    {
        // update
        m_pool[i]->updateAll( dt );
        
        // swap
        if( m_pool[i]->active == true )
        {
            if( i != count )
            {
                // swap
                YFlare * swap = m_pool[i];
                m_pool[i] = m_pool[count];
                m_pool[count] = swap;
            }

            // increment count
            count++;
        }
    }
    
    // update
    m_numActive = count;
    
    // sort
    // sortZ( m_pool, count );
    
    // update
    for( int j = 0; j < influences.size(); j++ )
    {
        influences[j]->update( dt );
    }
    
    // loop over active and apply influence
    for( int i = 0; i < m_numActive; i++ )
    {
        // influence
        for( int j = 0; j < influences.size(); j++ )
        {
            influences[j]->apply( m_pool[i], dt );
        }
    }
}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YFlarePool::render()
{
    // loop over active
    for( int i = 0; i < m_numActive; i++ )
    {
        // draw it
        m_pool[i]->drawAll();
    }
}




//-----------------------------------------------------------------------------
// name: render()
// desc: render a flare entity
//-----------------------------------------------------------------------------
void YColumn::render()
{
    // disable lighting
    glDisable( GL_LIGHTING );
    
    // disable depth
    // glDisable( GL_DEPTH_TEST );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    
    // disable writing
    glDepthMask( GL_FALSE );
    
    // enable blending
    glEnable( GL_BLEND );
    // blend function
    glBlendFunc( GL_ONE, GL_ONE );
    // enable texture mapping
    glEnable( GL_TEXTURE_2D );
    // bind the texture
    glBindTexture( GL_TEXTURE_2D, texture );
    
    glPushMatrix();
    
    // enable
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    
    // HACK: fudge
    GLfloat fudge = 1.0f / numLayers;
    
    // color
    // glColor4f( col.x * alpha, col.y * alpha, col.z * alpha, alpha );
    colorVals[0] = col.x * alpha * fudge;
    colorVals[1] = col.y * alpha * fudge;
    colorVals[2] = col.z * alpha * fudge;
    colorVals[3] = alpha;
    colorVals[4] = colorVals[0];
    colorVals[5] = colorVals[1];
    colorVals[6] = colorVals[2];
    colorVals[7] = colorVals[3];
    colorVals[8] = colorVals[0];
    colorVals[9] = colorVals[1];
    colorVals[10] = colorVals[2];
    colorVals[11] = colorVals[3];
    colorVals[12] = colorVals[0];
    colorVals[13] = colorVals[1];
    colorVals[14] = colorVals[2];
    colorVals[15] = colorVals[3];
    
    // vertex
    glVertexPointer( 2, GL_FLOAT, 0, vertices );
    // texture coordinate
    glTexCoordPointer( 2, GL_FLOAT, 0, g_texCoords2 );
    // color
    glColorPointer( 4, GL_FLOAT, 0, colorVals );
    
    // rotate
    glRotatef( rotation, 0, 1, 0 );
    
    // draw layers
    for( int i = 0; i < numLayers; i++ )
    {
        // triangle strip
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
        // rotate
        glRotatef( 180.0f / numLayers, 0, 1, 0 );
    }

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    
    glPopMatrix();
    
    // disable
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );
    
    // enable writing
    glDepthMask( GL_TRUE );
}




#include <iostream>
//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YColumn::update( YTimeInterval dt )
{
    // NOTE: don't update super here
    // YFlare::update( dt );
    
    // alpha
    // ge: (2012) commented this out, as this doesn't really work in some case,
    // for example when we are using dynamic time step for updates
    // alpha *= alpha_factor * MoGfx::getDesiredFrameRate() * dt;
    // scale *= scale_factor * MoGfx::getDesiredFrameRate() * dt;
    
//    // hmm... ge: what the poop
//    if( alpha > 0.5 && !reactivating )
//    {
//        alpha_factor = 1.0;
//        alpha = 0.5;
//    }

//    if( reactivating )
//    {
//        if( alpha > 1.0 )
//        {
//            alpha = 1.0;
//            alpha_factor = 0.95;
//        }
//        
//        if( alpha_factor < 1.0 && alpha < 0.5 )
//        {
//            alpha = 0.5;
//            alpha_factor = 1.0;
//            reactivating = false;
//        }
//    }

    // hmm... ge: what does it mean?
    scale = alpha;
    
    // increment
    rotation += rotationRate * 20 * dt;
    
    // stop scaling
    if( scale < scale_lowerBound ) scale_factor = 1.0f;
    
//    // active
//    if( alpha < .01f )
//    {
//        active = false;
//        // HACK:
//        oscillate = false;
//    }
}




//-----------------------------------------------------------------------------
// name: activate()
// desc: ...
//-----------------------------------------------------------------------------
void YColumn::activate()
{
    active = true;
//    alpha = 0.05;
//    alpha_factor = 1.5;
}




//-----------------------------------------------------------------------------
// name: reactivate()
// desc: ...
//-----------------------------------------------------------------------------
void YColumn::reactivate()
{
    active = true;
    reactivating = true;
    alpha = 0.5;
    alpha_factor = 1.7;
}




//-----------------------------------------------------------------------------
// name: g_squareVertices
// desc: vertices for a cube
//-----------------------------------------------------------------------------
static const GLfloat g_squareVertices[] = 
{
    // FRONT
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    // BACK
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    // LEFT
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    // RIGHT
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    // TOP
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    // BOTTOM
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
};




//-----------------------------------------------------------------------------
// name: g_squareNormals
// desc: normals for a cube
//-----------------------------------------------------------------------------
static const GLfloat g_squareNormals[] = 
{
    // FRONT
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    // BACK
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    // LEFT
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    // RIGHT
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    // TOP
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    // BOTTOM
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0
};




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YCube::render()
{
    // enable state
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    // set vertex pointer
    glVertexPointer( 3, GL_FLOAT, 0, g_squareVertices );
    glNormalPointer( GL_FLOAT, 0, g_squareNormals );

    // push
    glPushMatrix();
    // scale
    glScalef( size.value, size.value, size.value );

    // draw it
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 16, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 20, 4 );
    
    // pop
    glPopMatrix();
    
    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YCube::update( YTimeInterval dt )
{
    // interp
    size.interp( dt );
}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YCubeOutline::render()
{
    
    // enable state
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    
    // set vertex pointer
    glVertexPointer( 3, GL_FLOAT, 0, g_squareVertices );
    glNormalPointer( GL_FLOAT, 0, g_squareNormals );
    
    // push
    glPushMatrix();
    // scale
    glScalef( size.value, size.value, size.value );
    
    // draw it
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 16, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 20, 4 );
    
    // color
    glColor4f( outlineColor.x, outlineColor.y, outlineColor.z, 1.0f );
    // draw outline
    glutWireCube( 1.025f );
    
    // pop
    glPopMatrix();
    
    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YCubeOutline::update( YTimeInterval dt )
{
    // interp
    size.interp( dt );
}




////-----------------------------------------------------------------------------
//// name: render()
//// desc: ...
////-----------------------------------------------------------------------------
//void YBox::render()
//{
//    // push
//    glPushMatrix();
//    // scale
//    glScalef( size.value, size.value, size.value );
//    // render
//    glutSolidCube( dims.x, dims.y, dims.z );
//    // line width
//    glLineWidth( 2.0 );
//    // color
//    glColor4f( outlineColor.x, outlineColor.y, outlineColor.z, 1.0f );
//    // render
//    glutWireCube( dims.x * 1.0025, dims.y * 1.0025, dims.z * 1.0025 );
//    // pop
//    glPopMatrix();
//}
//
//
//
//
////-----------------------------------------------------------------------------
//// name: update()
//// desc: ...
////-----------------------------------------------------------------------------
//void YBox::update( YTimeInterval dt )
//{
//    // interp
//    size.interp( dt );
//}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YSphere::render()
{
    // render
    glutSolidSphere( size.value, slices, stacks );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YSphere::update( YTimeInterval dt )
{
    // interp
    size.interp( dt );

}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YCone::render()
{
    // render
    glutSolidCone( base.value * size.value, height.value * size.value, slices, stacks );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YCone::update( YTimeInterval dt )
{
    // interp
    base.interp( dt );
    height.interp( dt );
    size.interp( dt );    
}



// static instantiation
std::vector<Vector3D> YPitchVortex::ourColorMap;




//-----------------------------------------------------------------------------
// name: YPitchVortex()
// desc: ...
//-----------------------------------------------------------------------------
YPitchVortex::YPitchVortex()
: m_currentAlphaGoal(0,0,5)
{
    m_currentIndex = 0;
    m_radius = 0;
    
    m_singlePitchMode = false;
    m_on = true;
}




//-----------------------------------------------------------------------------
// name: ~YPitchVortex()
// desc: ...
//-----------------------------------------------------------------------------
YPitchVortex::~YPitchVortex()
{
}




//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
void YPitchVortex::init( GLuint texture )
{
    // sanity check
    if( m_lightColumns.size() > 0 )
        return;
    
    // column
    YColumn * c = NULL;
    
    // add twelve light columns for pitch classes
    for( int i = 0; i < 12; i++ )
    {
        // instantiate
        c = new YColumn( 3, .15f );
        // taper
        c->updateTaper( 1.0f, 1.0f );
        // add to vector
        m_lightColumns.push_back( c );
        // init
        c->set( 1.0, 0.0, 1.0, .8, texture );
        // set layers
        c->numLayers = 10; // 5;
    }

    // colors
    m_lightColumns[0]->col.set( 1.0f, 0.5f, 0.0f );
    m_lightColumns[1]->col.set( 1.0f, 0.3f, 0.0f );
    m_lightColumns[2]->col.set( 1.0f, 0.0f, 0.0f );
    m_lightColumns[3]->col.set( 1.0f, 0.0f, 0.5f );
    m_lightColumns[4]->col.set( 1.0f, 0.0f, 1.0f );
    m_lightColumns[5]->col.set( 0.5f, 0.0f, 1.0f );
    m_lightColumns[6]->col.set( 0.0f, 0.0f, 1.0f );
    m_lightColumns[7]->col.set( 0.0f, 1.0f, 0.5f );
    m_lightColumns[8]->col.set( 0.0f, 1.0f, 0.0f );
    m_lightColumns[9]->col.set( 0.5f, 1.0f, 0.0f );
    m_lightColumns[10]->col.set( 1.0f, 1.0f, 0.0f );
    m_lightColumns[11]->col.set( 1.0f, .7f, 0.0f );
}




//-----------------------------------------------------------------------------
// name: pitch2color()
// desc: map pitch to color
//-----------------------------------------------------------------------------
const Vector3D & YPitchVortex::pitch2color( int pitch )
{
    // init if needed
    if( ourColorMap.size() == 0 )
    {
        // add 12 colors
        ourColorMap.push_back( Vector3D( 1.0f, 0.5f, 0.0f ) );
        ourColorMap.push_back( Vector3D( 1.0f, 0.3f, 0.0f ) );
        ourColorMap.push_back( Vector3D( 1.0f, 0.0f, 0.0f ) );
        ourColorMap.push_back( Vector3D( 1.0f, 0.0f, 0.5f ) );
        ourColorMap.push_back( Vector3D( 1.0f, 0.0f, 1.0f ) );
        ourColorMap.push_back( Vector3D( 0.5f, 0.0f, 1.0f ) );
        ourColorMap.push_back( Vector3D( 0.0f, 0.0f, 1.0f ) );
        ourColorMap.push_back( Vector3D( 0.0f, 1.0f, 0.5f ) );
        ourColorMap.push_back( Vector3D( 0.0f, 1.0f, 0.0f ) );
        ourColorMap.push_back( Vector3D( 0.5f, 1.0f, 0.0f ) );
        ourColorMap.push_back( Vector3D( 1.0f, 1.0f, 0.0f ) );
        ourColorMap.push_back( Vector3D( 1.0f, 0.7f, 0.0f ) );
    }
    
    // map pitch
    int actual = pitch % 12;
    
    // return
    return ourColorMap[actual];
}




//-----------------------------------------------------------------------------
// name: pitch2color()
// desc: map pitch to color
//-----------------------------------------------------------------------------
const Vector3D & YPitchVortex::pitch2color_f( float pitch )
{
    // round
    int actual = (int)(pitch + .5f);
    // return
    return pitch2color( actual );
}




//-----------------------------------------------------------------------------
// name: map()
// desc: ...
//-----------------------------------------------------------------------------
void YPitchVortex::setSinglePitchMode( bool singlePitchMode )
{
    m_singlePitchMode = singlePitchMode;
}




//-----------------------------------------------------------------------------
// name: show()
// desc: ...
//-----------------------------------------------------------------------------
void YPitchVortex::show()
{
    // on
    m_on = true;
}




//-----------------------------------------------------------------------------
// name: hide()
// desc: ...
//-----------------------------------------------------------------------------
void YPitchVortex::hide()
{
    // off
    m_on = false;
    // set goal
    m_currentAlphaGoal.update( 0 );
}




//-----------------------------------------------------------------------------
// name: map()
// desc: ...
//-----------------------------------------------------------------------------
void YPitchVortex::map( int pitch, float intensity )
{
    // sanity check
    if( m_lightColumns.size() == 0 || !m_on )
        return;
    
    // deactivate all other pitch columns
    // in single pitch mode
    if( m_singlePitchMode )
    {
        for( int i = 0; i < m_lightColumns.size(); i++ )
        {
            YColumn *col = m_lightColumns[i];
            col->active = false;
            col->alpha = 0.0;
        }
    }
    
    // previous index
    int prevIndex = m_currentIndex;
    // map pitch
    m_currentIndex = pitch % 12;
    // get column
    YColumn * c = m_lightColumns[m_currentIndex];
    // activate
    c->active = true;
    // set value and goal
    if( prevIndex != m_currentIndex )
    {
        // reset value
        m_currentAlphaGoal.value = 0;
    }
    // set goal
    m_currentAlphaGoal.update( intensity*1.75 );
    
    //    // set alpha
    //    c->alpha += intensity;
    //    // clamp
    //    if( c->alpha > 1.0 ) c->alpha = 1.0f;
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YPitchVortex::update( YTimeInterval dt )
{
    // interp
    m_currentAlphaGoal.interp( dt );
    
    // get column
    YColumn * c = m_lightColumns[m_currentIndex];
    // set alpha
    c->alpha = m_currentAlphaGoal.value;
    // clamp
    if( c->alpha > 1.0 ) c->alpha = 1.0f;
    
    // iterate through columns
    for( int i = 0; i < 12; i++ )
    {
        // get current
        YColumn * c = m_lightColumns[i];
        // update
        c->update( dt );
    }
}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YPitchVortex::render()
{
    // sanity check
    if( m_lightColumns.size() == 0 )
        return;
    
    YColumn * c = NULL;
    
    // iterate through columns
    for( int i = 0; i < 12; i++ )
    {
        // get current
        c = m_lightColumns[i];
        // check for active
        if( c->active )
        {
            // push
            glPushMatrix();
            // rotate
            glRotatef( i * -30, 0, 0, 1 );
            // translate
            glTranslatef( 0, m_radius, 0 );
            // rotate
            glRotatef( 90.0f, 1, 0, 0 );
            // render
            c->render();
            // pop
            glPopMatrix();
        }
    }
}




//-----------------------------------------------------------------------------
// name: YIris()
// desc: ...
//-----------------------------------------------------------------------------
YIris::YIris()
{
    // zero
    m_numBlades = 0;
    // identity
    m_R[0] = m_R[3] = 1.0f;
    m_R[1] = m_R[2] = 0.0f;
    // position
    m_position.set( 0.0f, 0.0f, 1.0f );
    // outline width
    m_outlineWidth = 1.0f;
}




//-----------------------------------------------------------------------------
// name: ~YIris()
// desc: ...
//-----------------------------------------------------------------------------
YIris::~YIris()
{
}




//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
void YIris::init( unsigned int N, GLfloat outlineWidth )
{
    // set
    m_numBlades = N;
    // find angle between each adjacent piece
    double angle = 360.0 / N;
    
    // rotation matrix (counter-clockwise)
    m_R[0] = ::cos( PI_OVER_180*angle );
    m_R[1] = ::sin( PI_OVER_180*angle );
    m_R[2] = -::sin( PI_OVER_180*angle );
    m_R[3] = ::cos( PI_OVER_180*angle );
    
    // zero
    memset( m_verts, 0, sizeof(GLfloat) * 9 );
    
    // outline width
    m_outlineWidth = outlineWidth;
}




// vertices
static GLfloat g_irisVerts[] = 
{
    -.1f, -.1f, 0.04f,
    .14f, .3f, -0.02f,
    -.1f, 0.8f, -0.06f
};




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YIris::render()
{
    // push
    glPushMatrix();
    
    // enable
    glEnable( GL_DEPTH_TEST );
    
    // enable
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    
    // vertex
    glVertexPointer( 3, GL_FLOAT, 0, g_irisVerts );
    // normal
    glNormalPointer( GL_FLOAT, 0, g_normals );
    
    // find angle between each adjacent piece
    GLfloat angle = 360.0f / m_numBlades;
    
    // position
    GLfloat pos = (1 - m_position.value);
    
    // rotate the whole thing
    glRotatef( pos * 240, 0, 0, 1 );
    // set the blade length
    // g_irisVerts[7] = .5 + pos / 3.0f;
    // set the blade width
    g_irisVerts[3] = .14f * 36 / m_numBlades + pos*.2;
    
    // color
    glColor4f( col.x, col.y, col.z, 1 );
    // enable lighting
    glEnable( GL_LIGHTING );
    // loop overs
    for( int i = 0; i < m_numBlades; i++ )
    {
        // push
        glPushMatrix();
        // rotate
        glRotatef( -angle * i, 0, 0, 1 );
        // translate from center
        glTranslatef( .8f, 0.0f, 0.0f );
        // rotate
        glRotatef( pos * 90, 0, 0, 1 );
        // triangle strip
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );
        // pop
        glPopMatrix();
    }
    
    // outline color
    glColor4f( .5, .5, .5, 1 );
    // translate a bit
    glTranslatef( 0, 0, .001 );
    // linewidth
    glLineWidth( m_outlineWidth );
    // no lighting
    glDisable( GL_LIGHTING );
    // no normal
    glDisableClientState( GL_NORMAL_ARRAY );
    // second pass for outline
    for( int i = 0; i < m_numBlades; i++ )
    {
        // push
        glPushMatrix();
        // rotate
        glRotatef( -angle * i, 0, 0, 1 );
        // translate from center
        glTranslatef( .8f, 0.0f, 0.0f );
        // rotate
        glRotatef( pos * 90, 0, 0, 1 );
        // triangle strip
        glDrawArrays( GL_LINE_LOOP, 0, 3 );
        // pop
        glPopMatrix();
    }
    
    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    
    // pop
    glPopMatrix();
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YIris::update( YTimeInterval dt )
{
    // std::cerr << "update: " << dtFixed << std::endl;
    // interp
    m_position.interp( dt );
}
