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

/*! \file ac_trace_replay_sa.h
  \brief Catapult builtins
  Implements methods uded in the stand alone TraceReplay manager to read traced file dta.
*/

#ifndef AC_REPLAY_STAND_ALONE
		Compile for Stand Alone Replay ONLY
		Please define AC_REPLAY_STAND_ALONE on the compile line
#endif

#include "ac_trace_replay.h"
#include <cassert>
#include <string>

using std::string;


// initialize singleton instance of Trace/Replay manager
TraceReplay* TraceReplay::_inst = NULL;

#define TR_DUMP
// #define PARSE_DUMP

ReplayParser::ReplayParser(
  const char *filename
)
: inFile(filename)
, lineno(1)
, is_ok(false)
, ver(0)
{
	// open file if it exists
	if (inFile.is_open() && inFile.good()) {
		cout << "Trace file " << filename << " opened for replay." << endl;
		is_ok = true;
	}
	else {
		errStrm << "Error: Unable to open trace file " << filename << " for replay." << endl;
	}
}

ReplayParser::~ReplayParser()
{
	if (inFile.is_open()){
		inFile.close();
	}
}

int ReplayParser::parse_channel_ids(idMap &id_map)
{
	char module[256];
	char tok;
	int id;
	int count = 0;

	// read version #
	while (!inFile.eof()){
		inFile.get(tok);
		if (isalnum(tok)) break;
	}
	if (tok == 'V') {
		inFile >> id;	
#ifdef PARSE_DUMP
cout << "Version:  " << id << endl;	
#endif
		ver = id;
		inFile.ignore(1, '\n'); // discard eoln
	}
	else {
		is_ok = false;
		errStrm << "Error: Unable to read file format." << endl;
	}
	
	// read variable names
	while (!inFile.eof()){
		inFile.get(tok);
		*module = '\0';
		if (tok == 'I') {
			inFile.ignore(1, ' ');
			// parsed ID token get module name
			inFile >> id;
			inFile.ignore(1, ' ');
			inFile.get(module, 1024);
			// crude error check - may help avoid a crash...
			if (*module == '\0') {
				errStrm << "Error: Unable to read channel ID on line:" << lineno << endl;
				is_ok = false;
				break;
			}
#ifdef PARSE_DUMP
 cout << "I " << id << ' ' << module << endl;
#endif
			// look to see if this ID match any channels constructed before file parse
			// (Other will check in later)
			TraceReplay::inst()->find_early_channel(module, id);

			id_map[module] = id;
			inFile.ignore(1, '\n'); // discard eoln
			count++;
			lineno++;
		}
		else {
			inFile.putback(tok);
			break;
		}
	}

	return count;
}

int ReplayParser::parse_events(
		eSizeVector& sizeVec,
		eDataVector& dataVec
)
{
	switch (ver) {
		//case 0: return parse_events_v0(events);
		case 1: return parse_events_v1(sizeVec, dataVec);
		default: errStrm << "Error: Unable to read file format of version " << ver << endl;
	}
	return 0;
}


int ReplayParser::parse_events_v1(
		eSizeVector& sizeVec,
		eDataVector& dataVec
)
{
	while (ok() && !inFile.eof()) {
				// can ignore this ater
		int channel_id;
		eData_t data;
		int size;
		bool is_size = false;

#ifdef TR_DUMP
		// may not work w/o timestamps too
		// if thie line doesn't start with a number eat it
		char nextCh = inFile.peek();
		if (!(isdigit(nextCh))) {
			inFile.ignore(256, '\n');

			// may be debug messages id DUMP is on
			// cout << "Parse error in TRACE file at line " << lineno << endl;

			continue;
		}
#endif

		// need more error handling if people are going to edit file
		inFile >> channel_id;

		// look for size token
		inFile.ignore(1, ' ');
		char star = inFile.peek();
		if (star == '*') {
			is_size = true;
			inFile >> star;
			inFile >> size;
			
			sizeVec[channel_id].push_back(size);
#ifdef PARSE_DUMP
			cout << channel_id << " * " << size << endl;
#endif
		}
		else {
			size = 0;
			inFile >> size;
#ifdef PARSE_DUMP
			cout << channel_id << ' ' << size;
#endif
			for (int i = 0; i < size; i++) {
				inFile >>  data;
				dataVec[channel_id].push_back(data);
#ifdef PARSE_DUMP
				cout << ' ' << data;
#endif
			}
#ifdef PARSE_DUMP
				cout << endl;
#endif			
		}
	}
	cout << "Trace file parsed successfully." << endl;
	
	return 1;
}

