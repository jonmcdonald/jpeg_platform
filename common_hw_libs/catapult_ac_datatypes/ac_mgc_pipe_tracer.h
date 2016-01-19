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

/*! \file ac_mgc_pipe_tracer.h
  \brief Catapult builtins
  An object used in the TraceReplay system to monitor mgc_pipe state (on clock edges) and trace the results
  for use in a C++ replay environment.
*/

#ifndef INCLUDED_AC_MGC_PIPE_TRACER_H
#define INCLUDED_AC_MGC_PIPE_TRACER_H

#include <tlm.h>
#include "ac_trace_replay.h"
#include <ac_int.h>
#include "mc_typeconv.h"
#include <deque>

using std::deque;

// todo - will need to adjust for masked bits
template<int Twidth, int Rwidth>
void vector_to_type(const sc_lv<Twidth> &in, bool issigned, ac_int<Rwidth,true> *result) {
	ac_int<Twidth, true> tmp;
    vector_to_type(in, issigned, &tmp); 
    *result = tmp;
}

template<int Twidth, int Rwidth>
void vector_to_type(const sc_lv<Twidth> &in, bool issigned, ac_int<Rwidth,false> *result) {
	ac_int<Twidth, false> tmp;
    vector_to_type(in, issigned, &tmp); 
    *result = tmp;
}

template<typename T, int S, int W>
SC_MODULE(ac_mgc_pipe_tracer)
{
public:

	sc_in< bool > clock;                
	sc_in< sc_logic > reset;
	
	int loc_sz; // buf size
	TraceData *tData;

	// probes for each input channel - note where lower bits have been optomized out
	// todo - is size always the same width as data?
	sc_in< sc_logic >      probe_req_size;
	// NOTE: size is fixed size of 32 bit now (be ware that this could change
	sc_in< sc_lv< S > >    probe_size;
	sc_in< sc_logic >      probe_ldin;
	sc_in< sc_logic >      probe_vdin;
	sc_in< sc_lv< W > >    probe_din;

	int lshift;
	T mask;

	// Constructor
	SC_HAS_PROCESS(ac_mgc_pipe_tracer);
	ac_mgc_pipe_tracer(
	      const sc_module_name& name,
		  int ls,
		  T    m
   )
    :  clock("clock")
      , reset("reset")   
	  , loc_sz(0)
	  , tData(NULL)
	  , probe_req_size("probe_req_size")
      , probe_size("probe_size")
	  , probe_ldin("probe_ldin")
	  , probe_vdin("probe_vdin")
	  , probe_din("probe_din")
	  , lshift(ls)
	  , mask(m)
	{
		SC_METHOD(eval);
		sensitive << clock.pos() << reset;
		dont_initialize();
	}
	
	void setupTraceFile(const char *tname)
	{
		tData = TraceReplay::inst()->get_trace_data(tname, TRData::NB_CHAN);
	}
	  
	private: 
		void eval()
		{
			// NOTE: We are registering size to conform to its behavior
			// in the design - so trace size before accessing it	
			if (probe_req_size.read() == '1') {
				if (tData) TraceReplay::inst()->trace_size(tData, loc_sz);
		    }
			// accessing size signal
			vector_to_type(probe_size.read(), false, &loc_sz);
			
			if ((probe_ldin.read() == '1')&&(probe_vdin.read() == '1')) {
				T data;
				vector_to_type(probe_din.read(), false, &data);
				data = (data << lshift) | mask;
				//  do type conversion here
				string sdat;
				type_to_string(data, sdat);
				if (tData) TraceReplay::inst()->trace_read(tData, sdat);
			}
		}
};

// virtual base so client needn't know field types
template<int STR_W>
class ac_field_tracer {
public:
	virtual void trace_field(sc_lv< STR_W > str_vec, int offset, TraceData *tData) = 0;
	virtual int  num_items() = 0;
	virtual int  hdl_width() = 0;
   virtual ~ac_field_tracer() {}
};

template<int STR_W, typename T, int W>
class ac_typed_field_tracer : public ac_field_tracer<STR_W> {
public:

	ac_typed_field_tracer(
		int ls,  // left shift (for optomized bits)
		T    m   // mask
	)
	: ac_field_tracer<STR_W>()
   , lshift(ls)
	, mask(m)
	{}
   virtual ~ac_typed_field_tracer() {}

