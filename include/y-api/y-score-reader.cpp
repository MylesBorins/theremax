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
// name: y-score-reader.cpp
// desc: reads song scores
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//    date: Winter 2010
//    version: 1.0
//-----------------------------------------------------------------------------
#include <iostream>
#include "y-score-reader.h"
#include "MidiFileIn.h"

using namespace stk;
using namespace std;




#pragma mark - MIDI helpers


//-----------------------------------------------------------------------------
// name: isNoteOff()
// desc: ...
//-----------------------------------------------------------------------------
static bool isNoteOff( std::vector<unsigned char> & shuttle )
{
    if( shuttle.size() >= 1 && shuttle[0] >> 4 == 0x8 ) // note off event
        return true;
    
    if( shuttle.size() >= 3 && shuttle[0] >> 4 == 0x9 && shuttle[2] == 0 ) // note on event with velocity 0
        return true;
    
    return false;
}




//-----------------------------------------------------------------------------
// name: isControl()
// desc: ...
//-----------------------------------------------------------------------------
static bool isControl( std::vector<unsigned char> & shuttle )
{
    if( shuttle.size() >= 1 && ( shuttle[0] & 0xf0 ) == 0xb0 )
        return true;
    
    return false;
}




//-----------------------------------------------------------------------------
// name: isMeta()
// desc: ...
//-----------------------------------------------------------------------------
static bool isMeta( std::vector<unsigned char> & shuttle )
{
    if( shuttle.size() >= 1 && shuttle[0] == 0xff )
        return true;
    
    return false;
}




//-----------------------------------------------------------------------------
// name: isProgram()
// desc: ...
//-----------------------------------------------------------------------------
static bool isProgram( std::vector<unsigned char> & shuttle )
{
    if( shuttle.size() >= 1 && ( shuttle[0] >= 0xC0 && shuttle[0] <= 0xCF ) )
        return true;

    return false;
}




//-----------------------------------------------------------------------------
// name: YScoreReader()
// desc: constructor
//-----------------------------------------------------------------------------
YScoreReader::YScoreReader()
    : m_midiFile(NULL), m_velocity_scale(0.0f), m_numNonZeroTracks(0)
{
}




//-----------------------------------------------------------------------------
// name: ~YScoreReader()
// desc: destructor
//-----------------------------------------------------------------------------
YScoreReader::~YScoreReader()
{
    // clean up
    cleanup();
}




//-----------------------------------------------------------------------------
// name: cleanup()
// desc: reclaim
//-----------------------------------------------------------------------------
void YScoreReader::cleanup()
{
    // clean up
    if( m_midiFile )
    {
        // pointer to event
        NoteEvent * e = NULL;
        NoteEvent * next = NULL;

        // clean up the tracks
        for( long i = 0; i < m_midiFile->getNumberOfTracks(); i++ )
        {
            // for each track
            for( long j = 0; j < m_events[i].size(); j++ )
            {
                // get track
                e = m_events[i][j];
                // delete simultaneous events
                while( e )
                {
                    // get next
                    next = e->simultaneous;
                    // delete current
                    delete e;
                    // advance
                    e = next;
                }
            }
            
            // clear the vectors
            m_events[i].clear();
        }

        // delete midiFile
        delete m_midiFile;
        m_midiFile = NULL;
        
        // clean the vector
        m_events.clear();
        m_countMaps.clear();
        m_nonZeroTrackIndices.clear();

        // clear the queue
        while( m_queue.size() > 0 )
            m_queue.pop_front();
    }
}