/*
	TraceReplay Methods
*/

void TraceReplay::set_replay_data(
	ReplayData *dat
)
{
	if (dat->get_id() == TRData::PRE_PARSE_ID) {
		staticChanMap[dat->get_var_name()] = dat;
	}
	else if (dat->get_id() > TRData::NO_TRACE_ID) {
		channelList[dat->get_id()] = dat;
	}
}

ReplayData * TraceReplay::get_replay_data(
	const char *module_name,
	TRData::ReplayType type
)
{
	int id = get_id(module_name);
	// if OK construct object and check it in
	if (id != TRData::NO_TRACE_ID) {
		ReplayData *dat = new ReplayData(id, module_name, type);
		set_replay_data(dat);
		return dat;
	}
	else return NULL;
}

void TraceReplay::capture_trace_sizes()
{
	// record number of elements parsed in each bin
	for (int i = 1; i <= nextId; i++){
		data_set_sizes[i] = replayDataSet[i].size();
		size_set_sizes[i] = replaySizeSet[i].size();
	}	
}

		// non blocking read method
int TraceReplay::get_size(ReplayData *tData)
{
	eSizeQue &q = replaySizeSet[tData->get_id()];
	int sz = -1;
	if (!q.empty()) {
		sz = q.front();
		q.pop_front();
		tData->set_size(sz);
	}
	else {
		// if we have no traced data, retrun 0 so C code does not do a read
		if (tData->no_traced_data()) return 0;
		// ran out of tickets - return invalid number
		tData->no_size();
	}
	
	return sz;
}

int TraceReplay::peek_size(ReplayData *tData)
{
	eSizeQue &q = replaySizeSet[tData->get_id()];
	int sz = -1;
	if (!q.empty()) {
		sz = q.front();
	}
	return sz;
}


int TraceReplay::data_count(ReplayData *tData)
{
	return replayDataSet[tData->get_id()].size();
}

// Read a single item
int TraceReplay::get_read(ReplayData *tData)
{
	eDataQue &q = replayDataSet[tData->get_id()];
	int res = -1;
	if (!q.empty()) {
		eData_t dat = q.front();
		q.pop_front();
		tData->set_data(dat);
		res = 1;
	}
	else {
		// ran out of traced data
		tData->read_past_trace();
	}
	return res;
}

int TraceReplay::get_id(
	const char *module_name
)
{
# if 0
	// replace withh linear search below to look for multpile matchs (error) and 
	// a kludgy support for class member channels when objects are not(ac_) named
	idMap::iterator itt = id_map.find(module_name);
	if (itt != id_map.end()) {
		return itt->second;
	}
#endif

	// if may be a member of an un-named class
	// we will look for that case and use it if there is only one instance
	int result = (int)TRData::NO_TRACE_ID;
	for  (idMap::iterator itt = id_map.begin(); itt != id_map.end(); ++itt) {
		string id = itt->first;	
		// trim object names off and look for match
		size_t dot_loc = 0;
		size_t end_loc = id.find_first_of('@');
		while (dot_loc < end_loc) {
			string tmp = id.substr(dot_loc);
			if (strcmp(tmp.c_str(), module_name) == 0) {
				// if this is the second match we have an abiguous result 
				// --  annotate with special ID
				if (result != (int)TRData::NO_TRACE_ID) {
					string varname = tmp.substr(0, end_loc);
					return (int)TRData::MULTI_ID;
				}
				// other wise return this as a "best match"
				result = itt->second;
			}
			// no match move down context of sif name
			// such as from:  chan_obj.chan@class@-1 to chan@class@-1
			id.assign(tmp);
			dot_loc = id.find_first_of('.');
			if (dot_loc==string::npos) break;
			++dot_loc;
		}
	}
	
	// if file hasn't been parsed yet return placeholder
	if (parser == NULL) {
		return (int)TRData::PRE_PARSE_ID;
	}
	return result;
}

