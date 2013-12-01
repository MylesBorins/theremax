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
// name: y-charting.h
// desc: graphical objects for charting
//
// name: Ge Wang (ge@ccrma.stanford.edu)
// date: spring 2013
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_CHARTING_H__
#define __MCD_Y_CHARTING_H__

#include "y-entity.h"
#include <vector>
#include <string>




// forward reference
class YHistoBin;
class YLineNode;




//-----------------------------------------------------------------------------
// name: class YHistogram
// desc: basic histogram
//-----------------------------------------------------------------------------
class YHistogram : public YEntity
{
public:
    YHistogram();
    ~YHistogram();

public:
    // initialize the histogram
    void init( GLfloat width, GLfloat height, unsigned int numBins );
    // clean up
    void cleanup();
    // set max value
    void setMaxValue( GLfloat value );
    // get number of bins
    long numBins() const;
    // get a bin by index
    YHistoBin * bin( unsigned int index );
    // get width
    GLfloat getWidth() const { return m_width; }
    // get height
    GLfloat getHeight() const { return m_height; }
    
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();

protected:
    // width
    GLfloat m_width;
    // height
    GLfloat m_height;
    // vector of bins
    std::vector<YHistoBin *> m_bins;
};




//-----------------------------------------------------------------------------
// name: class YHistogramBin
// desc: base class for a histogram bin
//-----------------------------------------------------------------------------
class YHistoBin : public YEntity
{
public:
    YHistoBin();

public:
    // get/set max value
    virtual GLfloat getMaxValue() const;
    virtual void setMaxValue( GLfloat max );

    // get/set value
    virtual GLfloat getValue() const;
    virtual void setValue( GLfloat value );

    // get/set value as percentage [0, 1] of max
    virtual GLfloat getPercentValue() const;
    virtual void setPercentValue( GLfloat percentValue );
    
    // get/set value slew (default == 1)
    virtual GLfloat getValueSlew() const;
    virtual void setValueSlew( GLfloat slew );
    
    // get/set height
    virtual GLfloat getHeight() const;
    virtual void setHeight( GLfloat height );
    
    // get/set width
    virtual GLfloat getWidth() const;
    virtual void setWidth( GLfloat width );
    
    // get/set name
    virtual void setName( const std::string & n );
    virtual std::string getName() const;
    
    // set color
    virtual void setColor( const Vector3D & c );

public:
    void update( YTimeInterval dt );
    void render();

protected:
    void computeVertices();

protected:
    GLfloat m_maxValue;
    Vector3D m_iValue;
    Vector3D m_iHeight;
    Vector3D m_iWidth;
    iSlew3D m_iColor;
    std::string m_name;

protected:
    char m_strbuf[64];
    YText m_textLabel;
    YText m_textValue;

protected:
    // depth
    GLboolean useDepth;
    // bin vertices
    GLfloat vertices[8];
    // outline vertices
    GLfloat outline[8];
    // optional texture
    GLuint texture;
};




//-----------------------------------------------------------------------------
// name: class YLineChart
// desc: basic line chart
//-----------------------------------------------------------------------------
class YLineChart : public YEntity
{
public:
    YLineChart();
    ~YLineChart();
    
public:
    // initialize the histogram
    void init( GLfloat width, GLfloat height );
    // clean up
    void cleanup();
    
public:
    // add a control node mapped to a X value
    void addValue( GLfloat x, GLfloat y );
    // set X range for viewing
    void viewX( GLfloat min, GLfloat max );
    // set Y range for viewing
    void viewY( GLfloat min, GLfloat max );
    
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();

protected:
    // generate stuff to render
    void generate();
    
protected:
    // width
    GLfloat m_width;
    // height
    GLfloat m_height;
    
    // the list
    YLineNode * m_line;
    // left most node in viewable region (even if partial node)
    YLineNode * m_leftViewable;
    // right most node
    YLineNode * m_rightViewable;
    
    // boundaries
    Vector3D m_leftBound;
    Vector3D m_rightBound;
    Vector3D m_lowerBound;
    Vector3D m_upperBound;
};




//-----------------------------------------------------------------------------
// name: class YLineNode
// desc: line node
//-----------------------------------------------------------------------------
class YLineNode
{
public:
    // constructor
    YLineNode() { x = y = 0; prev = next = NULL; }
    
public:
    // where
    GLfloat x;
    // the value
    GLfloat y;
    
public:
    // linked list pointers
    YLineNode * prev;
    YLineNode * next;
};




#endif