//-----------------------------------------------------------------------------
// name: load()
// desc: load a MIDI file
//-----------------------------------------------------------------------------
bool YScoreReader::load( const char * path, float velScale )
{
    // sanity check
    if( m_midiFile ) cleanup();
    
    // open midi file
    try
    {
        m_midiFile = new MidiFileIn( path );
        // print a little information about the file.
        std::cerr << "[score-reader]: MIDI file: " << path << std::endl;
        std::cerr << "[score-reader]:   | format = " << m_midiFile->getFileFormat() << std::endl;
        std::cerr << "[score-reader]:   | tracks = " << m_midiFile->getNumberOfTracks() << std::endl;
        std::cerr << "[score-reader]:   | seconds / ticks = " << m_midiFile->getTickSeconds() << std::endl;
        std::cerr << "[score-reader]:   | BPM = " << m_midiFile->getBPM() << std::endl;
    }
    catch ( StkError & e )
    {
        // you might want to do something more useful here.
        std::cerr << "[score-reader]: oh no, there was a problem loading: " 
                  << path << std::endl;
        // delete
        cleanup();
        
        return false;
    }
    
    // set velocity scale
    m_velocity_scale = velScale;
    // reset
    m_numNonZeroTracks = 0;
    // clear
    m_nonZeroTrackIndices.clear();

    // allocate element for vector
    m_events.resize( m_midiFile->getNumberOfTracks() );
    m_lyricEvents.resize( m_midiFile->getNumberOfTracks() );
    m_indices.resize( m_midiFile->getNumberOfTracks() );
    m_countMaps.resize( m_midiFile->getNumberOfTracks() );
    // iterate and set
    for( long i = 0; i < m_midiFile->getNumberOfTracks(); i++ )
    {
        // load up the arrays
        loadTrack( i, m_events[i], m_lyricEvents[i] );
        // log
        string name = getTrackName( i );
        printf( "[score-reader]: track-%ld (%s): events: %lu low: %lu high: %lu\n",
                i, name.c_str(), m_events[i].size(), getLowestNote( i, NULL ), getHighestNote( i, NULL ) );
        // set indice
        m_indices[i] = 0;
        // add search pointer
        m_searchIndices.push_back( 0 );
        // count non zero
        if( m_events[i].size() )
        {
            // increment
            m_numNonZeroTracks++;
            // append
            m_nonZeroTrackIndices.push_back( i );
        }
    }

    return true;
}




//-----------------------------------------------------------------------------
// name: getNumTracks()
// desc: get the number of tracks
//-----------------------------------------------------------------------------
long YScoreReader::getNumTracks() const
{
    if( m_midiFile == NULL ) return 0;
    return m_midiFile->getNumberOfTracks();
}




//-----------------------------------------------------------------------------
// name: getNumTracksNonZero()
// desc: get number of tracks with more than 0 zero
//-----------------------------------------------------------------------------
long YScoreReader::getNumTracksNonZero() const
{
    if( m_midiFile == NULL ) return 0;
    return m_numNonZeroTracks;
}




//-----------------------------------------------------------------------------
// name: getTracksNonZero()
// desc: get a vector of tracks indices that have more than 0 events
//-----------------------------------------------------------------------------
const std::vector<long> & YScoreReader::getTracksNonZero() const
{
    return m_nonZeroTrackIndices;
}




//-----------------------------------------------------------------------------
// name: getBPM()
// desc: get BPM
//-----------------------------------------------------------------------------
double YScoreReader::getBPM() const
{
    if( m_midiFile == NULL ) return 0;
    return m_midiFile->getBPM();
}




//-----------------------------------------------------------------------------
// name: getLowestNote()
// desc: get lowest note for a track
//-----------------------------------------------------------------------------
long YScoreReader::getLowestNote( long track, const NoteEvent * start )
{
    int lowestNote = 128;
    int startingPoint = 0;

    // get the starting point
    if( start != NULL )
    {
        for( int i = 0; i < m_events[track].size(); i++ )
        {
            if( m_events[track].at(i) == start )
            {
                startingPoint = i + 1;
                break;
            }
        }
    }
    
    for( int i = startingPoint; i < m_events[track].size(); i++ )
    {
        if( m_events[track].at(i)->data2 < lowestNote )
            lowestNote = m_events[track].at(i)->data2;
        
        if( m_events[track].at(i)->phrasemark ) break;
    }
    
    return lowestNote < 128 ? lowestNote : 0;
}




//-----------------------------------------------------------------------------
// name: getHighestNote()
// desc: get highest note for a track
//-----------------------------------------------------------------------------
long YScoreReader::getHighestNote( long track, const NoteEvent *start )
{
    long highestNote = 0;
    long startingPoint = 0;

    // get the starting point
    if( start != NULL )
    {
        for( long i = 0; i < m_events[track].size(); i++ )
        {
            if( m_events[track].at(i) == start )
            {
                startingPoint = i+1;
                break;
            }
        }
    }
    
    for( long i = startingPoint; i < m_events[track].size(); i++ )
    {
        if( m_events[track].at(i)->data2 > highestNote )
            highestNote = m_events[track].at(i)->data2;
        
        if( m_events[track].at(i)->phrasemark ) break;
    }
    
    return highestNote;
}