bool TraceReplay::complete_channel_connection(ReplayData *td)
// utility to check for size reads (indicates NB channel) thus 
// a replayable channel. Report findings to user, sets type
{
	if (peek_size(td) < 0) {
	
		if (data_count(td) == 0) {
			// no data was ever traced something to note (?)
			report_channel_msg(ReplayError::INFO, ReplayError::NO_SIZE_DATA, td, true);
			td->set_no_traced_data();
			return true;
		}
		else {
			// no size data, will treat this channel as blocking reads - which may be OK
			// (Note: we can have no size read in mcg_pipe was never asserted)
			// (otherwise we have a problem)
			td->no_size_data();
		}
	}
	report_channel_msg(ReplayError::INFO, ReplayError::TRACE_CONNECTED, td, true);
	return true;
}

void TraceReplay::find_early_channel(string module, int id)
{
	statMap::iterator dit = staticChanMap.find(module);
	if (dit != staticChanMap.end()) {
		// caught one, update its id field and move it to the OK list
		ReplayData *td = dit->second;
		td->set_id(id);
		channelList[id] = td;
	}
}

void TraceReplay::checkEarlyChannels()
{
	// now that we have parsed IDs we need to complain about static channels that expected trace data but found non.
	statMap::iterator it;
	for (it=staticChanMap.begin(); it!=staticChanMap.end(); it++) {
		ReplayData *cur = it->second;
		if (cur->get_id() == TRData::PRE_PARSE_ID) {
			cur->no_size_data();
			report_channel_msg(ReplayError::WARN, ReplayError::NO_TRACE_IN_FILE, cur, true);
		}
		else if (cur->get_id() != TRData::NO_TRACE_ID) {
			// found ID check for data etc.
			complete_channel_connection(cur);
		}
	}
}
	

// make sure trace data was connected - it would be better to do this
// before the test starts but static construction of ac_channels can occur any time
// so for now  -wait until test is complete
void TraceReplay::check_channel_connections()
{
	// go through the parsed channels and makesure we have a replayer for each
	idMap::iterator it;
	for ( it = id_map.begin(); it != id_map.end(); it++ ) {
		datMap::iterator dit = channelList.find(it->second);
		if (dit == channelList.end()) {
			// look to see if there is any size data, if not its irrelavent, channel did blocking reads
			if (replaySizeSet[it->second].empty()) {
				if (!replayDataSet[it->second].empty()) {
					// valid data, should have been used
					report_channel_msg(ReplayError::WARN, ReplayError::BL_NO_CTOR, it->first.c_str(), false);
				}
			}
			else {
				// valid data, should have been used
				report_channel_msg(ReplayError::ERR, ReplayError::WRONG_CTOR, it->first.c_str(), false);
			}
		}
	}
}

void TraceReplay::report_channel_msg(
	TR_severity   sev,
	TR_error_type et,
	const char   *name,      // assume we have a missing channel
	bool          print_now  // true = output during run, as well as in summary
)
{
	ReplayData *tmp = new ReplayData(0, name, TRData::BL_CHAN);
	tmp->cond_set_error(et,sev);
	// add this to channel list so error can be saved;
	channelList[++nextId] = tmp;
	if (print_now) print_message(tmp,sev,et);
}

