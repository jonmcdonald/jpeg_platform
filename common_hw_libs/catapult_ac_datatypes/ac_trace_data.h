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
#ifndef _AC_TRACE_DATA_H_
#define _AC_TRACE_DATA_H_

// For compatibilty with gcc 4.3.3
#include <cstring>

// debug hooks
//#define LOGOUT TraceReplay::inst()->log()
//#define LOG_TSTAMP

typedef long long eData_t;

struct ReplayError {
	enum error_type {
		NO_ERR = 0,
		TRACE_CONNECTED,
		NO_SIZE_DATA,
		EMPTY_CHANNEL,     // tried to read channel w/ no data (size out of sync?)
		NO_TRACE_DATA,
		NO_TRACE_SIZE,
		AVAILABLE_CALL,
		NO_TRACE_IN_FILE,  // trace data not found in file
		MULTI_IDS,         // ID is ambiguous, mutiple possibilities
		BL_NO_CTOR,         // blocking chan did not create ac_channel with ID producing constructor args
		WRONG_CTOR,         // did not create ac_channel with ID producing constructor args
		ERROR_TYPE_COUNT // keep at end
	};
	enum severity {
		NONE = 0,
		INFO = 1,
		WARN = 2,
		ERR = 3,
		FATAL= 4
	};
};

typedef ReplayError::error_type TR_error_type;
typedef ReplayError::severity TR_severity;

static const char *TypeStrings[] = { "an Output", "an Input", "a Blocking", "a Non-blocking" };

/** TRData  **/
class TRData {
public:

	enum {
		NO_TRACE_ID = 0,     // file parsed, no data for you
		PRE_PARSE_ID = -100,  // place holder 'til file is read
		MULTI_ID = -200      // can't match ID - need more context
	};

	enum ReplayType {
		OUTPUT,
		IN_CHAN,
		BL_CHAN,
		NB_CHAN
	};

	TRData(int _id, const char *_name, ReplayType _type)
	: data_error_count(0)
	, m_id(_id)
	, m_size(0)
	, m_error(ReplayError::NO_ERR)
	, m_severity(ReplayError::NONE)
	, read_count(0)
	, size_count(0)
	, type(_type)
	{
		id_string = new char [strlen(_name)+1];
		strcpy(id_string, _name);

		// get neme from ID
		string id(_name);
		int eon = id.find_first_of('@');
		string var(id, 0, eon);
		const char * t = var.data();
		name = new char [strlen(t)+1];
		strcpy(name, t);
		
		// get file from (static) id
		filename = NULL;
		int efn = id.find_first_of('@', eon+1);
		if (efn>0) {
			string file(id, eon+1, efn-eon-1);
			t = file.data();
			filename = new char [strlen(t)+1];
			strcpy(filename, t);
		}
		// get line number from (static) id
		lineno = NULL;
		int eln = id.find_first_of('@', efn+1);
		if (eln>0) {
			string line(id, efn+1, eln-efn-1);
			t = line.data();
			lineno = new char [strlen(t)+1];
			strcpy(lineno, t);
		}
		else {
			string line(id, efn+1, id.length());
			t = line.data();	
			lineno = new char [strlen(t)+1];
			strcpy(lineno, t);
		}

		// get field name if this is a field
		fieldname = NULL;
		if (eln>0) {
			string fldn(id, eln+1, id.length());
			t = fldn.data();
			fieldname = new char [strlen(t)+1];
			strcpy(fieldname, t);
		}
		
		for (int i = 0; i < ReplayError::ERROR_TYPE_COUNT; i++) {
			m_error_trac[i] = 0;
		}
	}
   virtual ~TRData() {
      if (id_string) delete id_string; id_string = 0;
      if (name) delete name; name = 0;
      if (filename) delete filename; filename = 0;
      if (lineno) delete lineno; lineno = 0;
      if (fieldname) delete fieldname; fieldname = 0;
   }

	static const char *getTypeString(ReplayType type) {
		return TypeStrings[(int)type];
	}

	TR_error_type get_message(TR_severity &sev) { sev = m_severity; return m_error; }
	// true if error of queried severity has been reported prior
	bool has_error(TR_severity sev) { return (sev > m_severity)?false:true; }
	
	int get_id() { return m_id; }
	
	// running count of call in this session
	int get_read_count() { return read_count; }
	int get_size_count() { return size_count; }
	
	int get_size() { return m_size; }
	ReplayType get_type() { return type; }
	void set_id(int id) { m_id = id; }
	void set_size(int sz) {  size_count++; m_size = sz; }
	void incr_trace_count() { read_count++; }
	void no_size_data() { type = BL_CHAN; }  // none traced
	

