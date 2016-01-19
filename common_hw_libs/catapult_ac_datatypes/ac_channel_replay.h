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

#ifndef _AC_CHANNEL_REPLAY_H_
#define _AC_CHANNEL_REPLAY_H_

#include "replay_incl.h"
#include <ac_trace_replay.h>
#ifdef AC_REPLAY_STAND_ALONE

#define MC_NO_SYSC
#include <mc_container_types.h>

#include <cstdlib>

using std::string;

template<class Tclass, class Rclass>
void trace_to_type(Tclass in, Rclass &result) {
  result = (Rclass)in;
}

template<class Tclass>
void trace_to_type(Tclass in, bool &result) {
  result = (in != 0) ? true : false;
}

#if defined(__AC_INT_H)
template<class Tclass, int Twidth, bool Signed>
void trace_to_type(Tclass in, ac_int<Twidth,Signed> &result) {
	// casting is supported in ac_int
	result = (ac_int<Twidth,Signed>)in;
}
#endif

#if defined(__AC_FIXED_H)
template<class Tclass, int Twidth, int Ibits, bool Signed, ac_q_mode Qmode, ac_o_mode Omode>
void trace_to_type(Tclass in, ac_fixed<Twidth,Ibits,Signed,Qmode,Omode> &result) {
  static ac_int<Twidth,Signed> tmp;
  trace_to_type(in, tmp);
  result.set_slc(0, tmp);
}
#endif

// single element data reader

// bool?

template<class Tclass> 
struct tr_data_reader {
	int read_data(Tclass &ret, ReplayData *tdata)  
	{ 
		int res = TraceReplay::inst()->get_read(tdata);
		trace_to_type(tdata->get_read_data(), ret);
		return res;
	}
};

// arrays
template<class Tclass,int Tsize> 
struct tr_data_reader< mgc_sysc_ver_array1D<Tclass,Tsize> > {
	int read_data(mgc_sysc_ver_array1D<Tclass,Tsize>&ret, ReplayData *tdata)
	{
		int i = 0;
		for (; i < Tsize; i++) {
			if (TraceReplay::inst()->get_read(tdata) < 0){
				// if we give out of data  - leave now
				return -1;
			}
			trace_to_type(tdata->get_read_data(), ret[i]);
		}
		return i;
	}
};


class ac_channel_replay_base {
public:	
	virtual int get_size() = 0;
	virtual int data_count() = 0;
	virtual bool connected() = 0;	
	virtual bool empty_read_covered() = 0;
	virtual void report_channel_msg(
		TR_severity   sev,
		TR_error_type et,
		bool print_now = true // true = output during run, as well as in summary
	) = 0;
		// output messages regarding results
	virtual bool report_output_summary() = 0;
};

//////////////////////////////////////
// a base interface class typed to the channel
template < class Tclass > 
class ac_channel_replay : public ac_channel_replay_base {
public:	
	virtual int get_read(Tclass &data) = 0;
	virtual int get_output(Tclass &data) = 0;
	virtual bool equal(Tclass &cdata, Tclass &rtldata) = 0;
	virtual void report_channel_mismatch(
		const char           *chan_str,
		Tclass               &cdata,
		Tclass               &rtldata,
		TraceReplay::TR_event rw   // read or write?
	) = 0;
	
	virtual void report_field_mismatch(
		const char           *chan_str,
		Tclass               &cdata,
		Tclass               &rtldata
	) {};
	
	static ac_channel_replay< Tclass > *do_cast( ac_channel_replay_base *base_obj) {
		return static_cast <ac_channel_replay< Tclass > * > (base_obj);
    	}
};

// class used when there is a one to one mapping of ac_channels and pipes
template < class Tclass > 
class ac_channel_pipe_replay_base : public ac_channel_replay<Tclass> {
public:	
	
	virtual int get_size() {
		return tdata ? TraceReplay::inst()->get_size(tdata) : -1;
	}
	virtual int data_count() {
		return tdata ? TraceReplay::inst()->data_count(tdata) : -1;
	}
	
	virtual bool connected() { return tdata && (tdata->get_id() > TRData::NO_TRACE_ID); }

	virtual bool empty_read_covered() {
		return (tdata) ? tdata->empty_read_covered() : false;
	}	

	virtual void report_channel_msg(
		TR_severity   sev,
		TR_error_type et,
		bool print_now = true // true = output during run, as well as in summary
	)
	{
		// if not tdata - we have worse things to report
		if (tdata) TraceReplay::inst()->report_channel_msg(sev, et, tdata, print_now);
	}
	
		// output messages regarding results
	virtual bool report_output_summary()
	{
		return (tdata) ? TraceReplay::inst()->report_output_summary(tdata) : false;
	}
	
protected:

	//constructor
	ac_channel_pipe_replay_base(ReplayData *td)
	: ac_channel_replay<Tclass>()
	, tdata(td)
	{}
	
	// data
	ReplayData *tdata;
};

// class used when there is a one to one mapping of ac_channels and pipes and non-struct data
template < class Tclass > 
class ac_channel_pipe_replay : public ac_channel_pipe_replay_base<Tclass> {
public:	
	
	virtual int get_read(Tclass &data) {
		if (this->tdata == NULL) return -1;
		// even if result in that we had no data we must return something
		// tdata will return its last valid value
		return data_reader.read_data(data, this->tdata);
	}
	
