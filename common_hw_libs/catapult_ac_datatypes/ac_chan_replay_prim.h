////////////////////////////////////////////////////////////////////////////////
// Catapult Synthesis
// 
// © 2009 Mentor Graphics Corporation
//       All Rights Reserved
// 
// This document contains information that is proprietary to Mentor Graphics  
// Corporation. The original recipient of this document may duplicate this  
// document in whole or in part for internal business purposes only, provided  
// that this entire notice appears in all copies. In duplicating any part of  
// this document, the recipient agrees to make every reasonable effort to  
// prevent the unauthorized use and distribution of the proprietary information.
////////////////////////////////////////////////////////////////////////////////

// This file is part of catapult, not part of the design.

/*! \file ac_channel_replay.h
  \brief Catapult builtins
	THere are two versions of this class here. One for and integrated TRace/Replay system.
	And onefor the stand-alone C++ replay testbench.
	This class is queried by ac_channel for traced data (for RTL simulation) during execution of read and size methods.
*/

#ifndef _AC_MANIFOLD_CHAN_REPLAY_PRIM_H_
#define _AC_MANIFOLD_CHAN_REPLAY_PRIM_H_

#include <ac_channel_replay.h>

template<class Tclass>
void get_chan_ports_builtin( ac_channel_replay< Tclass > *&client_ports, const char *name){
	client_ports = ac_channel_pipe_replay<Tclass >::construct_channel_replay_ports(name, true);
}

template <> extern void get_channel_replay_ports<bool>( ac_channel_replay<bool> *&client_ports, const char *name) {get_chan_ports_builtin<bool>(client_ports, name);}
template <> extern void get_channel_replay_ports<char>( ac_channel_replay<char> *&client_ports, const char *name) {get_chan_ports_builtin<char>(client_ports, name);}
template <> extern void get_channel_replay_ports<unsigned char>( ac_channel_replay<unsigned char> *&client_ports, const char *name) {get_chan_ports_builtin<unsigned char>(client_ports, name);}
template <> extern void get_channel_replay_ports<signed char>( ac_channel_replay<signed char> *&client_ports, const char *name) {get_chan_ports_builtin<signed char>(client_ports, name);}

template <> extern void get_channel_replay_ports<short>( ac_channel_replay<short> *&client_ports, const char *name) {get_chan_ports_builtin<short>(client_ports, name);}
template <> extern void get_channel_replay_ports<unsigned short>( ac_channel_replay<unsigned short> *&client_ports, const char *name) {get_chan_ports_builtin<unsigned short>(client_ports, name);}

template <> extern void get_channel_replay_ports<int>( ac_channel_replay<int> *&client_ports, const char *name) {get_chan_ports_builtin<int>(client_ports, name);}
template <> extern void get_channel_replay_ports<unsigned int>( ac_channel_replay<unsigned int> *&client_ports, const char *name) {get_chan_ports_builtin<unsigned int>(client_ports, name);}

template <> extern void get_channel_replay_ports<long>( ac_channel_replay<long> *&client_ports, const char *name) {get_chan_ports_builtin<long>(client_ports, name);}
template <> extern void get_channel_replay_ports<unsigned long>( ac_channel_replay<unsigned long> *&client_ports, const char *name) {get_chan_ports_builtin<unsigned long>(client_ports, name);}

template <> extern void get_channel_replay_ports<float>( ac_channel_replay<float> *&client_ports, const char *name) {get_chan_ports_builtin<float>(client_ports, name);}
template <> extern void get_channel_replay_ports<double>( ac_channel_replay<double> *&client_ports, const char *name) {get_chan_ports_builtin<double>(client_ports, name);}

template <> extern void get_channel_replay_ports<long long>( ac_channel_replay<long long> *&client_ports, const char *name) {get_chan_ports_builtin<long long>(client_ports, name);}
template <> extern void get_channel_replay_ports<unsigned long long>( ac_channel_replay<unsigned long long> *&client_ports, const char *name) {get_chan_ports_builtin<unsigned long long>(client_ports, name);}

#endif
