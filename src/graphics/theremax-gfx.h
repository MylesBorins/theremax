//-----------------------------------------------------------------------------
// name: theremax-gfx.h
// desc: graphics stuff for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __THEREMAX_GFX_H__
#define __THEREMAX_GFX_H__

#include "x-def.h"
#include "x-gfx.h"
#include "x-fun.h"
#include <string>


// entry point for graphics
bool theremax_gfx_init( int argc, const char ** argv );
void theremax_gfx_loop();
void theremax_about();
void theremax_keys();
void theremax_help();
void theremax_usage();
void theremax_endline();
void theremax_line();




#endif
