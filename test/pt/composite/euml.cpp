// Copyright 2010 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include "benchmark.hpp"
// back-end
#include <boost/msm/back/state_machine.hpp>
// front-end
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front::euml;

BOOST_MSM_EUML_EVENT(play)
BOOST_MSM_EUML_EVENT(end_pause)
BOOST_MSM_EUML_EVENT(stop)
BOOST_MSM_EUML_EVENT(pause_)
BOOST_MSM_EUML_EVENT(open_close)
BOOST_MSM_EUML_EVENT(cd_detected)
BOOST_MSM_EUML_EVENT(previous_song)
BOOST_MSM_EUML_EVENT(next_song)

BOOST_MSM_EUML_ACTION(start_next_song){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(start_prev_song){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(start_playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(open_drawer){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(close_drawer){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(store_cd_info){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(stop_playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(pause_playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(resume_playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(stop_and_open){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(stopped_again){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};

BOOST_MSM_EUML_ACTION(Log_No_Transition){template <class FSM, class Event> void operator()(Event const &e, FSM &, int state){}};

BOOST_MSM_EUML_STATE((), Song1)
BOOST_MSM_EUML_STATE((), Song2)
BOOST_MSM_EUML_STATE((), Song3)

// clang-format off
BOOST_MSM_EUML_TRANSITION_TABLE((
        Song2         == Song1          + next_song       / start_next_song,
        Song1         == Song2          + previous_song   / start_prev_song,
        Song3         == Song2          + next_song       / start_next_song,
        Song2         == Song3          + previous_song   / start_prev_song
    ),transition_table_playing )
// clang-format on

// create a state machine "on the fly"
BOOST_MSM_EUML_DECLARE_STATE_MACHINE((transition_table_playing,                    // STT
                                      init_ << Song1,                              // Init State
                                      no_action,                                   // Entry
                                      no_action,                                   // Exit
                                      attributes_ << no_attributes_,               // Attributes
                                      configure_ << no_exception << no_msg_queue,  // configuration
                                      Log_No_Transition                            // no_transition handler
                                      ),
                                     playing_)  // fsm name

BOOST_MSM_EUML_STATE((), Empty)
BOOST_MSM_EUML_STATE((), Open)
BOOST_MSM_EUML_STATE((), Stopped)
msm::back::state_machine<playing_> Playing;
BOOST_MSM_EUML_STATE((), Paused)

// clang-format off
BOOST_MSM_EUML_TRANSITION_TABLE((
    Playing ==  Stopped + play        / start_playback ,
    Open    ==  Stopped + open_close  / open_drawer    ,
    Stopped ==  Stopped + stop        / stopped_again  ,
    Empty   ==  Open    + open_close  / close_drawer   ,
    Open    ==  Empty   + open_close  / open_drawer    ,
    Stopped ==  Empty   + cd_detected / store_cd_info  ,
    Stopped ==  Playing + stop        / stop_playback  ,
    Paused  ==  Playing + pause_       / pause_playback ,
    Open    ==  Playing + open_close  / stop_and_open  ,
    Playing ==  Paused  + end_pause   / resume_playback,
    Stopped ==  Paused  + stop        / stop_playback  ,
    Open    == Paused  + open_close   / stop_and_open
),transition_table)
// clang-format on

// create a state machine "on the fly"
BOOST_MSM_EUML_DECLARE_STATE_MACHINE((transition_table,                            // STT
                                      init_ << Empty,                              // Init State
                                      no_action,                                   // Entry
                                      no_action,                                   // Exit
                                      attributes_ << no_attributes_,               // Attributes
                                      configure_ << no_exception << no_msg_queue,  // configuration
                                      Log_No_Transition                            // no_transition handler
                                      ),
                                     player_)  // fsm name

typedef msm::back::state_machine<player_> player;

int main() {
  player p2;
  p2.start();

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1'000; ++i) {
      p2.process_event(open_close);
      p2.process_event(open_close);
      p2.process_event(cd_detected);
      p2.process_event(play);
      for (auto j = 0; j < 1'000; ++j) {
        p2.process_event(next_song);
        p2.process_event(next_song);
        p2.process_event(previous_song);
        p2.process_event(previous_song);
      }

      p2.process_event(pause_);
      // go back to Playing
      p2.process_event(end_pause);
      p2.process_event(pause_);
      p2.process_event(stop);
      // event leading to the same state
      p2.process_event(stop);
      p2.process_event(open_close);
      p2.process_event(open_close);
    }
  });
  benchmark_memory_usage(p2);
}
