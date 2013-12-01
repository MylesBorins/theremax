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
// name: y-echo.h
// name: feedback echo effect
//
// authors: adapted from Mike Rotondo's FeedbackEcho
//          Ge Wang (added effect mix, slew, multichan)
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_ECHO_H__
#define __MCD_Y_ECHO_H__

#include "DelayL.h"
#include "x-vector3d.h"




//-----------------------------------------------------------------------------
// name: class YEcho
// desc: feedback echo effect
//-----------------------------------------------------------------------------
class YEcho
{
public:
    // constructor
    YEcho( int srate, float maxDelay = 2.0, float delaySeconds = 1.0,
             float feedbackCoefficient = 0.5, float fxMix = .25 );
    // destructor
    virtual ~YEcho();

public:
    // fill buffer
    virtual int synthesize2( float * buffer, unsigned int numFrames );
    // toggle
    virtual void toggle( bool onOff );

public:
    void setDelay( int chan, float inSeconds );
    void setFeedback( float coef );
    void setFxMix( float mix );

private:
    // delay
    float m_srate;
    int m_numChannels;
    stk::DelayL * m_delay;
    float * m_delaySamples;
    float m_maxDelay;
    float m_feedbackCoefficient;
    float m_fxMix;

    // slews
    Vector3D * m_iDelay;
    Vector3D m_iFeedback;
    Vector3D m_iFxMix;
    
    bool m_onOff;
};




#endif
