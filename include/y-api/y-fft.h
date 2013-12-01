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
// name: yfft.h
// desc: fft impl - based on CARL distribution and chuck_fft.*
//
// authors: code from San Diego CARL package
//          Ge Wang (ge@ccrma.stanford.edu)
// date: spring 2013
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_FFT_H__
#define __MCD_Y_FFT_H__


#include "x-audio.h"

// complex type
typedef struct { SAMPLE re ; SAMPLE im ; } complex;

// complex absolute value
#define cmp_abs(x) ( sqrt( (x).re * (x).re + (x).im * (x).im ) )

#define FFT_FORWARD 1
#define FFT_INVERSE 0

// c linkage
#if ( defined( __cplusplus ) || defined( _cplusplus ) )
  extern "C" {
#endif

// make the window
void hanning( SAMPLE * window, unsigned long length );
void hamming( SAMPLE * window, unsigned long length );
void blackman( SAMPLE * window, unsigned long length );
// apply the window
void apply_window( SAMPLE * data, SAMPLE * window, unsigned long length );

// real fft, N must be power of 2
void rfft( SAMPLE * x, long N, unsigned int forward );
// complex fft, NC must be power of 2
void cfft( SAMPLE * x, long NC, unsigned int forward );

// c linkage
#if ( defined( __cplusplus ) || defined( _cplusplus ) )
  }
#endif


#endif
