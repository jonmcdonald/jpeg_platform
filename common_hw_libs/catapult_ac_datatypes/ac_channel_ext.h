////////////////////////////////////////////////////////////////////////////////
// Catapult Synthesis
// 
// © 2010 Mentor Graphics Corporation
//       All Rights Reserved
// 
//  This document contains information that is proprietary to Mentor Graphics  
//  Corporation. The original recipient of this document may duplicate this  
//  document in whole or in part for internal business purposes only, provided  
//  that this entire notice appears in all copies. In duplicating any part of  
//  this document, the recipient agrees to make every reasonable effort to  
//  prevent the unauthorized use and distribution of the proprietary information.
////////////////////////////////////////////////////////////////////////////////

#ifndef __AC_CHANNEL_EXT_H
#define __AC_CHANNEL_EXT_H

//#define AC_TR_DUMP

#define AC_REPLAY_MODE (TraceReplay::inst()->get_mode() == TraceReplay::REPLAY)
#define AC_REPLAY_ACTIVE (AC_REPLAY_MODE && replay_ports && replay_ports->connected())
#define AC_TRACE_ACTIVE (tracer != NULL)

template <class T>
T ac_channel<T>::read_tr()
{
	// if using replay we must sync with trace ticket
	if (AC_REPLAY_ACTIVE) {
		// read ticket ASSUMES trace of size has been
		// checked
		T traced_data;
		int sz = replay_ports->get_read(traced_data);

		if (sz == -1) {
			// read with no data present, is this an error 

			// have to return something!!!!
			//use channel data if it exists
			if (!chan.empty()) {
#ifdef AC_TR_DUMP
			    if (AC_TR_DUMP_COND) cout << "CH " << identity() << ": No Traced DATA (continue with channel data)" << endl;
#endif	
				replay_ports->report_channel_msg(ReplayError::INFO, ReplayError::NO_TRACE_DATA, true);
				traced_data = chan.front();
			}
			else {
				// return last datum from trace  - should probably assert here....
				//assert (!chan.empty());
#ifdef AC_TR_DUMP
			    if (AC_TR_DUMP_COND) cout << "CH " << identity() << ": EMPTY CHANNEL and NO_TRACE_DATA" << endl;
#endif	
				replay_ports->report_channel_msg(ReplayError::ERR, ReplayError::EMPTY_CHANNEL, false);	
				// Kludge: Advance non-existant traced data - it is common to read past trace 
				// an error will be flagged later if writer does not cover this - the data is 
				// not likely to match but it should not matter to the output result
			    pre_popped++;
				return traced_data;
			}
		}

		// if channel is actally empty return 
		// traced data nad track it as it will have to be pulled off
		// later when it shows up here

		if (chan.empty()) {
#ifdef AC_TR_DUMP
			if (AC_TR_DUMP_COND){cout << "CH " << identity() << ": Advance traced data " << traced_data << endl;}
#endif
			// save pre-arrived data to check later - order should be preserved
			advanced_from_trace.push_back(traced_data);
			pre_popped++;
			return traced_data;
		}

		// if here we can use front of fifo - do normal read
		// TODO -- merge with else section below
		T t = chan.front(); 
		chan.pop_front();
#ifdef AC_TR_DUMP
		if (AC_TR_DUMP_COND){cout << "CH " << identity() << ": Returning channel data " << (int)t << endl;}
#endif
		// assert(t == traced_data);
		if ( ! replay_ports->equal(t, traced_data)) {
			replay_ports->report_channel_mismatch(print_name(), t, traced_data, TraceReplay::READ);
		}
		return t;
	}
	else {
		if (chan.empty()){
			cout << "Error: ac_channel " << print_name() << ": Attempted to read from an empty channel." << endl;
		}
		assert (!chan.empty());  
		T t= chan.front(); 
		chan.pop_front(); 
		if (AC_TRACE_ACTIVE) {
			// used in special cases to trace channel content directly 
			tracer->trace_read(t);
		}
		return t;
	}
}

