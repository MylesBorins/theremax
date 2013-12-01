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
// name: y-echo.cpp
// name: feedback echo effect
//
// authors: Mike Rotondo (created)
//          Ge Wang (added effect mix, slew)
//-----------------------------------------------------------------------------
#include "y-echo.h"
#include "x-fun.h"




//-----------------------------------------------------------------------------
// name: YEcho()
// desc: constructor
//-----------------------------------------------------------------------------
YEcho::YEcho( int srate, float maxDelay, float delaySeconds,
              float feedbackCoefficient, float fxMix )
{ 
    m_srate = srate;
    int numChannels = 2;
    
    // sanity check
    assert( numChannels > 0 );
    assert( numChannels <= 2 );
    
    // num channels
    m_numChannels = numChannels;
    
    // allocate
    m_delay = new stk::DelayL[m_numChannels];
    m_delaySamples = new float[m_numChannels];
    m_iDelay = new Vector3D[m_numChannels];

    // set max delay
    m_maxDelay = maxDelay;    
    // set feedback
    m_feedbackCoefficient = feedbackCoefficient;
    // set fxMix
    m_fxMix = fxMix;
    
    // iterate
    for( int i = 0; i < m_numChannels; i++ )
    {
        // set max
        m_delay[i].setMaximumDelay( m_srate * m_maxDelay );
        // set delay
        m_delaySamples[i] = delaySeconds * m_srate;
        // set delay
        m_delay[i].setDelay( m_delaySamples[i] );
        // clear delay
        m_delay[i].clear();
        // set slew
        m_iDelay[i].set( delaySeconds, delaySeconds, 5 );
    }

    // set slew
    m_iFeedback.set( m_feedbackCoefficient, m_feedbackCoefficient, 1 );
    m_iFxMix.set( m_fxMix, m_fxMix, 1 );

    // toggle
    toggle( false );
}




//-----------------------------------------------------------------------------
// name: ~YEcho()
// desc: destructor
//-----------------------------------------------------------------------------
YEcho::~YEcho()
{
    // clean up
    delete [] m_delay;
    delete [] m_delaySamples;
    delete [] m_iDelay;
    
    // zero
    m_delay = NULL;
    m_delaySamples = NULL;
    m_iDelay = NULL;
}




//-----------------------------------------------------------------------------
// name: setDelay()
// desc: set the delay by channel
//-----------------------------------------------------------------------------
void YEcho::setDelay( int chan, float inSeconds )
{
    // sanity check
    assert( chan >= 0 );
    assert( chan < m_numChannels );

    // clamp
    float v = XFun::clampf( inSeconds, 0, m_maxDelay );
    // set
    m_iDelay[chan].update( v );
}




//-----------------------------------------------------------------------------
// name: setFeedback()
// desc: set the feedback coefficient
//-----------------------------------------------------------------------------
void YEcho::setFeedback( float coef )
{
    // clamp
    float v = XFun::clampf( coef, 0, 1 );
    // set
    m_iFeedback.update( v );
}




//-----------------------------------------------------------------------------
// name: setFxMix()
// desc: set the dry/wet mix
//-----------------------------------------------------------------------------
void YEcho::setFxMix( float mix )
{
    // clamp
    float v = XFun::clampf( mix, 0, 1 );
    // set
    m_iFxMix.update( v );
}




//-----------------------------------------------------------------------------
// name: synthesize2()
// desc: do it!
//-----------------------------------------------------------------------------
int YEcho::synthesize2( float * buffer, unsigned int numFrames )
{
    // format
    int frames = numFrames;
    int channels = m_numChannels;
    float * data = (float *)buffer;

    // iterate
    for( int i = 0; i < frames; i++ )
    {
        float input_sample = 0.0;
        float output_sample = 0.0;

        // iterate
        for( int j = 0; j < channels; j++ )
        {
            // interp
            m_iDelay[j].interp( 1.0f / m_srate );
            // set the delay
            m_delaySamples[j] = m_srate * m_iDelay[j].value;
            m_delay[j].setDelay( m_delaySamples[j] );
            // get input
            input_sample = data[i * channels + j];
            // get delay output
            output_sample = m_delay[j].nextOut();
            // feedback
            m_delay[j].tick( input_sample + output_sample * m_feedbackCoefficient );

            // mix
            data[i * channels + j] = (1 - m_fxMix)*input_sample + m_fxMix*output_sample;
        }
    }
    
    // return frames
    return numFrames;
}




//-----------------------------------------------------------------------------
// name: toggle()
// desc: turn on and off
//-----------------------------------------------------------------------------
void YEcho::toggle( bool onOff )
{
    m_onOff = onOff;
}