//-----------------------------------------------------------------------------
// name: current()
// desc: return last thing read on track
//-----------------------------------------------------------------------------
const NoteEvent * YScoreReader::current( long track, long offset )
{
    // sanity check
    if( !m_midiFile || track >= m_events.size() )
    {
        std::cerr << "[score-reader]: cannot access current data!" << std::endl;
        return NULL;
    }
    
    // the new index
    long index = m_indices[track] + offset;
    // check it
    if( index < 0 || index >= m_events[track].size() ) return NULL;
    // return it
    return m_events[track][index];
}




//-----------------------------------------------------------------------------
// name: next()
// desc: advance to an event on a track
//-----------------------------------------------------------------------------
bool YScoreReader::next( long track, long numToAdvance )
{
    // sanity check
    if( !m_midiFile || track >= m_events.size() )
    {
        std::cerr << "[score-reader]: cannot access current data!" << std::endl;
        return false;
    }
    
    // the next index
    long index = m_indices[track] + numToAdvance;
    // check it
    if( index < 0 ) index = 0;
    else if( index >= m_events[track].size() ) index = m_events[track].size();
    // update
    m_indices[track] = index;
    
    // return
    if( index >= m_events[track].size() ) return false;
    return true;
}




//-----------------------------------------------------------------------------
// name: nextNoteOn()
// desc: advance to an event on a track
//-----------------------------------------------------------------------------
bool YScoreReader::nextNoteOn( long track )
{
    // sanity check
    if( !m_midiFile || track >= m_events.size() )
    {
        std::cerr << "[score-reader]: cannot access current data!" << std::endl;
        return false;
    }
    
    // event
    const NoteEvent * e = NULL;
    long index = 0;
    
    // go
    do {
        // the next index
        index = m_indices[track] + 1;
        // check it
        if( index < 0 ) index = 0;
        else if( index >= m_events[track].size() ) index = m_events[track].size();
        // update
        m_indices[track] = index;
        // break
        if( index >= m_events[track].size() ) break;
        // set the e
        e = m_events[track][index];
    } while( (e->data1 >> 4 != 0x9) || (e->data3 == 0) );

    // return
    if( index >= m_events[track].size() ) return false;
    return true;
}




//-----------------------------------------------------------------------------
// name: rewind()
// desc: go to begining
//-----------------------------------------------------------------------------
void YScoreReader::rewind()
{
    // sanity check
    if( !m_midiFile ) return;
    
    // set all indices to 0
    for( long i = 0; i < m_midiFile->getNumberOfTracks(); i++ )
    {
        // clear
        m_indices[i] = 0;
        m_searchIndices[i] = 0;
    }
}




//-----------------------------------------------------------------------------
// name: getEvents()
// desc: get notes in a time window
//-----------------------------------------------------------------------------
void YScoreReader::getEvents( long track, double startTime, double endTime,
                              vector<const NoteEvent *> & result,
                              bool includeSimultaneous )
{
    // clear the result
    result.clear();

    // sanity check
    if( !m_midiFile || track >= m_events.size() )
    {
#ifndef __SM_DISTRIBUTION__
        std::cerr << "[score-reader]: cannot read track: " << track << "!" << std::endl;
#endif
        return;
    }
    
    // get the event
    const NoteEvent * searchPtr = current( track, m_searchIndices[track] );
    
    // advance the search pointer
    while( searchPtr && searchPtr->endTime < startTime )
    {
        // TODO: this doesn't handle the case where simultaneous note ends later than parent

        // increment index
        m_searchIndices[track]++;
        // get event
        searchPtr = current( track, m_searchIndices[track] );
    }
    
    // check
    if( searchPtr == NULL )
        return;
    
    // the index
    unsigned long searchIndex = m_searchIndices[track];
    
    // scan for events within the window
    while( searchPtr )
    {
        // within window - this is some sexy logic, thanks Jieun and Tom
        // ge: added the <= for endTime
        if( searchPtr->endTime > startTime && searchPtr->startTime <= endTime )
        {
            // add to vector
            result.push_back( searchPtr );
        }

        // check
        if( includeSimultaneous && searchPtr->simultaneous )
        {
            // move to simultaneous - could have different end time
            searchPtr = searchPtr->simultaneous;
        }
        else // to the next parent (top level event)
        {
            // advance search index
            searchIndex++;
            // get event
            searchPtr = current( track, searchIndex );
        }
    }
}