template <class T>
void ac_channel<T>::write_tr(const T& t) 
{
	chan.push_back(t);

	// if using replay we must sync with trace ticket
	if (AC_REPLAY_ACTIVE) {
		// if we returned data prior to its arrival and 
		// data now exists we need to to pop it now
		// (Later may want to do a compare of trace, actual data
		if (pre_popped) {
			pre_popped--;
			// first look to cover empty reads that may happen after TRace data is spent
			if (replay_ports->empty_read_covered()){
				return;
			}
#ifdef AC_TR_DUMP
			if (AC_TR_DUMP_COND) cout << "CH " << identity() << ":  Popping channel data " << (int)chan.front() << endl;
#endif
			// check that data now coming off is in the same order
			// as it arrived in the trace.
			T a = advanced_from_trace.front();
			T c = chan.front();
			// assert(a == c);
			if ( !replay_ports->equal(a,c) ) {
				const char *nm = print_name();
				replay_ports->report_channel_mismatch(nm, c, a, TraceReplay::WRITE);
			}
			advanced_from_trace.pop_front();
			chan.pop_front(); 
		}
	}
}

template <class T>
int  ac_channel<T>::size_tr()
{
	size_call_count++;
	// if replaying, we subordinate the results
	// of this call to the tracer
	if (AC_REPLAY_ACTIVE) {
		int sz = replay_ports->get_size();
		if (sz < 0) {
#ifdef AC_TR_DUMP
			if (AC_TR_DUMP_COND) cout << "CH " << identity() << ": No data from SIZE call - return 0" << endl;
#endif
			replay_ports->report_channel_msg(ReplayError::INFO, ReplayError::NO_TRACE_SIZE, true);
			
			// fall through and return actual channel state 
			// - we are likely to be running after significant data has past
		}
		else {
#ifdef AC_TR_DUMP
			if (AC_TR_DUMP_COND){cout << "CH " << identity() << ": Returning size " << sz << endl;}
#endif
			return sz;
		}
	}
	return (int)chan.size();
}

template <class T>
bool ac_channel<T>::available_tr(int k)
{ 
	if (AC_REPLAY_ACTIVE) {
		// try to get info from  trace - in general, the RTL will return "true" here
		// until traced data is no longer available
		int sz = replay_ports->data_count();
#ifdef AC_TR_DUMP
		if (AC_TR_DUMP_COND){cout << "CH " << identity() << ": Available call sees " << sz << endl;}
#endif
		// if we got back -1 we have no traced data - which is correct in the case of
		// a blocking channel - return its size
		if (sz < 0) {
			return (int)chan.size() >= k;
		}
		return sz >= k; // TRUE is the correct responce from the design BUT may cause infinite loop if called as loop test.
	}
	else return (int)chan.size() >= k;
}


template <class T>
void ac_channel<T>::init_replay(const char *name)
{
	id_init(name);
	setup_replay(name);
}

// this one is used in TRACE mode to trace formal inputs (that are channels) directly when needed
template <class T>
void ac_channel<T>::setup_direct_trace(const char *writer, const char *id)
{
	// cout << "ac_channel<T>::setup_direct_trace of " << id << endl;
	tracer = new ac_channel_simple_tracer<T>(writer, id);
}

// overloaded for field of preserved struct
template <class T>
void ac_channel<T>::setup_direct_trace_struct(const char *writer, ac_field_tracer_base<T> *fld, const char *id)
{
	// todo - checking each filed so we know which are being captured
	if (tracer == NULL) {
		// cout << "ac_channel<T>::setup_direct_trace of " << id << endl;
		tracer = new ac_channel_struct_tracer<T>(writer, id);
	}
	((ac_channel_struct_tracer<T>*)tracer)->add_field(fld);
}

// make connection to traced data for use during replay
template <class T>
void ac_channel<T>::setup_replay(const char *name)
{
	replay_ports = NULL;
	if ( AC_REPLAY_MODE && has_identity()) {
		// get trace channel 
		get_channel_replay_ports(replay_ports, name);
	}
}



#endif
