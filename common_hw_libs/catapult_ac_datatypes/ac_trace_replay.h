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

/*! \file ac_trace_replay.h
  \brief Catapult builtins
  This is a singleton class for tracing events on the blocking side of the
  ac channels and accessing that data for replay. Used for validation.
*/
#ifndef _AC_TRACE_REPLAY_H_
#define _AC_TRACE_REPLAY_H_

#include <fstream>
#include <iostream>
#include <cstdio>
#include <list>
#include <map>
#include <string>
#include <iomanip>

using std::string;
using std::endl;
using std::cerr;
using std::cout;
using std::list;

#define errStrm cerr

struct strCmp {
	// must be case insensitive because capitals in file names
	// are down cased 
	bool operator()( string s1, string s2 ) const {
		return std::strcmp( s1.data(), s2.data() ) < 0; }  
};


// Trace supportF
#include <deque>
#include <vector>
#include <ac_trace_data.h>

using std::ofstream;
using std::ifstream;
using std::vector;
using std::deque;
using std::string;

typedef std::map<string, int, strCmp> idMap;

typedef deque<int>       eSizeQue;
typedef vector<eSizeQue> eSizeVector;
typedef deque<eData_t>   eDataQue;
typedef vector<eDataQue> eDataVector;

#define AC_TRACE_FILE_VERSION 1


#ifdef AC_REPLAY_STAND_ALONE
// keep record of channels checked in
typedef std::map<int, ReplayData*> datMap;
// keep map of channels that check in before data parsed (C++ static construction)
typedef std::map<string, ReplayData*, strCmp> statMap;
#else
// keep record of channels checked in
typedef std::map<int, TraceData*> datMap;
#endif


#ifdef AC_REPLAY_STAND_ALONE
// put in file parser
class ReplayParser {

public:

	ReplayParser(const char *filename);

	~ReplayParser();

	int parse_channel_ids(idMap &id_map);

	int parse_events(
		eSizeVector& sizeVec,
		eDataVector& dataVec
	);

	bool ok() { return is_ok; }

private:
	int parse_events_v1(
		eSizeVector& sizeVec,
		eDataVector& dataVec
	);

private:
	ifstream inFile;
	int      lineno;
	bool     is_ok;
	int      ver;
};
#else
#include <systemc.h>
typedef std::map<string, sc_module *, strCmp> chMap;
#endif


class TraceReplay {
public:

	enum TR_event {
		READ = 0,    // id thread by read or write side
		WRITE = 1
	};

	enum TR_mode {
		OFF,
		TRACE,
		REPLAY
	};
	
	~TraceReplay()
	{
		if (outFile.is_open()){
			outFile.close();
		}
    }

	static TraceReplay *inst() {
		if(TraceReplay::_inst == NULL) {
			TraceReplay::_inst = new TraceReplay();
			// initialize to TRACE MODE
			// TraceReplay::_inst->setup(TRACE);
		}
		return TraceReplay::_inst;
	}

	TR_mode get_mode() { return mode; }	
	void set_mode(TR_mode mode_) { 	mode = mode_; }
	bool ok() { return fatal_error == false; }
	int get_channel_mismatches() { return chan_mismatches; }
	
	void report_channel_mismatch(
		const char   *chan_str,
		const string &cdata,
		const string &rtldata,
		TR_event     rw   // read or write?
	);

	// these 2 are for reports on struct/array data
	void report_mismatch_intro(
		const char *chan_str,
		TR_event rw   // read or write?
	);
	void report_field_mismatch(
		const char *field,
		const string &cdata,
		const string &rtldata
	);
	
	// use this only when channel doesn't really exist (missing trace data or constructor)
	void report_channel_msg(
		TR_severity   sev,
		TR_error_type et,
		const char *name,     // assume we have a missing channel
		bool print_now = true // true = output during run, as well as in summary
#ifdef AC_REPLAY_STAND_ALONE
		// implementation only needed during replay
    );
#else
	) 
	// for trace messages
	{}
#endif

	TraceData *get_trace_data(const char *module_name, TRData::ReplayType type);

#ifdef AC_REPLAY_STAND_ALONE
	// get system wide ID number
	int get_id(const char *module_name);

	void set_replay_data(ReplayData *tData);
	
	ReplayData *get_replay_data(
		const char *module_name,
		TRData::ReplayType type
	);

	void start_iteration(int itr) { 
		iteration = itr; 
	}

	void find_early_channel(string module, int id);
	bool complete_channel_connection(ReplayData *tData);

	// called by parser to do error checking after parsing data
	void checkEarlyChannels();
	
	void report_channel_msg(
		TR_severity   sev,
		TR_error_type et,
		TRData    *tData,
		bool print_now // true = output during run, as well as in summary
	);
	
	// call to see if traced data was used
	void check_channel_connections();
	
	// dump error report at end of replay
	void report_channel_summary();
	bool report_output_summary(ReplayData *td);
	
	// get data from trace file
	int get_size(ReplayData *tData);
	int peek_size(ReplayData *tData);  // get size without popping size deque
	int data_count(ReplayData *tData);  // number of datum in data deque
	int get_read(ReplayData *tData);