//-----------------------------------------------------------------------------
// name: isDone()
// desc: is done
//-----------------------------------------------------------------------------
bool YScoreReader::isDone( long track, double currTime )
{
    // sanity check
    if( track < 0 || track >= m_events.size() )
        return true;

    // number of events on a track
    long numEvents = m_events[track].size();
    // check time against last element
    if( numEvents <= 0 )
        return true;

    return m_events[track][numEvents-1]->endTime < currTime;
}




//-----------------------------------------------------------------------------
// name: applyControl()
// desc: applies control data to an existing event
//-----------------------------------------------------------------------------
bool YScoreReader::applyControl( long track, long data2, long data3, NoteEvent * e )
{
    // sanity check
    if( !e ) return false;
    if( data2 < 0 || data2 > 127 ) return false;
    if( data3 < 0 || data3 > 127 ) return false;
    
    // flag
    bool found = false;
    
    if( data2 == 0x52 )
    {
        // gliss
        e->shouldBend = true;
        found = true;
        // increment
        incrementCount( track, "bend" );
        e->bendAmount = (int)data3;
    }
    
    if( data2 == 0x53 )
    {
        // gliss
        e->phrasemark = true;
        found = true;
        // increment
        incrementCount( track, "phrasemark" );
    }
    
    return found;
}




//-----------------------------------------------------------------------------
// name: loadTrack()
// desc: load a track from file
//-----------------------------------------------------------------------------
bool YScoreReader::loadTrack( long track, std::vector<NoteEvent *> & data, std::vector<LyricEvent *> & lyricData )
{
    // sanity check
    if( !m_midiFile || track >= m_events.size() )
    {
#ifndef __SM_DISTRIBUTION__
        std::cerr << "[score-reader]: cannot read track: " << track << "!" << std::endl;
#endif
        return false;
    }
    
    // clear out the vectors
    data.clear();
    // clear out active MIDI notes
    m_activeNotes.clear();
    // a vector to hold the return information
    std::vector<unsigned char> shuttle;
    // piano event pointers
    NoteEvent * e = NULL;
    LyricEvent * le = NULL;
    // last event
    NoteEvent * prev_e = NULL;
    LyricEvent * prev_le = NULL;
    // parent event
    NoteEvent * parent = NULL;
    
    int currentProgram = 27; // program defaults to electric gutiar (27) in case it is not set in the score
    
    // load next on the track
    try {
        // seconds accum
        double secondsAccum = 0;
        // loop
        while( true )
        {
            // get the next MIDI event
            unsigned long ticks = m_midiFile->getNextEvent( &shuttle, (unsigned int)track );
            
            // accumulate time
            secondsAccum += ticks * m_midiFile->getTickSeconds();
            
            // MIDI message spec: http://www.srm.com/qtma/davidsmidispec.html

            // check size (done?)
            if( shuttle.size() == 0 ) break;
            
            if( isNoteOff( shuttle ) )
            {
                // do note off
                handleNoteOff( shuttle[1], secondsAccum );
                continue;
            }
            else if( isControl( shuttle ) )
            {
                // apply control to previous
                applyControl( track, shuttle[1], shuttle[2], prev_e );
                continue;
            }
            else if( isMeta( shuttle ) )
            {
                // format: http://www.recordingblogs.com/sa/Wiki/tabid/88/Default.aspx?topic=MIDI+meta+messages
                // shuttle[1]: message type
                // shuttle[2]: number of bytes in custom data part
                // shuttle[3]: start of custom data part
                
                switch( shuttle[1] )
                {
                    case 1: // text
                    {
                        string text = string( (char *)&shuttle[3] /* name */, shuttle[2] /* length */ );
                        break;
                    }
                    case 3: // track name
                    {
                        string name = string( (char *)&shuttle[3] /* name */, shuttle[2] /* length */ );
                        setTrackName( track, name );
                        break;
                    }
                    case 5: // lyric
                    {
                        string lyric = string( (char *)&shuttle[3] /* name */, shuttle[2] /* length */ );
                        
                        le = new LyricEvent;
                        le->lyric = lyric;
                        le->time = le->endTime = secondsAccum;
                        
                        if( prev_le )
                        {
                            prev_le->endTime = secondsAccum;
                        }
                        
                        lyricData.push_back( le );
                        
                        prev_le = le;
                        
                        break;
                    }
                }
                continue;
            }

            // store a program change in an event
            else if( isProgram( shuttle ) )
            {
                if( shuttle[1] >= 0 ) 
                    currentProgram = shuttle[1];
                continue;
            }

            // continue if it's anything but a note on
            if( shuttle[0] >> 4 != 0x9 || shuttle[2] == 0 ) continue;

            // instantiate
            e = new NoteEvent();
            // fill
            e->data1 = (unsigned short)shuttle[0];
            e->data2 = (unsigned short)shuttle[1];
            e->data3 = (unsigned short)shuttle[2];
            // check
            if( (m_velocity_scale > 0) && e->data3 )
            {
                // scale
                e->data3 = e->data3 + m_velocity_scale * (127 - e->data3);
                // clamp
                if( e->data3 > 127 ) e->data3 = 127;
            }

            // fill the until next for prev
            if( prev_e ) prev_e->untilNext = e->time - prev_e->startTime;
            // advance time
            e->startTime = e->time = secondsAccum;
            // simultaneous
            e->simultaneous = NULL;
            // store current program
            e->programChange = currentProgram;
            
            // simultaneous or in series?
            if( prev_e != NULL && e->time == prev_e->time )
            {
                // sim
                prev_e->simultaneous = e;
                // point to parent
                e->parent = parent;
            }
            else
            {                
                // increment
                incrementCount( track, "note" );
                // series
                data.push_back( e );
                // save as parent
                parent = e->parent = e;
            }
            
            // add to lookup
            handleNoteOn( e, e->data2 );
            
            // update
            prev_e = e; 
        }
    }
    catch( StkError & )
    {
        // you might want to do something more useful here.
        std::cerr << "[score-reader]: oh no, there was a read problem!" << std::endl;
        
        // clear out active MIDI notes
        m_activeNotes.clear();
        
        return false;
    }
    
    // clear out active MIDI notes
    m_activeNotes.clear();
    
    return true;
}




