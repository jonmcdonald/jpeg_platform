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

#ifndef _AC_MANIFOLD_CHANNEL_REPLAY_H_
#define _AC_MANIFOLD_CHANNEL_REPLAY_H_

#include <ac_channel_replay.h>

using std::string;

template<class Tclass, class Bclass > 
void report_mismatch(
		Tclass               *field,
		const char           *chan_str,
		Bclass               &cdata,
		Bclass               &rtldata,
		bool                  test_first = true  // don't report if fields are equal
)
{
	if (field) {
		if (!test_first || (cdata != rtldata)) {
			field->report_field_mismatch(chan_str, cdata, rtldata);
		}
	}
}

// call field type to read data, return amount of data read
template<class Tclass, class Bclass > 
int get_read_data(
		Tclass               *field,
		Bclass               &data
)
{
	if (field) {
		return (field->get_read(data) != -1) ? 1 : 0;
	}
	return 0;
}


// derive a utility base class for split multi-pipe channels
template<class Tclass, int Size > 
class ac_split_channel_replay : public ac_channel_replay< Tclass > {
public:	

	// constructor - struct NOT preserved
	ac_split_channel_replay(ac_channel_replay_base *flds[Size])
	: proxy(NULL)
	{
		// pick a proxy
		int sz_loc, sz = -1;
		for (int i=0; i<Size; ++i) {
			// some fields my be null if optomized out
			fields[i] = flds[i];
			if (flds[i] == NULL) continue;
			
			sz_loc = flds[i]->data_count();
			// first look for size difference  - I dont think there will be one - unless a field is 0 due to being reduced out.
			// should probably assert if this true
			if ((sz!=-1)&&(sz_loc!=0)&&(sz_loc!=sz)){
				cout << "Data field size mis-match in channel replay data" << endl;
			}
			if (sz_loc > sz) {
				proxy = flds[i];
				sz = sz_loc;
			}
		}
		// must now have a proxy - check prior that there is data for this channel
		assert(proxy != NULL);
	}

	virtual int get_size() {
		// not meaningful - the assuption is that this will not be called as non-blocking channels will not be allowed to
	    // split to multi-channels
		// assert(false);
		return proxy->get_size();
	}
	virtual int data_count() {
		return proxy->data_count();
	}

	virtual bool connected() {
		return proxy->connected();
	}

	virtual bool empty_read_covered() {
		return proxy->empty_read_covered();
	}	

	virtual void report_channel_msg(
		TR_severity   sev,
		TR_error_type et,
		bool print_now = true // true = output during run, as well as in summary
	)
	{
		// if not tdata - we have worse things to report
		proxy->report_channel_msg(sev, et, print_now);
	}
	
		// output messages regarding results
	virtual bool report_output_summary()
	{
		// I dont think this will be used (???)
		return false;
	}

	virtual int get_output(Tclass &data) { 
		return get_read(data);
	}
	
	ac_channel_replay_base *get_field(int idx) {
           return this->fields[idx];
        }

protected: //data
	ac_channel_replay_base *proxy;  // the one to use for tracking size etc.
	ac_channel_replay_base *fields[Size];
};

////////////////////////////////////////
// an interface class for type readers
// these are used to to create reader for stucts 
class ac_type_reader_base {
protected:
	ac_type_reader_base(ReplayData *td)
	: tdata(td)
	{}

	ReplayData *tdata;
};

// typed reader for a field in a struct
template<class Tclass>
class ac_type_reader : public ac_type_reader_base {
public:

    ac_type_reader(ReplayData *td)
	: ac_type_reader_base(td)
	{}

	int get_read(Tclass &ret) {	return data_reader.read_data(ret, tdata); }

	void report_field_mismatch(
		const char           *chan_str,
		Tclass               &cdata,
		Tclass               &rtldata
	)
	{
		if (tdata) {
			string cd;
			string rd;
			tr_data_printer<Tclass>::type_to_print(cdata, cd);
			tr_data_printer<Tclass>::type_to_print(rtldata, rd);
			TraceReplay::inst()->report_field_mismatch(chan_str, cd, rd);
		}
		// should never happen
		else assert (false);
	}

	static ac_type_reader< Tclass > *do_cast( ac_type_reader_base *base_obj) {
		return static_cast < ac_type_reader< Tclass > * > (base_obj);
    	}
	
private: //data

	tr_data_reader<Tclass> data_reader;
};

///////////////////////////////////////////////////////////
// derive a utility base class for preserved multi-pipe channels
// currently looks the same asa pipe except for those methods
// overridden in the derived class
template<class Tclass, int Size > 
class ac_psrv_channel_replay : public ac_channel_pipe_replay_base< Tclass > {
public:	
	// constructor - struct preserved
	ac_psrv_channel_replay(ReplayData *td, ac_type_reader_base *flds[Size])
	: ac_channel_pipe_replay_base< Tclass >::ac_channel_pipe_replay_base(td)
	{
		for (int i = 0; i < Size; i++) {
			fields[i] = flds[i];
		}
	}

	virtual int get_output(Tclass &data) { 
		return get_read(data);
	}
	
	ac_type_reader_base *get_field(int idx) {
		return this->fields[idx];
	}

protected: //data
	ac_type_reader_base *fields[Size];
};

#endif