#else	
	sc_module *getTraceChannel(const char *name)
	{
		chMap::iterator itt = channelMap.find(name);
		if (itt != channelMap.end()) {
			// already exists
			return itt->second;
		}
		return NULL;		
	}
	
	void addTraceChannel(const char *name, sc_module *newCh)
	{
		channelMap[name] = newCh;
	}
	
	void check_trace_results()
	{
		if (get_mode() == TRACE) {
			datMap::iterator it;
			cout << "Channel Trace Results:" << endl << endl;
			for (it=channelList.begin(); it!=channelList.end(); it++) {
				TraceData *cur = it->second;
				cur->report_trace_results();
			}
		}
	}
#endif
	
	// given unique channel name, return data holder
	// use to setup channels to trace
	void open_trace_file(const char *filename);
	void trace_size(TRData *tData, int sz);
	void trace_read(TRData *tData, const string &data);

	void trace_output(TRData *tData, const string &data);
	
	// use these for multi-item data
	void trace_complex_begin(TRData *tData, int sz);
	void trace_complex_item(TRData *tData, const string &data);
	void trace_complex_end(TRData *tData);

private: // methods
#ifdef AC_REPLAY_STAND_ALONE
	void print_message(
		TRData    *tData,
		TR_severity   sev,
	    TR_error_type et
    );
	void print_bad_news(
		TRData    *tData
    );
	void chan_report(ReplayData *tData);
#endif

private:
	// singleton instance of this object
	static TraceReplay* _inst;

private:
	// private ctr
	TraceReplay()
	: mode(REPLAY)
	, fatal_error(false)
	, iteration(0)
	, chan_mismatches(0)
	, channelList()
	, filepath()
	, nextId(1)
	, id_map()
#ifdef AC_REPLAY_STAND_ALONE
    , parser(NULL)
	, replayDataSet()
	, replaySizeSet()
	, data_set_sizes()
	, size_set_sizes()
	, staticChanMap()
#else
	, channelMap()
#endif
	{}
	
	bool do_trace() { return outFile.is_open(); }

#ifdef AC_REPLAY_STAND_ALONE
	// special file IO stuff
	void capture_trace_sizes();
#endif

private: // data
	TR_mode   mode;
	bool      fatal_error;
	int       iteration;
	int       chan_mismatches;
	datMap    channelList;
	ofstream  outFile;
	string    filepath;
	int       nextId;
	idMap     id_map;   // get you ID given your name
#ifdef AC_REPLAY_STAND_ALONE
// File parser support
	ReplayParser *parser;

	eDataVector   replayDataSet;
	eSizeVector   replaySizeSet;
	
	// keep original sizes of trace
	vector<int>   data_set_sizes;
	vector<int>   size_set_sizes;
	
	statMap       staticChanMap;
#else
	chMap         channelMap;
#endif
};

// INLINE METHODS
	
inline void TraceReplay::open_trace_file(const char * file_path)
{
#ifdef AC_REPLAY_STAND_ALONE
    // open exisiting trace  file for replay
	parser = new ReplayParser(file_path);
	if (parser->ok()){
		nextId = parser->parse_channel_ids(id_map);
		if (parser->ok()) {
			int num_traces = (id_map.size()+1);
						
			replayDataSet.insert(replayDataSet.begin(), num_traces, eDataQue());
			replaySizeSet.insert(replaySizeSet.begin(), num_traces, eSizeQue());
			
			data_set_sizes.insert(data_set_sizes.begin(), num_traces, 0);
			size_set_sizes.insert(size_set_sizes.begin(), num_traces, 0);
			
			parser->parse_events(replaySizeSet, replayDataSet);
			capture_trace_sizes();
		}
		if (!parser->ok()){
			errStrm << "Error: Unable to apply replay file due to parse errors." << endl;
			set_mode(OFF);
			fatal_error = true;
		}
		else checkEarlyChannels();
	}
	else {
		errStrm << "Error: Unable to read replay file. Make sure SC Verify had been run to completion." << endl;
		set_mode(OFF);
		fatal_error = true;
	}
#else 
	if (file_path) {
		filepath.assign(file_path);
		// open file for tracing
		outFile.open(file_path);
		if (outFile.is_open()) {
			cout << "Opened file " << filepath << endl;
			// print file version
			outFile << "V " << AC_TRACE_FILE_VERSION << endl;
		}
		else {
			// can't trace
			errStrm << "Warning: Can't open trace file '" << filepath << "'." << endl;
			set_mode(OFF);
			fatal_error = true;
		}
	}
	else {
		// file path was null  - so trace replay is not active in the environment
		errStrm << "Warning: Trace/Replay not supported in this environment. Trace function disabled." << endl;
		set_mode(OFF);
	}
#endif
}