//-----------------------------------------------------------------------------
// name: enqueue()
// desc: enqueue a event to remember
//-----------------------------------------------------------------------------
void YScoreReader::enqueue( const NoteEvent * event )
{
    // TODO: synchronization?
    // while( event != NULL )
    // {
    m_queue.push_back( event );
    //    event = event->simultaneous;
    // }
}




//-----------------------------------------------------------------------------
// name: front()
// desc: get the front event
//-----------------------------------------------------------------------------
const NoteEvent * YScoreReader::front() const
{
    if( m_queue.size() == 0 ) return NULL;
    else return m_queue.front();
}




//-----------------------------------------------------------------------------
// name: dequeue()
// desc: dequeue the event
//-----------------------------------------------------------------------------
void YScoreReader::dequeue()
{
    if( m_queue.size() > 0 )
        m_queue.pop_front();
}




//-----------------------------------------------------------------------------
// name: enqueue_front()
// desc: enqueue to front
//-----------------------------------------------------------------------------
void YScoreReader::enqueue_front( const NoteEvent * e )
{
    m_queue.push_front( e );
}




//-----------------------------------------------------------------------------
// name: getNextEvent()
// desc: get the next event
//-----------------------------------------------------------------------------
const NoteEvent * YScoreReader::getNextEvent( long track, bool & isNewSet )
{
    const NoteEvent * e = NULL;
    isNewSet = false;
    // see if there is last event
    if( ( e = front() ) )
    {
        // pop it
        dequeue();
        // get simulaneous
        e = e->simultaneous;
    }
    // see if still NULL
    if( !e )
    {
        // get current
        e = current( track );
        // set flag
        isNewSet = true;
    }
    // set the next event
    enqueue( e );
    
    return e;
}




