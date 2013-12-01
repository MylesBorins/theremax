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
// name: y-score-reader.h
// desc: midi score reader/data structures
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Tom Lieber (tom@smule.com)
//
//    date: Winter 2010
//    version: 1.1
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_SCORE_READER_H__
#define __MCD_Y_SCORE_READER_H__


#include <vector>
#include <deque>
#include <string>
#include <map>




//-----------------------------------------------------------------------------
// name: struct NoteEvent
// desc: a MIDI note event, parsed for start and end time, with simultaneous
//       notes combined into the 'simultaneous' linked list
//-----------------------------------------------------------------------------
struct NoteEvent
{
    // data
    unsigned short data1;
    unsigned short data2;
    unsigned short data3;
    // time data
    double time;
    double untilNext;
    // link to simultaneous events
    NoteEvent * simultaneous;
    // link to parent (could be self)
    NoteEvent * parent;
    
    // more data
    double startTime;
    double endTime;
    bool shouldBend;
    int bendAmount;
    int programChange;
    bool phrasemark;

    // constructor
    NoteEvent() : data1(0), data2(0), data3(0), time(0.0), untilNext(0.0),
        simultaneous(NULL), parent(NULL), startTime(0), endTime(0), shouldBend(false),
        bendAmount(0), programChange(0), phrasemark(false) { }
};




//-----------------------------------------------------------------------------
// name: struct LyricEvent
// desc: a MIDI lyric event, with "end" time given by the time of the next
//       event in the list
//-----------------------------------------------------------------------------
struct LyricEvent
{
    std::string lyric;
    // time data
    double time;
    double endTime;
    
    // constructor
    LyricEvent() : time(0), endTime(0) { }
};




// forward referendes
namespace stk { class MidiFileIn; }




//-----------------------------------------------------------------------------
// name: class YScoreReader()
// desc: loads a MIDI score
//-----------------------------------------------------------------------------
class YScoreReader
{
public:
    YScoreReader();
    ~YScoreReader();

public:
    bool load( const char * filename, float velScale = 0.0f );
    void cleanup();

public:
    // next advance to next note on
    bool nextNoteOn( long track );
    // get the current (only top event, no simultaneous)
    const NoteEvent * current( long track, long offset = 0 );
    // rewind to beginning
    void rewind();
    // get notes in a time window
    void getEvents( long track, double startTime, double endTime, std::vector<const NoteEvent *> & result,
                    bool includeSimultaneous = false );
    // isDone
    bool isDone( long track, double currTime );
    
public: // TODO: figure out what we can consolidate with the previous set of functions
    // next advance to next event
    bool next( long track, long numToAdvance = 1 );
    // get the next event (respect simultaneous)
    const NoteEvent * getNextEvent( long track, bool & isNewSet );
    
public:
    // get number of tracks
    long getNumTracks() const;
    // get number of events per track
    long getNumEvents( long track ) const;
    // get number of tracks with more than 0 events
    long getNumTracksNonZero() const;
    // get a vector of tracks indices that have more than 0 events
    const std::vector<long> & getTracksNonZero() const;
    // get BPM
    double getBPM() const;
    // get an entire track's note events
    const std::vector<NoteEvent *> & getNoteEvents( long track );
    // get an entire track's lyric events
    const std::vector<LyricEvent *> & getLyricEvents( long track );
    // get count
    long getCount( long track, const std::string & key );
    // get lowest note for a track
    long getLowestNote( long track, const NoteEvent *start );
    // get highest note for a track
    long getHighestNote( long track, const NoteEvent *start );
    
public: // TODO: these may as well be a separate object
    // push a event to remember
    void enqueue( const NoteEvent * event );
    // get the front event
    const NoteEvent * front() const;
    // dequeue the event
    void dequeue();
    // push to front
    void enqueue_front( const NoteEvent * event );
    
public: // track naming
    // sets track name
    void setTrackName( long track, const std::string & name );
    // gets track name ("" if not present)
    std::string getTrackName( long track );
    // gets the track with the given name (-1 if not present)
    long getTrackForName( const std::string & name );

protected: // for use while loading a track
    // load a track
    bool loadTrack( long track, std::vector<NoteEvent *> & data, std::vector<LyricEvent *> & lyricData );
    // apply control to existing event
    bool applyControl( long track, long data2, long data3, NoteEvent * e );
    // increment count for the given key (used for counting the number of notes, glissandi, etc)
    void incrementCount( long track, const std::string & key );
    // handle note events, managing m_activeNotes and setting the event's endTime
    void handleNoteOn( NoteEvent * e, long note );
    void handleNoteOff( long note, double time );

protected:
    // midi
    stk::MidiFileIn * m_midiFile;
    // vector of tracks of events
    std::vector< std::vector<NoteEvent *> > m_events;
    // vector of tracks of events
    std::vector< std::vector<LyricEvent *> > m_lyricEvents;
    // vector of indices
    std::vector<unsigned long> m_indices;
    // stack of events
    std::deque<const NoteEvent *> m_queue;
    // vector of event
    std::vector<const NoteEvent *> m_result;
    // search pointer
    std::vector<unsigned long> m_searchIndices;
    // count map
    std::vector< std::map< std::string, long > > m_countMaps;
    // HACK: scale the velocity (0-1)
    float m_velocity_scale;
    // track names
    std::map<std::string, long> m_nameToTrack;
    std::map<long, std::string> m_trackToName;
    // number of non-zero tracks
    long m_numNonZeroTracks;
    // vector of indices of non-zero tracks
    std::vector<long> m_nonZeroTrackIndices;

private: // used during track loading
    // active notes, map from MIDI note to event
    std::map<long, NoteEvent *> m_activeNotes;
};




#endif