void TraceReplay::report_channel_msg(
		TR_severity   sev,
		TR_error_type et,
		TRData        *tData,    // so we can trace error count
		bool          print_now  // true = output during run, as well as in summary
)
{
	if (tData->cond_set_error(et, sev)){
		// will return true if we should print (may be repeat)
		if (print_now) print_message(tData,sev,et);
	}
}

void TraceReplay::print_bad_news(
		TRData    *tData
)
{
	TR_severity sev;
	// get most sever message
	TR_error_type et = tData->get_message(sev);
	print_message(tData,sev,et);
}

void TraceReplay::print_message(
		TRData       *tData,
		TR_severity   sev,
	    TR_error_type et
)
{
	bool include_field_name = false;  // control printing field name
	const char *prepend;
	string id = tData->get_id_string();
	switch (sev) {
		case ReplayError::NONE: // shouldn't  happen 
			return;
		case ReplayError::INFO:
			prepend = "Info:    ";
			break;
		case ReplayError::WARN:
			prepend = "Warning: ";
			break;
		case ReplayError::ERR:
		case ReplayError::FATAL:
			prepend = "Error:   ";
			break;
		default:
			prepend = "???";
	};
	string msg;
	switch (et) {
		case ReplayError::TRACE_CONNECTED:
			include_field_name = true;
			msg.append("Connected to RTL traced data.");
			break;
		case ReplayError::NO_SIZE_DATA:
			msg.append("No data traced.");
			break;
		case ReplayError::EMPTY_CHANNEL:
			msg.append("Attempted to read empty channel.");
			// calling this a channel mis-match, simulation will be said to have failed
			chan_mismatches++;
			break;
		case ReplayError::NO_TRACE_DATA:
			msg.append("C++ execution continued beyond HDL traced data.");
			break;
		case ReplayError::NO_TRACE_SIZE:
			msg.append("C++ execution continued beyond HDL traced data for SIZE.");
			break;
		case ReplayError::AVAILABLE_CALL:
			msg.append("Called available() prior to non-blocking read, replay not handle this correctly.");
			break;
		case ReplayError::NO_TRACE_IN_FILE:
		{
			// need more explaination here, may have been optomized, or the C file editied to move chan decl.
			msg.append("No HDL trace found.");
			msg.append(" It may have been optimized out.");
			// need to add a check of name-only to see if there is a match except for line no.
			msg.append(" Or, make sure C source file was not edited.");
			msg.append(" (Constructed: file: ");
			msg.append( tData->get_file_name());
			msg.append(", line: ");
			msg.append(tData->get_line_num());
			msg.append(")");
			break;
		}
		case ReplayError::WRONG_CTOR:
		{
			msg.append("Did not connect to its traced data. ");
			if (tData->is_class_member()) {
				msg.append("Make sure class instance is Named, and use constructor macro for '");
				msg.append(tData->get_var_name());
				msg.append("' in class constructor.");
			}
			else {
				msg.append("Use constructor macro AC_CHAN_CTOR(");
				msg.append(tData->get_var_name());
				msg.append("). (Expected at file: ");
				msg.append(tData->get_file_name());
				msg.append(", line: ");
				msg.append(tData->get_line_num());
				msg.append(").");
			}
			msg.append(" See User's Manual for details.");
			break;
		}
		case ReplayError::BL_NO_CTOR:
		{
			msg.append("Blocking channel not connect to its traced data. ");
			if (tData->is_class_member()) {
				msg.append("Make sure class instance is Named, and use constructor macro for '");
				msg.append(tData->get_var_name());
				msg.append("' in class constructor.");
			}
			else {
				msg.append("Use constructor macro AC_CHAN_CTOR(");
				msg.append(tData->get_var_name());
				msg.append("). (Expected at file: ");
				msg.append(tData->get_file_name());
				msg.append(", line: ");
				msg.append(tData->get_line_num());
				msg.append(").");
			}
			msg.append(" See User's Manual for details.");
			break;
		}
		case ReplayError::MULTI_IDS:
		{
			msg.append("Ambiguous ID. If channel is a class member, make sure the class instance is Named and AC_CHAN_NAMED_OBJ used for channel construction.");
			msg.append(" See User's Manual for details.");
			break;
		}
	default:
		// bad error arg
		msg.append("Internal Error");
		assert(false);
	}
	cout << prepend << "ac_channel '" << tData->get_var_name();
	// if this is a field of a channel add it in
	const char *fld = tData->get_field_name();
	if (include_field_name && fld) {
		cout << ' ' << fld;
	}
	cout << "': " << msg.c_str();
	cout << endl;
}


