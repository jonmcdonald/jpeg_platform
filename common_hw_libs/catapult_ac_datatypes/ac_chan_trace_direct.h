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

/*! \file ac_chan_trace_direct.h
  \brief Catapult builtins
	This class is used by ac_channel to trace channels on inputs directly during SC Verivy runs so that
	channel data is available during replay. A "direct trace" is used only when the channel is optomized out
	of the RTL design.
*/

#ifndef _AC_CHAN_TRACE_DIRECT_H_
#define _AC_CHAN_TRACE_DIRECT_H_

#include "replay_incl.h"
#include <ac_trace_replay.h>
#include <mc_type_to_string.h>
#include <mc_container_types.h>
#include <deque>

using std::string;

template <class T>
class ac_field_tracer_base {
public:	
	virtual void trace_field(const T &data, TraceData *tData) = 0;
	virtual int num_items() = 0; // size of array or 1
};

template <class T> 
class ac_channel_tracer {
public:	
	ac_channel_tracer(const char *writer, const char *id) {
		// this will set it up in the trace file
		tdata = TraceReplay::inst()->get_trace_data(id, TRData::IN_CHAN);
		if (tdata) tdata->notify_bypass(writer);
	}
   virtual ~ac_channel_tracer() {}
	virtual void trace_read(const T &data) = 0;
	// should only be called by derived class
	virtual void add_field(ac_field_tracer_base<T> *fld) {} 

protected: //data
	TraceData *tdata;
};

template <class T> 
class ac_channel_simple_tracer : public ac_channel_tracer<T> {
public:	
	ac_channel_simple_tracer(const char *writer, const char *id) 
	: ac_channel_tracer<T>(writer, id)
	{}

	virtual void trace_read(const T &data) {
		string tmp;
		type_to_string(data, tmp);
		TraceReplay::inst()->trace_read(this->tdata, tmp);
	}
};

// special cae for preserved structures

template <class T, class P>
class ac_direct_field_tracer : public ac_field_tracer_base<P> {
public:	
	ac_direct_field_tracer( T(*func)(const P&) )
	: acc_func(func)
	{}
	
	virtual void trace_field(const P &strct, TraceData *tData) {
		T data = (*acc_func)(strct);
		string sdat;
		type_to_string(data, sdat);
		TraceReplay::inst()->trace_complex_item(tData, sdat);
	}
	
	virtual int num_items() { return 1; }
	
private:
	// function pointer to get this field from parent's data
	T (*acc_func)(const P&);
};

//////////////////////////////////
// ac_direct_array_field_tracer
/////////////////////////////////

template <class T, class P, int Sz>
class ac_direct_field_array_tracer : public ac_field_tracer_base<P> {
public:	
	ac_direct_field_array_tracer( void(*func)(const P&, mgc_sysc_ver_array1D<T,Sz>&)  )
	: acc_func(func)
	{}
	
	virtual void trace_field(const P &strct, TraceData *tData) {
		// call field access function to flatten array (if nec)
		(*acc_func)(strct, flat_array);
		for (int i = 0; i < Sz; i++) {
			T data = flat_array[i];
			// access function gives array pointer
			string sdat;
			type_to_string(data, sdat);
			TraceReplay::inst()->trace_complex_item(tData, sdat);
		}
	}
	
	virtual int num_items() { return Sz; }
	
protected:
	// function pointer to get this field from parent's data
	void (*acc_func)(const P&, mgc_sysc_ver_array1D<T,Sz>&);
	mgc_sysc_ver_array1D<T,Sz> flat_array;  //stores flattened array data from structure
};

//////////////////////////////////////////////

template <class T> 
class ac_channel_struct_tracer : public ac_channel_tracer<T> {

	typedef std::deque< ac_field_tracer_base<T> * > fieldlist;

public:	
	ac_channel_struct_tracer(const char *writer, const char *id)
	: ac_channel_tracer<T>(writer, id)
	, num_items(0)
	{}
	
	virtual void trace_read(const T &data) {
		// start trace for this struct
		TraceReplay::inst()->trace_complex_begin(this->tdata, num_items);			
		// loop through fields sending data to trace 
		typename fieldlist::iterator iter;
		for (iter = fields.begin(); iter != fields.end(); ++iter) {
			(*iter)->trace_field(data, this->tdata);
		}
		// end struct trace
		TraceReplay::inst()->trace_complex_end(this->tdata);
	}
	
	// should only be called by derived class
	virtual void add_field(ac_field_tracer_base<T> *fld) {
		num_items += fld->num_items();
		fields.push_back(fld);
	}
	
private: //data
	fieldlist fields;
	int num_items;
};

#endif
