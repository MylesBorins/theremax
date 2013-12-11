//-----------------------------------------------------------------------------
// name: theremax-sim.cpp
// desc: bokeh visualization simulation
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "theremax-sim.h"
#include <iostream>
using namespace std;




// up to max fps steps per second, e.g., 1./60
#define STEPTIME (1.0 / getDesiredFrameRate())
// max sim step size in seconds
#define SIM_SKIP_TIME (.25)




//-------------------------------------------------------------------------------
// name: THEREMAXSim()
// desc: constructor
//-------------------------------------------------------------------------------
THEREMAXSim::THEREMAXSim()
{
    m_desiredFrameRate = 60;
    m_useFixedTimeStep = false;
    m_timeLeftOver = 0;
    m_simTime = 0;
    m_lastDelta = 0;
    m_first = true;
    m_isPaused = false;
}




//-------------------------------------------------------------------------------
// name: ~THEREMAXSim()
// desc: ...
//-------------------------------------------------------------------------------
THEREMAXSim::~THEREMAXSim()
{
    // nothing to do
}




//-------------------------------------------------------------------------------
// name: triggerUpdates()
// desc: trigger system wide update with time steps
//-------------------------------------------------------------------------------
void THEREMAXSim::systemCascade()
{
    // get current time (once per frame)
    XGfx::getCurrentTime( true );
    
    // Timing loop
    YTimeInterval timeElapsed = XGfx::getCurrentTime() - m_simTime;
    m_simTime += timeElapsed;
    
    // special case: first update
    if( m_first )
    {
        // set time just enough for one update
        timeElapsed = STEPTIME;
        // set flag
        m_first = false;
    }
    
    // clamp it
    if( timeElapsed > SIM_SKIP_TIME )
        timeElapsed = SIM_SKIP_TIME;
    
    // update it
    // check paused
    if( !m_isPaused )
    {
        if ( Globals::ChangeColor )
        {
            Globals::bgColor.update(Vector3D(0.5,0,.8));
            m_gfxRoot.recursiveSetColor(Globals::ourYellow);
            Globals::ChangeColor = false;
        }
        // update the world with a fixed timestep
        m_gfxRoot.updateAll( timeElapsed );
    }
    
    // redraw
    m_gfxRoot.drawAll();
    
    // set
    m_lastDelta = timeElapsed;
}





//-------------------------------------------------------------------------------
// pause the simulation
//-------------------------------------------------------------------------------
void THEREMAXSim::pause() { m_isPaused = true; }
//-------------------------------------------------------------------------------
// resume the simulation
//-------------------------------------------------------------------------------
void THEREMAXSim::resume() { m_isPaused = false; }
//-------------------------------------------------------------------------------
// get is paused
//-------------------------------------------------------------------------------
bool THEREMAXSim::isPaused() const { return m_isPaused; }
//-------------------------------------------------------------------------------
// set desired frame rate
//-------------------------------------------------------------------------------
void THEREMAXSim::setDesiredFrameRate( double frate ) { m_desiredFrameRate = frate; }
//-------------------------------------------------------------------------------
// get it
//-------------------------------------------------------------------------------
double THEREMAXSim::getDesiredFrameRate() const { return m_desiredFrameRate; }
//-------------------------------------------------------------------------------
// get the timestep in effect (fixed or dynamic)
//-------------------------------------------------------------------------------
YTimeInterval THEREMAXSim::delta() const
{ return m_lastDelta; }