	virtual bool cond_set_error(TR_error_type er, TR_severity sev) { 
	
		// keep most severe error - for report at end
		if (sev>m_severity) { 
			m_error = er; 
			m_severity = sev; 
		}
		
		if (m_error_trac[er]++) {
			// if error is already output return false to avoid 
		    // repeating runtime errors
			return false;
		}
		return true;
	}
	
	// a klugy way to know if this is a class member, works for now
	bool is_class_member() {
		return (*lineno == '-') ? true : false;
	}
	
	const char *get_id_string() { return id_string; }
	const char *get_var_name() { return name; }
	const char *get_file_name() { return filename; }
	const char *get_line_num() { return lineno; }
	const char *get_field_name() { return fieldname; }

	int           data_error_count;  // used by internal channels only
	
protected:

	int           m_id;
	int           m_size;

	TR_error_type m_error;
	TR_severity   m_severity;
	int           m_error_trac[ReplayError::ERROR_TYPE_COUNT]; // keep account of which message have been given
	int           read_count;
	int 		  size_count;
	ReplayType    type;
	char         *id_string;
	char         *name;
	char         *filename;
	char         *lineno;
	char         *fieldname;
};


class ReplayData : public TRData {
public:

	ReplayData(int id, const char *name, ReplayType type)
	: TRData(id, name, type)
	, no_trace(false)   // default val, will set after trace parse
	, past_size(0)
	, past_trace(0)
	, empty_read_count(0)
	{
		// if ID indicates an error state set the err now
		// so we can report it (or fix it) later
		if (id == MULTI_ID) {
			cond_set_error(ReplayError::MULTI_IDS, ReplayError::ERR);
		}
	}
   virtual ~ReplayData() { }
	
	void set_no_traced_data() { no_trace = true; } 
	bool no_traced_data() { return no_trace; } 
	void no_size() { past_size++; size_count++; }       // out of trace data
	void read_past_trace() {past_trace++; read_count++;}
	int get_reads_past_trace() { return past_trace;}
	int get_empty_reads() { return empty_read_count;}
	int get_size_calls_past_trace() { return past_size;}
	eData_t get_read_data() { return m_data; }

	void set_data(eData_t dat) { 
		// check to see if this is a blocking or non-blocking channel
		// this is for debug purposes for now but we may want to warn if
		// size is called sometimes and "available" or nothing called other times
		if (read_count == 0 && size_count == 0) {
			if (type == NB_CHAN) type = BL_CHAN;
		}
		m_data = dat; read_count++; 
	}
	
	virtual bool cond_set_error(TR_error_type er, TR_severity sev) { 
	
		if (er == ReplayError::EMPTY_CHANNEL) {
			// defer empty read error until we give writer a chance to catch up
			empty_read_count++;
			return false;
		}
		return TRData::cond_set_error(er, sev);
	}
	
	
	
	bool empty_read_covered() {
		if (empty_read_count) {
			empty_read_count--;
			return true;
		} else return false;
	}

private:

	bool     no_trace;  // set true if no data is traced for this ID
						// we will alway return 0 if size is called here
	eData_t  m_data;
	
	int      past_size;	     // number of size queries after trace data is spent
	int      past_trace;     // number of read attempt after trace data is spent
	int      empty_read_count;
};

class TraceData : public TRData {
public:

	TraceData(int id, const char *name, ReplayType type)
	: TRData(id, name, type)
	, bypass_source(NULL)
	{}
   virtual ~TraceData() {}
	
	void report_trace_results()
	{
		if (get_type() == OUTPUT) return;
		cout << "#    " << get_var_name();
		if (read_count || size_count) {
			cout  << ": captured ";
			if (get_type() == NB_CHAN) {
				cout << size_count << " read attempts (size queries) and ";
			}
			cout << read_count << " read events ";
			if (bypass_source) {
				if (get_type() == IN_CHAN) {
					cout << "(directly from formal input " << bypass_source << ") ";
				}
				else cout << "(directly from channel " << bypass_source << ") ";
			}
			cout << "as " << getTypeString(get_type()) << " Channel.";
		}
		else {
			cout  << ": was not traced.";
		}
		cout << endl;
	}
	
	void notify_bypass(const char *_wr)
	{
		bypass_source = new char [strlen(_wr)+1];
		strcpy(bypass_source, _wr);
	}

private:
	char  *bypass_source;  // writing channel - if this is bypass
};

#endif