//-----------------------------------------------------------------------------
// name: getNumEvents()
// desc: get number of events on a track
//-----------------------------------------------------------------------------
long YScoreReader::getNumEvents( long track ) const
{
    // sanity check
    if( track < 0 || track >= m_events.size() )
        return 0;
    
    // return the number of elements
    return m_events[track].size();
}




//-----------------------------------------------------------------------------
// name: getNoteEvents()
// desc: get an entire track's note events
//-----------------------------------------------------------------------------
static vector<NoteEvent *> s_nullNoteVector;
const vector<NoteEvent *> & YScoreReader::getNoteEvents( long track )
{
    // sanity check
    if( track < 0 || track >= m_events.size() )
        return s_nullNoteVector;
    
    return m_events[track];
}




//-----------------------------------------------------------------------------
// name: getLyricEvents()
// desc: get an entire track's lyric events
//-----------------------------------------------------------------------------
static vector<LyricEvent *> s_nullLyricVector;
const vector<LyricEvent *> & YScoreReader::getLyricEvents( long track )
{
    // sanity check
    if( track < 0 || track >= m_lyricEvents.size() )
        return s_nullLyricVector;
    
    return m_lyricEvents[track];
}




//-----------------------------------------------------------------------------
// name: getCount()
// desc: return a count associated with key
//-----------------------------------------------------------------------------
long YScoreReader::getCount( long track, const std::string & key )
{
    // sanity check
    if( track < 0 || track >= m_events.size() ) return 0;
    // find
    std::map< std::string, long >::iterator iter = m_countMaps[track].find( key );
    // not found
    if( iter == m_countMaps[track].end() ) return 0;
    // return value
    return iter->second;
}




//-----------------------------------------------------------------------------
// name: incrementCount()
// desc: increment a count associated with key
//-----------------------------------------------------------------------------
void YScoreReader::incrementCount( long track, const std::string & key )
{
    // sanity check
    if( track < 0 || track >= m_events.size() ) return;

    // find
    std::map< std::string, long >::iterator iter = m_countMaps[track].find( key );

    if( iter == m_countMaps[track].end() )
    {
        // if not found, add
        m_countMaps[track][key] = 1;
    }
    else
    {
        // if found, increment
        iter->second++;
    }
}




//-----------------------------------------------------------------------------
// name: handleNoteOn()
// desc: insert a note on into table
//-----------------------------------------------------------------------------
void YScoreReader::handleNoteOn( NoteEvent * e, long note )
{
    // sanity check
    if( !e )
        return;
    
    // check to see if occupied
    if( m_activeNotes[note] != NULL )
    {
        // treat as a note-off for the same note
        handleNoteOff( note, e->time );
    }
    
    // insert
    m_activeNotes[note] = e;
}




//-----------------------------------------------------------------------------
// name: handleNoteOff()
// desc: insert a note off into table
//-----------------------------------------------------------------------------
void YScoreReader::handleNoteOff( long note, double time )
{
    // get the piano event
    NoteEvent * e = m_activeNotes[note];
    // check
    if( !e )
        return;
    
    // set end time
    e->endTime = time;
    
    // set
    m_activeNotes[note] = NULL;
}




//-----------------------------------------------------------------------------
// name: setTrackName()
// desc: sets track name
//-----------------------------------------------------------------------------
void YScoreReader::setTrackName( long track, const std::string & name )
{
    // x-map it!
    m_trackToName[track] = name;
    m_nameToTrack[name] = track;
}




//-----------------------------------------------------------------------------
// name: getTrackName()
// desc: gets track name ("" if not present)
//-----------------------------------------------------------------------------
string YScoreReader::getTrackName( long track )
{
    map<long, string>::iterator itr = m_trackToName.find( track );
    
    if( itr == m_trackToName.end() )
        return "";
    
    return itr->second;
}




//-----------------------------------------------------------------------------
// name: getTrackForName()
// desc: gets the track with the given name (-1 if not present)
//-----------------------------------------------------------------------------
long YScoreReader::getTrackForName( const std::string & name )
{
    // look up the track index by name
    map<string, long>::iterator itr = m_nameToTrack.find( name );
    
    // didn't find
    if( itr == m_nameToTrack.end() )
        return -1;
    
    // make sure the track actually exists
    if( itr->second >= m_events.size() )
        return -1;
    
    return itr->second;
}
