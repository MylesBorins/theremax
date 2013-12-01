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
// name: y-charting.cpp
// desc: graphical object for charting
//
// name: Ge Wang (ge@ccrma.stanford.edu)
// date: spring 2013
//-----------------------------------------------------------------------------
#include "y-charting.h"




// texture coords
static const GLfloat g_texCoords[] = {
    0, 0,
    1, 0,
    0, 1,
    1, 1
};




//-----------------------------------------------------------------------------
// name: YHistogram()
// desc: constructor
//-----------------------------------------------------------------------------
YHistogram::YHistogram()
{
    // default
    m_width = 0;
    m_height = 0;
}




//-----------------------------------------------------------------------------
// name: YHistogram()
// desc: constructor
//-----------------------------------------------------------------------------
YHistogram::~YHistogram()
{
    cleanup();
}




//-----------------------------------------------------------------------------
// name: init()
// desc: initialize
//-----------------------------------------------------------------------------
void YHistogram::init( GLfloat width, GLfloat height, unsigned int numBins )
{
    // clean up first
    cleanup();

    // bin pointer
    YHistoBin * bin = NULL;
    // bin width
    GLfloat binWidth = numBins ? width / numBins : 0;

    // add bins
    for( long i = 0; i < numBins; i++ )
    {
        // the new bin
        bin = new YHistoBin();
        // set height
        bin->setHeight( height );
        // set width
        bin->setWidth( binWidth * .985 );
        // set location
        bin->loc.set( binWidth/2 + i * binWidth, 0, 0 );
        // set color
        bin->setColor( Vector3D( 1,1,1 ) );
        // add it
        m_bins.push_back( bin );
        // add as child
        this->addChild( bin );
    }
}




//-----------------------------------------------------------------------------
// name: cleanup()
// desc: ...
//-----------------------------------------------------------------------------
void YHistogram::cleanup()
{
    // remove all children (do this first since it sets state on children)
    this->removeAllChildren();

    // iterate
    for( long i = 0; i < m_bins.size(); i++ )
    {
        // delete
        SAFE_DELETE( m_bins[i] );
    }
    
    // clear the vector
    m_bins.clear();
}




//-----------------------------------------------------------------------------
// name: setMaxValue()
// desc: set max value
//-----------------------------------------------------------------------------
void YHistogram::setMaxValue( GLfloat value )
{
    // iterate
    for( long i = 0; i < m_bins.size(); i++ )
    {
        // set for each
        m_bins[i]->setMaxValue( value );
    }
}




//-----------------------------------------------------------------------------
// name: numBins()
// desc: ...
//-----------------------------------------------------------------------------
long YHistogram::numBins() const
{
    return m_bins.size();
}