	virtual int get_output(Tclass &data) { 
		if (this->tdata == NULL) return -1;
		// even if result in that we had no data we must return something
		// tdata will return its last valid value
		return data_reader.read_data(data, this->tdata);
	}
	
	virtual bool equal(Tclass &cdata, Tclass &rtldata) {
		return (cdata == rtldata) ? true : false;
	}

	static ac_channel_pipe_replay< Tclass > *construct_channel_replay_ports(const char *name, bool report = false) {
		// assume non blocking channel - for now, may be overridden
		ReplayData *tdata = TraceReplay::inst()->get_replay_data(name, TRData::NB_CHAN);
		if (tdata) {
			// if we actually found trace data, annouce that we are connected
			if (tdata->get_id() > TRData::NO_TRACE_ID) {
				// found trace ID, complete connection to data
				if (!TraceReplay::inst()->complete_channel_connection(tdata)) {
					return NULL;
				}
			}
			return (new ac_channel_pipe_replay(tdata));
		}
		else {
			// didn't find trace data under this name
			if (report) {
				TraceReplay::inst()->report_channel_msg(ReplayError::WARN, ReplayError::NO_TRACE_IN_FILE, name);
			}
			return NULL;
		}
	}

	static ac_channel_replay< Tclass > *get_output_replay_port(const char *name) {
		ReplayData *tdata = TraceReplay::inst()->get_replay_data(name, TRData::OUTPUT);
		if (tdata) {
			return (new ac_channel_pipe_replay(tdata));
		}
		else return NULL;
	}

	virtual void report_channel_mismatch(
		const char           *chan_str,
		Tclass               &cdata,
		Tclass               &rtldata,
		TraceReplay::TR_event rw   // read or write?
	)
	{
		if (this->tdata) {
			string cd;
			string rd;
			
			tr_data_printer<Tclass>::type_to_print(cdata, cd);
			tr_data_printer<Tclass>::type_to_print(rtldata, rd);
			
			this->tdata->data_error_count++;
			TraceReplay::inst()->report_channel_mismatch(chan_str, cd, rd, rw);
		}
		// should never happen
		else assert (false);
	}
	
	virtual void report_field_mismatch(
		const char           *fname,
		Tclass               &cdata,
		Tclass               &rtldata
	)
	{
		if (this->tdata) {
			string cd;
			string rd;
			
			tr_data_printer<Tclass>::type_to_print(cdata, cd);
			tr_data_printer<Tclass>::type_to_print(rtldata, rd);
			TraceReplay::inst()->report_field_mismatch(fname, cd, rd);
		}
		// should never happen
		else assert (false);
	}
	
private:

	//constructor
	ac_channel_pipe_replay(ReplayData *td)
	: ac_channel_pipe_replay_base<Tclass>(td)
	{}
	
private: //data

	tr_data_reader<Tclass> data_reader;
};

// declare generic function template for constructing replay connection, primitive defined specifically in ac_chan_replay_prim.h
// thisis done so we can generate specific versions for user defined structs - defined in generated file replay_channel_struct.h
template < class Tclass > 
extern void get_channel_replay_ports( ac_channel_replay< Tclass > *&client_ports, const char *name);

#if defined(__AC_INT_H)
template<int Twidth, bool Signed>
extern void get_channel_replay_ports( ac_channel_replay< ac_int<Twidth, Signed > > *&client_ports, const char *name){
	client_ports = ac_channel_pipe_replay<ac_int<Twidth, Signed > >::construct_channel_replay_ports(name, true);
}
#endif
#if defined(__AC_FIXED_H)
template<int Twidth, int Ibits, bool Signed, ac_q_mode Qmode, ac_o_mode Omode>
extern void get_channel_replay_ports( ac_channel_replay< ac_fixed<Twidth,Ibits,Signed,Qmode,Omode> > *&client_ports, const char *name){
	client_ports = ac_channel_pipe_replay<ac_fixed<Twidth,Ibits,Signed,Qmode,Omode> >::construct_channel_replay_ports(name, true);
}
#endif

#else 
// stub for trace mode (should derive this in a clean way later
// was implementeed for single process - see pre 9a version
template < class Tclass > 
class ac_channel_replay {
public:	
	int get_size() {
		return -1;
	}
	int peek_size() {
		return -1;
	}
	
	// get actual number of datum remaining in trace 
	int data_count() {
		return -1;
	}
	
	bool equal(Tclass &cdata, Tclass &rtldata) {
		return false;
	}
	
	int get_read(Tclass &data) {
		return -1;
	}
	
	int get_output(Tclass &data) { 
		return -1;
	}
	
	static ac_channel_replay< Tclass > *get_output_replay_port(const char *name) {
		return NULL;
	}

	bool connected() { return false; }
	
	void report_channel_msg(
		TR_severity   sev,
		TR_error_type et,
		bool print_now = true // true = output during run, as well as in summary
	)
	{
	}
	
	bool empty_read_covered() {
		return false;
	}
	
	void report_channel_mismatch(
		const char *chan_str,
		Tclass &cdata,
		Tclass &rtldata,
		TraceReplay::TR_event rw   // read or write?
	)
	{
	}
	
		// output messages regarding results
	bool report_output_summary()
	{
		return false;
	}
	
private: //data

};


template < class Tclass > 
extern void get_channel_replay_ports( ac_channel_replay< Tclass > *&client_ports, const char *name) {}

#endif
#endif
