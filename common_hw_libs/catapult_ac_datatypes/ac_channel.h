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

#ifndef __AC_CHANNEL_H
#define __AC_CHANNEL_H

#include <iostream>
#ifndef __SYNTHESIS__
#include <deque>
#include <cassert>
#include <string>
#include <algorithm>
#include <ac_channel_id.h>
  
#ifdef  AC_TRACE_REPLAY_SYS
#include <ac_channel_replay.h>
#include <ac_trace_replay.h>
#include <ac_chan_trace_direct.h>
#endif  // AC_TRACE_REPLAY_SYS
#else
// Marco Definitions for synthesis
#define AC_CHAN_CTOR(varname) varname
#define AC_CHAN_CTOR_INIT(varname,init) varname(init)
#define AC_CHAN_CTOR_VAL(varname,init,val) varname(init,val)
// for compatibility with 9a
#define AC_CHAN_CPP AC_CHAN_SINGLE_OBJ
// use these macros to construct channels in objects NOT derived from ac_named
// WARNING:  there may be only one instance of this object in the design
#define AC_CHAN_SINGLE_OBJ(varname) varname()
#define AC_CHAN_SINGLE_OBJ_INIT(varname,init) AC_CHAN_CTOR_INIT(varname,init)
#define AC_CHAN_SINGLE_OBJ_VAL(varname,init,val) AC_CHAN_CTOR_VAL(varname,init,val)
// used these macros to construct channels of object derived from ac_named -- multiple instance supported -- be sure to "name " each instance in a unique way 
#define AC_CHAN_NAMED_OBJ(varname) varname()
#define AC_CHAN_NAMED_OBJ_INIT(varname,init) AC_CHAN_CTOR_INIT(varname,init)
#define AC_CHAN_NAMED_OBJ_VAL(varname,init,val) AC_CHAN_CTOR_VAL(varname,init,val)
// used on classes with ac_channel members
#define AC_NAMED_OBJECT(cl_name) class cl_name
#define AC_NAME_STATIC_INST(inst)
#define AC_NAME_MEMBER(mbr)
#endif

///////////////////////////////////////////
// Class: ac_channel
//////////////////////////////////////////

template <class T>
class  ac_channel
#ifndef __SYNTHESIS__
  : public ac_channel_id
#endif
{
public:

	T read();
	void read(T& t) ;
	void write(const T& t);

	// constructors
	ac_channel();  
	ac_channel(int init);
	ac_channel(int init, T val);

	bool available(int k);  // Return true if channel has at least k entries
	unsigned int  size();
	bool empty() {return size() == 0;}
 
#ifndef __SYNTHESIS__
	// constructor for Named objects (needed for replay when channel is a member of a class object
	ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno);
	ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init);
	ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init,T val);
#endif

	unsigned int debug_size() { 
#ifndef __SYNTHESIS__
      return chan.size(); 
#endif
   }

	T    operator[](unsigned int pos) const {
#ifndef __SYNTHESIS__
      return chan[pos];
#endif
   }

#ifndef __SYNTHESIS__
	// These functions are useful for writing the testbench and the 
	// scverify flow but are not supported for synthesis.  
	bool operator==(const ac_channel &rhs) const { return this->chan == rhs.chan; }
	bool operator!=(const ac_channel &rhs) const { return !operator==(rhs); }
	void  reset() {chan.clear(); for (int i=0; i<(int)rSz; i++) write(rVal);}
 
	bool nb_read(T& t)   {
		if (size() != 0) {
			  read(t);
			  return true;
		} else return false;
	}
  
	int get_size_call_count() { int tmp=size_call_count; size_call_count=0; return tmp; }