inline TraceData *TraceReplay::get_trace_data(
	const char *module_name,
	TRData::ReplayType type
)
{
#ifndef AC_REPLAY_STAND_ALONE
	// cout << "Set up " << module_name << endl;
	// save name - id association so the design can 
	// sync'ed from persistent data
	if (do_trace()) {
		outFile << "I " << nextId << ' ' << module_name << endl;
		TraceData *dat = new TraceData(nextId, module_name, type);
		channelList[nextId++] = dat;
		return dat;
	}
	else 
#endif
		return NULL;
}

inline void TraceReplay::trace_read(
	TRData       *tData, // should NOT be null
	const string &data
)
{
	outFile << tData->get_id() << " 1 " << data << endl;
	tData->incr_trace_count();
	
	if (tData->get_type() == TRData::NB_CHAN) {
		//if size sits at 0 either this is a blocking read or  there is a coding error.
		// todo - we need to determine the difference - for now assume a blcoking channel
		if(tData->get_size()==0) {
			tData->no_size_data();
		}
	}	
}

inline void TraceReplay::trace_output(
	TRData 	  *tData, // should NOT be null
	const string  &data
)
{
		outFile << tData->get_id() << " 1 " << data << endl;
}

inline void TraceReplay::trace_complex_begin(
	TRData 	  *tData, // should NOT be null
	int            sz
)
{
		outFile << tData->get_id() << ' ' << sz;
}
inline void TraceReplay::trace_complex_item(
	TRData 	  *tData,
	const string  &data
)
{
		outFile << ' ' << data;
}
inline void TraceReplay::trace_complex_end(
	TRData 	  *tData
)
{
		// count it here
		tData->incr_trace_count();
		outFile << endl;
}

inline void TraceReplay::trace_size(
	TRData *tData, // should NOT be null
	int sz
)
{
	outFile << tData->get_id() << " * " << sz << endl;
	// hold onto size so we can see in following reads are block/non-block, etc/
	tData->set_size(sz);
}

inline void TraceReplay::report_channel_mismatch(
	const char *chan_str,
	const string &cdata,
	const string &rtldata,
	TR_event rw   // read or write?
)
{
	chan_mismatches++;
	cout << "Error: Data mis-match on " << ((rw == READ) ? "READ" : "WRITE") << " of channel " << chan_str << " (test iteration: " << 
		iteration << "). C++  " <<
		cdata << " HDL: " << rtldata << endl;
}

// use these methods for errors in structural data
inline void TraceReplay::report_mismatch_intro(
	const char *chan_str,
	TR_event rw   // read or write?
)
{
	chan_mismatches++;
	cout << "Error: Data mis-match on " << ((rw == READ) ? "READ" : "WRITE") << " of channel " << chan_str << " (test iteration: " << 
		iteration << ")." << endl;
	cout << std::setw(16+4) << "field" <<  ' '
			<< std::setw(20) << "C++ value" << ' '
			<< std::setw(20) << "dut value" << endl;
}
inline void TraceReplay::report_field_mismatch(
	const char *field,
	const string &cdata,
	const string &rtldata
)
{
	cout << ((cdata == rtldata) ? "=== " : "ERR ");
	cout << std::setw(16) << field << ' '
	  << std::setw(20) << cdata << ' '
	  << std::setw(20) << rtldata << endl;
}

class TraceReplayContainer {
public:
	// open for tracing
	TraceReplayContainer(const char *filename, TraceReplay::TR_mode mode = TraceReplay::TRACE) {
		TraceReplay *tr = TraceReplay::inst();
		tr->set_mode(mode);
		tr->open_trace_file(filename);
	}
};

// here for now
#ifdef AC_REPLAY_STAND_ALONE
struct mc_wait_ctrl {
  enum enum_wait_mode     {UNIFORM  = 0, RANDOM = 1};
  enum enum_wait_elements {INITIAL  = 0, ALL  = 1, FIXED_INTERVAL = 2, RANDOM_INTERVAL = 3};

  explicit mc_wait_ctrl() 
  : iteration(-1)
  , cycles(-1)
  , interval(0)
  , mode(UNIFORM)
  , elements(INITIAL)
    {}

  mc_wait_ctrl(
    int i, 
    int c,
    enum_wait_mode m,
    enum_wait_elements e,
    int n) : iteration(i), cycles(c), 
    interval(n), mode(m), elements(e)
    {}
  void clear() { iteration=0; cycles=0; mode=UNIFORM; elements=INITIAL; interval=0; }
  bool is_set() { return (cycles > 0); }
  int                   iteration;  // testbench iteration (transaction) to which this info applies
  int                   cycles;     // num of cycles to wait (constant or 1/2 average of random)
  unsigned              interval;   // streamed element interval (fixed or max rand) at which wait cycles applied
  enum_wait_mode        mode;       // UNIFORM or RANDOM
  enum_wait_elements    elements;   // INITIAL (begin of transaction) or ALL (all elements in array/channel)
};
#endif

#endif