//-----------------------------------------------------------------------------
// name: bin()
// desc: return a bin
//-----------------------------------------------------------------------------
YHistoBin * YHistogram::bin( unsigned int index )
{
    // sanity check
    if( index >= m_bins.size() ) return NULL;

    // return the indexed bin
    return m_bins[index];
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YHistogram::update( YTimeInterval dt )
{
    // nothing to do
}




//-----------------------------------------------------------------------------
// name: render()
// desc: ..
//-----------------------------------------------------------------------------
void YHistogram::render()
{
    // nothing to do
}




//-----------------------------------------------------------------------------
// name: YHistoBin()
// desc: constructor
//-----------------------------------------------------------------------------
YHistoBin::YHistoBin()
{
    // default
    m_maxValue = 1;
    // default to 0 with slew of 1
    m_iValue.set( 0, 0, 2 );
    // height default
    m_iHeight.set( 0, .25, 5 );
    // width default
    m_iWidth.set( 0, .25, 5 );
    // set slew
    m_iColor.setSlew( 4.0f );
    
    // set text
    m_textValue.setWidth(1.0);
    m_textLabel.setWidth(1.0);
    
    // add
    this->addChild( &m_textLabel );
    this->addChild( &m_textValue );

    // state
    useDepth = false;
    texture = 0;
    
    // initial
    computeVertices();
}




//-----------------------------------------------------------------------------
// name: getMaxValue()
// desc: get max value
//-----------------------------------------------------------------------------
GLfloat YHistoBin::getMaxValue() const
{
    return m_maxValue;
}




//-----------------------------------------------------------------------------
// name: setMaxValue()
// desc: set max value
//-----------------------------------------------------------------------------
void YHistoBin::setMaxValue( GLfloat max )
{
    m_maxValue = max;
}




//-----------------------------------------------------------------------------
// name: getValue()
// desc: get value
//-----------------------------------------------------------------------------
GLfloat YHistoBin::getValue() const
{
    return m_iValue.goal;
}




//-----------------------------------------------------------------------------
// name: setValue()
// desc: set value
//-----------------------------------------------------------------------------
void YHistoBin::setValue( GLfloat value )
{
    // update goal
    m_iValue.update( value );
    // flash value
    m_iColor.set( Vector3D(1,1,1) );
}




//-----------------------------------------------------------------------------
// name: getPercentValue()
// desc: get value as percentage of max
//-----------------------------------------------------------------------------
GLfloat YHistoBin::getPercentValue() const
{
    // sanity check
    if( m_maxValue == 0 ) return 0;
    // return as percentage
    return m_iValue.goal / m_maxValue;
}




//-----------------------------------------------------------------------------
// name: setPercentValue()
// desc: set value as percentage of max
//-----------------------------------------------------------------------------
void YHistoBin::setPercentValue( GLfloat percentValue )
{
    // set goal
    m_iValue.update( m_maxValue * percentValue );
}




//-----------------------------------------------------------------------------
// name: getValueSlew()
// desc: ...
//-----------------------------------------------------------------------------
GLfloat YHistoBin::getValueSlew() const
{
    return m_iValue.slew;
}




//-----------------------------------------------------------------------------
// set value slew (default == 1)
//-----------------------------------------------------------------------------
void YHistoBin::setValueSlew( GLfloat slew )
{
    m_iValue.slew = slew;
}




//-----------------------------------------------------------------------------
// name: getHeight()
// desc: get height
//-----------------------------------------------------------------------------
GLfloat YHistoBin::getHeight() const
{
    return m_iHeight.goal;
}




//-----------------------------------------------------------------------------
// name: setHeight()
// desc: set height
//-----------------------------------------------------------------------------
void YHistoBin::setHeight( GLfloat height )
{
    m_iHeight.update( height );
}




//-----------------------------------------------------------------------------
// name: getWidth()
// desc: get width
//-----------------------------------------------------------------------------
GLfloat YHistoBin::getWidth() const
{
    return m_iWidth.goal;
}




//-----------------------------------------------------------------------------
// name: setWidth()
// desc: set width
//-----------------------------------------------------------------------------
void YHistoBin::setWidth( GLfloat width )
{
    m_iWidth.update( width );
}




//-----------------------------------------------------------------------------
// name: setName()
// desc: ...
//-----------------------------------------------------------------------------
void YHistoBin::setName( const std::string & n )
{
    // set name
    m_name = n;
}




//-----------------------------------------------------------------------------
// name: getName()
// desc: ...
//-----------------------------------------------------------------------------
std::string YHistoBin::getName() const
{
    return m_name;
}




//-----------------------------------------------------------------------------
// name: setColor()
// desc: set color
//-----------------------------------------------------------------------------
void YHistoBin::setColor( const Vector3D & c )
{
    // set goal
    m_iColor.update( c );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void YHistoBin::update( YTimeInterval dt )
{
    // interpolate
    m_iValue.interp( dt );
    m_iWidth.interp( dt );
    m_iHeight.interp( dt );
    m_iColor.interp( dt );
    
    // set
    this->col = m_iColor.actual();
    
    // compute vertices
    computeVertices();
}




//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void YHistoBin::render()
{
    // check
    if( !active ) return;

    // disable lighting
    glDisable( GL_LIGHTING );

    // depth
    if( useDepth ) glEnable( GL_DEPTH_TEST );
    else glDisable( GL_DEPTH_TEST );

    // disable writing
    glDepthMask( GL_FALSE );
    
    // if texture
    if( texture )
    {
        // enable texture mapping
        glEnable( GL_TEXTURE_2D );
        // enable blending
        glEnable( GL_BLEND );
        // blend function
        // glBlendFunc( GL_ONE, GL_ONE );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        // bind the texture
        glBindTexture( GL_TEXTURE_2D, texture );
    }
    
    // enable
    glEnableClientState( GL_VERTEX_ARRAY );
    // vertex
    glVertexPointer( 2, GL_FLOAT, 0, vertices );

    // if texture
    if( texture )
    {
        // texture
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        // texture coordinate
        glTexCoordPointer( 2, GL_FLOAT, 0, g_texCoords );
    }

    // triangle strip
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    
    // if texture
    if( texture )
    {
        // disable texture
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    }

    // color
    glColor4f( .5, .5, .5, alpha );
    // line width
    glLineWidth( 1 );
    // outline
    glVertexPointer( 2, GL_FLOAT, 0, outline );
    // line strip
    glDrawArrays( GL_LINE_LOOP, 0, 4 );

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );

    // if texture
    if( texture )
    {
        // disable
        glDisable( GL_TEXTURE_2D );
        glDisable( GL_BLEND );
    }

    // enable writing
    glDepthMask( GL_TRUE );
}




//-----------------------------------------------------------------------------
// name: computeVertices()
// desc: ...
//-----------------------------------------------------------------------------
void YHistoBin::computeVertices()
{
    // compute half width
    GLfloat half_width = m_iWidth.value / 2;
    // compute height
    GLfloat height = m_iHeight.value * m_iValue.value / m_maxValue;

    // compute text width
    GLfloat text_width = m_iWidth.value * 3;
    m_textValue.sca.set( text_width, text_width, text_width );
    sprintf( m_strbuf, "%i", (int)(m_iValue.value + .9) );
    m_textValue.set( m_strbuf );
    m_textValue.setCenterLocation( Vector3D( 0, height + m_iWidth.value * .3, 0 ) );
    m_textLabel.sca.set( text_width, text_width, text_width );
    sprintf( m_strbuf, "%s", m_name.c_str() );
    m_textLabel.set( m_strbuf );
    m_textLabel.setCenterLocation( Vector3D( 0, -.05 - .1*m_iWidth.value, 0 ) );

    // generate vertices
    vertices[0] = -half_width;
    vertices[1] = 0;
    vertices[2] = half_width;
    vertices[3] = 0;
    vertices[4] = -half_width;
    vertices[5] = height;
    vertices[6] = half_width;
    vertices[7] = height;
    
    // generate outline vertices
    outline[0] = -half_width;
    outline[1] = 0;
    outline[2] = half_width;
    outline[3] = 0;
    outline[4] = half_width;
    outline[5] = height;
    outline[6] = -half_width;
    outline[7] = height;
}




//-----------------------------------------------------------------------------
// name: YLineChart()
// desc: ...
//-----------------------------------------------------------------------------
YLineChart::YLineChart()
{
    // default
    m_width = 0;
    m_height = 0;

    // zero out list
    m_line = NULL;
    m_leftViewable = NULL;
    m_rightViewable = NULL;

    // set view bounds
    m_leftBound.updateSet( 0, 1 );
    m_rightBound.updateSet( 1, 1 );
    m_lowerBound.updateSet( 0, 1 );
    m_upperBound.updateSet( 1, 1 );
}




//-----------------------------------------------------------------------------
// name: YLineChart()
// desc: ...
//-----------------------------------------------------------------------------
YLineChart::~YLineChart()
{
    // clean up
}




//-----------------------------------------------------------------------------
// name: init()
// desc: initialize the histogram
//-----------------------------------------------------------------------------
void YLineChart::init( GLfloat width, GLfloat height )
{
    m_width = width;
    m_height = height;
}




//-----------------------------------------------------------------------------
// name: cleanup()
// desc: clean up
//-----------------------------------------------------------------------------
void YLineChart::cleanup()
{
    // pointers
    YLineNode * curr = m_line;
    YLineNode * next = NULL;
    
    // iterate
    while( curr != NULL )
    {
        // get next
        next = curr->next;
        // delete current
        SAFE_DELETE( curr );
        // move to next
        curr = next;
    }
    
    // zero out the list
    m_line = NULL;
}




//-----------------------------------------------------------------------------
// name: addValue()
// desc: add a control node mapped to a X value
//-----------------------------------------------------------------------------
void YLineChart::addValue( GLfloat x, GLfloat y )
{
    // pointers
    YLineNode * curr = m_line;
    YLineNode * next = NULL;

    // find
    while( curr != NULL )
    {
        // get next
        next = curr->next;
        
        // check for duplicate
        if( curr->x == x )
        {
            // update
            curr->y = y;
            // done
            break;
        }

        // check
        if( curr->x < x && ( !next || next->x > x ) )
        {
            // make new node
            YLineNode * node = new YLineNode();
            // set
            node->x = x;
            node->y = y;
            
            // link
            node->prev = curr;
            node->next = next;
            
            // re-link
            curr->next = node;
            if( next ) next->prev = node;
            
            // TODO: update left and right viewable
            
            // done
            break;
        }
        
        // advance
        curr = next;
    }

    // zero out the list
    m_line = NULL;
}




//-----------------------------------------------------------------------------
// name: viewX()
// desc: set X range for viewing
//-----------------------------------------------------------------------------
void YLineChart::viewX( GLfloat min, GLfloat max )
{
    // set it
    m_leftBound.update( min );
    m_rightBound.update( max );
}




//-----------------------------------------------------------------------------
// name: viewY()
// desc: set Y range for viewing
//-----------------------------------------------------------------------------
void YLineChart::viewY( GLfloat min, GLfloat max )
{
    // set it
    m_lowerBound.update( min );
    m_upperBound.update( max );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: update
//-----------------------------------------------------------------------------
void YLineChart::update( YTimeInterval dt )
{
    // interpolate
    m_leftBound.interp( dt );
    m_rightBound.interp( dt );
    m_lowerBound.interp( dt );
    m_upperBound.interp( dt );
}




//-----------------------------------------------------------------------------
// name: render()
// desc: render
//-----------------------------------------------------------------------------
void YLineChart::render()
{
}




//-----------------------------------------------------------------------------
// name: generate()
// desc: generate stuff to render
//-----------------------------------------------------------------------------
void YLineChart::generate()
{
    
}