#ifdef  AC_TRACE_REPLAY_SYS

	// Replay needs to copy input channels in verification
	//  (there is no trace data to replay in these)
	ac_channel(const ac_channel< T >& other) 
	: rSz(other.rSz)
	, rVal(other.rVal)
	, tracer(NULL)
	, replay_ports(NULL)
	, chan(other.chan)
	{}

	ac_channel& operator=(const ac_channel< T >& other) 
	{
		rSz = other.rSz;
		rVal = other.rVal;
		tracer = NULL;
		replay_ports = NULL;
		chan = other.chan; 
	}

	// to be called by primary inputs only
	void init_replay(const char *name);
	// trace formal inputs for downstram reader
	// this is a special case method tht will make sure formals that are
	// refs (or ptrs) to channels can supply data in replay when asked
	void setup_direct_trace(const char *writer, const char *id);
	void setup_direct_trace_struct(const char *writer, ac_field_tracer_base<T> *fld, const char *id);
#endif
	// data - The verifcation flow needs it to be public
	std::deque<T> chan;
#else
	// for synthesis
	bool nb_read(T& t);

private:
	// Prevent the compiler from autogenerating these.  (This enforces that channels are always passed by
	// reference.)  
	ac_channel(const ac_channel< T >&); 
	ac_channel& operator=(const ac_channel< T >&);
#endif

private:
 // data
#ifndef __SYNTHESIS__
	unsigned int rSz;    // reset size
	T            rVal;   // resetValue
	int          size_call_count;
#ifdef  AC_TRACE_REPLAY_SYS  
	// methods that checks into trace-replay manager
	// connect to trace data
	// override implementation appends name to context and set ID 
	virtual void set_member_context(const char *ctxt){
		ac_channel_id::set_member_context(ctxt);
		setup_replay(identity());
	}
	void setup_replay(const char *name);  
	
	T    read_tr();
	void write_tr(const T& t);
	int  size_tr();
	bool available_tr(int k);
	
	// Trace/Replay data 
	ac_channel_tracer<T>  *tracer;
	ac_channel_replay<T>  *replay_ports;
	int                    pre_popped;
	std:: deque<T>         advanced_from_trace; 
#endif

#else 
   T chan;  
#endif
};

// Some specializations for the simulation version of  builtin types.  This makes it less likely
// That a C simulation gets an (apparently) correct result due to uninitialized data.
#ifndef __SYNTHESIS__
#include <cstdlib>

#ifdef  AC_TRACE_REPLAY_SYS	
#define __CTOR_INIT_LIST(init) ac_channel_id(varname, owner, filepath, lineno), rSz((unsigned)init), size_call_count(0), tracer(NULL), replay_ports(NULL), pre_popped(0)
#define REPLAY_SU setup_replay(identity())
#define REPLAY_CTOR_STMTS 	,tracer(NULL), replay_ports(NULL), pre_popped(0)
#else
#define __CTOR_INIT_LIST(init) ac_channel_id(varname, owner, filepath, lineno), rSz((unsigned)init), size_call_count(0)
#define REPLAY_SU
#define REPLAY_CTOR_STMTS
#endif

template<> inline ac_channel< char >::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init)
	: __CTOR_INIT_LIST(init) { REPLAY_SU; for (unsigned i=0; i<rSz; i++) write(static_cast<char>(rand()% 256));}
template<> inline ac_channel<int>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init) 
	: __CTOR_INIT_LIST(init) { REPLAY_SU; for (unsigned i=0; i<rSz; i++) write(static_cast<int>(rand()));}
template<> inline ac_channel<unsigned>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init) 
	: __CTOR_INIT_LIST(init) { REPLAY_SU; for (unsigned i=0; i<rSz; i++) write(static_cast<unsigned>(rand()));}
template<> inline ac_channel<long>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init) 
	: __CTOR_INIT_LIST(init) { REPLAY_SU; for (unsigned i=0; i<rSz; i++) write(static_cast<long>(rand()));}
template<> inline ac_channel<short>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init) 
	: __CTOR_INIT_LIST(init) { REPLAY_SU; for (unsigned i=0; i<rSz; i++) write(static_cast<short>(rand()));}
template<> inline ac_channel<float>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init) 
	: __CTOR_INIT_LIST(init) { REPLAY_SU; for (unsigned i=0; i<rSz; i++) write(static_cast<float>(rand()));}