void TraceReplay::chan_report(ReplayData *td)
{
	cout << "  Read Count         = " << td->get_read_count() << endl;
	if (td->get_type() == TRData::NB_CHAN){
		cout << "  Size queries       = " << td->get_size_count() << endl;
	}
	cout << "  error count        = " << td->data_error_count << endl;
	if (td->get_size_calls_past_trace()) {
		// non-blocking channels may do size queries after traced data is used up
		// this is probably OK but should be reported
		cout << "  size after trace   = " << td->get_size_calls_past_trace() << endl;
	}
	int data_beyond_trace = td->get_reads_past_trace();
	if (data_beyond_trace) {
	
		int empty_reads = td->get_empty_reads();
		if (empty_reads) {
			cout << "  empty channel reads = " << empty_reads << endl;
			data_beyond_trace -= empty_reads;
		}
		// same asa bove but for blocking channels
	    cout <<     "  data read after trace = " << data_beyond_trace << endl;
	}
	else {
		// reporting traced data not used, this is also to be expected and OK (I think)
		cout << "  unread traced data = ";
		int num_unread = data_set_sizes[td->get_id()] - td->get_read_count();
		cout << num_unread << endl;
	}
}

bool TraceReplay::report_output_summary(ReplayData *td)
	// Trace Data specific output
{
	int num_unread = data_set_sizes[td->get_id()] - td->get_read_count();
	cout << "   unread traced data     = ";
	if (num_unread >= 0) {
		cout << num_unread << endl;
	}
	else {
		// Note: if num-unread is negative it means that sequential execution called the comparitor
		// more time that there wer captured outputs, which can happen
		cout << '0' << endl;
		
		cout << "   reads after trace      = ";
		cout << td->get_reads_past_trace() << endl;
	}

	return false;
}

	// dump error report at end of replay
void TraceReplay::report_channel_summary()
{
	// recall ReplayData objects
	datMap::iterator it;
	for (it=channelList.begin(); it!=channelList.end(); it++) {
		ReplayData *cur = it->second;
		// report all but outs (we do those later)
		if ((cur->get_id() > 0)&&(cur->get_type() != TRData::OUTPUT)) {
			cout << '\'' << cur->get_var_name() << "\' ( " << TRData::getTypeString(cur->get_type()) << " channel )" << endl;
			chan_report(cur);
			cout << endl;
		}
	}
	
	// re-iterate error's found 
	bool first_error = true;
	for (it=channelList.begin(); it!=channelList.end(); it++) {
		ReplayData *cur = it->second;
//		TR_severity sev = ReplayError::NONE;	
// cout << cur->get_id_string() << ' ' << (int)cur->get_error(sev) << ' ' << (int)sev << endl;

		// can now determine if empty reads are errors 
		if (cur->has_error(ReplayError::WARN) || cur->get_empty_reads()){
			if (first_error) {
				first_error = false;
				cout << "Messages from Replay channels:" << endl;
			}		
			// empty read take priority
			if (cur->get_empty_reads()) {
				print_message(cur, ReplayError::ERR, ReplayError::EMPTY_CHANNEL);
			}
			else print_bad_news(cur);
		}
	}
	if (!first_error) cout << endl;
}