	// given a vector with the struct state - trace the field starting at the given location
	virtual void trace_field(sc_lv< STR_W > str_vec, int offset, TraceData *tData) {
		T data;
		if (W > 0) {
			sc_lv<W> tmp = str_vec.range(offset+W-1, offset);
			vector_to_type(tmp, false, &data);
			// apply shift and mask 
			data = (T)((data << lshift) | mask);
		}
		else {
			// all bits optomized out apply mask only
			data = mask;
		}
		string sdat;
		type_to_string(data, sdat);
		TraceReplay::inst()->trace_complex_item(tData, sdat);
	}
	virtual int  num_items() { return 1; }
	virtual int  hdl_width() { return W; }
	
private: // data
	int lshift;
	T mask;
};

// tracer fo 1-dim array of type T element width W with SZ elements
template<int STR_W, typename T, int W, int SZ>
class ac_typed_array_field_tracer : public ac_field_tracer<STR_W> {
public:

	ac_typed_array_field_tracer()
	: ac_field_tracer<STR_W>()
	{}

	// given a vector with the struct state - trace the field starting at the given location
	virtual void trace_field(sc_lv< STR_W > str_vec, int offset, TraceData *tData) {
		for (int i=0; i<SZ; i++ ) {
			sc_lv<W> tmp = str_vec.range(offset+W-1, offset);
			T data;
			vector_to_type(tmp, false, &data);
			string sdat;
			type_to_string(data, sdat);
			TraceReplay::inst()->trace_complex_item(tData, sdat);
			// advance to next element
			offset += W;
		}
	}
	virtual int  num_items() { return SZ; }
	virtual int  hdl_width() { return W*SZ; }
};

template<int S, int W>
SC_MODULE(ac_struct_tracer)
{
public:

typedef std::deque< ac_field_tracer<W>* > fieldlist;

	sc_in<bool> clock;                
	sc_in< sc_logic > reset;
	
	int loc_sz; // buf size
	int num_items;     // total number of data items (each array item of each field)
	fieldlist fields;
	
	TraceData *tData;

	// probes for each input channel - note where lower bits have been optomized out
	// todo - is size always the same width as data?
	sc_in< sc_logic >      probe_req_size;
	sc_in< sc_lv< S > >    probe_size;
	sc_in< sc_logic >      probe_ldin;
	sc_in< sc_logic >      probe_vdin;
	sc_in< sc_lv< W > >    probe_din;

	// Constructor
	SC_HAS_PROCESS(ac_struct_tracer);
	ac_struct_tracer(
	      const sc_module_name& name
   )
    :  clock("clock")
      , reset("reset")   
	  , num_items(0)
	  , fields()
	  , tData(NULL)
	  , probe_req_size("probe_req_size")
      , probe_size("probe_size")
	  , probe_ldin("probe_ldin")
	  , probe_vdin("probe_vdin")
	  , probe_din("probe_din")
	{
		SC_METHOD(eval);
		sensitive << clock.pos() << reset;
		dont_initialize();
	}
	
	void setup_field_trace(fieldlist &flds)
	{
		fields = flds;
		typename fieldlist::iterator iter;
		for (iter = fields.begin(); iter != fields.end(); ++iter) {
			ac_field_tracer<W> *cur = *iter;
			num_items += cur->num_items();
		}
	}
	
	void setupTraceFile(const char *tname)
	{
		tData = TraceReplay::inst()->get_trace_data(tname, TRData::NB_CHAN);
	}
	  
	private: 
	
	void eval()
	{	
		if (tData == NULL) return;
		// NOTE: We are registering size to conform to its behavior
		// in the design
		if (probe_req_size.read() == '1') {
			TraceReplay::inst()->trace_size(tData, loc_sz);
		}
		// accessing size, trace results
		
		vector_to_type(probe_size.read(), false, &loc_sz);
		
		if ((probe_ldin.read() == '1')&&(probe_vdin.read() == '1')) {
			
			// start trace for this struct
			TraceReplay::inst()->trace_complex_begin(tData, num_items);	
			
			sc_lv<W> d_vec = probe_din.read();
			
			// loop through fields sending data to trace 
			int vloc = 0;
			typename fieldlist::iterator iter;
			for (iter = fields.begin(); iter != fields.end(); ++iter) {
				ac_field_tracer<W> *cur = *iter;
				cur->trace_field(d_vec, vloc, tData);
				vloc += cur->hdl_width();
			}
			// end struct trace
			TraceReplay::inst()->trace_complex_end(tData);
		}
	}
};
#endif