template<> inline ac_channel<double>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init) 
	: __CTOR_INIT_LIST(init) { REPLAY_SU; for (unsigned i=0; i<rSz; i++) write(static_cast<double>(rand()));}
 
#endif

// constructors with no name ID

template <class T>
ac_channel<T>::ac_channel()
#ifndef __SYNTHESIS__
	: rSz(0)
	, size_call_count(0)
	REPLAY_CTOR_STMTS
#endif
{}

// Default for types we don't know about
template <class T>
ac_channel<T>::ac_channel(int init) 
#ifndef __SYNTHESIS__
	: rSz(init)
	, size_call_count(0)
	REPLAY_CTOR_STMTS
#endif
{
	for (int i=init; i> 0; i--) {
		T dc; 
		write(dc);
	}
}

template <class T>
ac_channel<T>::ac_channel(int init, T val) 
#ifndef __SYNTHESIS__
	: rSz(init)
	, rVal(val)
	, size_call_count(0)
	REPLAY_CTOR_STMTS
#endif
{
	for (int i=init; i> 0; i--)
		write(val);
}

#ifndef __SYNTHESIS__
// constructors which support naming (for replay)
template <class T>
ac_channel<T>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno)
 : __CTOR_INIT_LIST(0) { REPLAY_SU; }

// Default for types we don't know about
template <class T>
ac_channel<T>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init) 
 : __CTOR_INIT_LIST(init)
{
	REPLAY_SU;
    for (int i=init; i> 0; i--) {
		T dc; 
		write(dc);
	}	
}

template <class T>
ac_channel<T>::ac_channel(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno, int init, T val) 
 : __CTOR_INIT_LIST(init)
{
	REPLAY_SU;
	for (int i=init; i> 0; i--) 
		write(val);
}

#endif

//The actual hardware looks very much like the SYNTHESIS model.  
// The 2 and 3 argument CTORs store the size arg
// in an integer variable similar to rSz.  The allows us to use a FIFO length which is 
// independent of the startup time of the design.

template <class T>
T ac_channel<T>::read()
{
#ifndef __SYNTHESIS__
#ifdef AC_TRACE_REPLAY_SYS
	return read_tr();
#else
	assert (!chan.empty());  
	T t= chan.front(); 
	chan.pop_front(); 
	return t;
#endif
#else
	// this is non-sense (doesn't get used, right?)
  T dc;
  unsigned int chan_rSz = 0;
  if (chan_rSz == 0) {
    return chan;
  } else {
    chan_rSz--;
    return dc;
  }
#endif
}

template <class T>
void ac_channel<T>::read(T& t) 
{
	t = read();
}

template <class T>
void ac_channel<T>::write(const T& t) 
{
#ifndef __SYNTHESIS__
#ifdef AC_TRACE_REPLAY_SYS
	write_tr(t);
#else
	chan.push_back(t);
#endif
#endif
}

template <class T>
unsigned int  ac_channel<T>::size()
{
#ifndef __SYNTHESIS__
#ifdef AC_TRACE_REPLAY_SYS
	return size_tr();
#else

	size_call_count++;
	return (int)chan.size();
#endif
#endif
}

template <class T>
bool ac_channel<T>::available(int k)
{ 
#ifndef __SYNTHESIS__
#ifdef AC_TRACE_REPLAY_SYS
	return available_tr(k);
#else
	return chan.size() >= k;
#endif
#endif
}

#ifndef __SYNTHESIS__
#ifdef AC_TRACE_REPLAY_SYS
#include <ac_channel_ext.h>
#endif
#endif


template<class T>
inline std::ostream& operator<< (std::ostream& os, ac_channel<T> &a)
{
#ifndef __SYNTHESIS__
  for (unsigned int i=0; i<a.size(); i++) {
    if (i > 0) os << " ";
    os << a[i];
  }
#endif
  return os;
}

#endif
