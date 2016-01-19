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

#ifndef INCLUDED_AC_NAMED_H
#define INCLUDED_AC_NAMED_H

#include <string>
#include <deque>

using std::string;
using std::deque;
class ac_named;

// For convenience to conditionally derived a class (without other inheritance) owning ac_channels from ac_named
#define AC_NAMED_OBJECT(cl_name) class cl_name : public ac_named

// Use these macro on instaces of objects DERIVED FROM AC_NAMED and owning ac_channels, to set their names:
// this one should be used on static instances -- make sure these instance are uniquely named.
#define AC_NAME_STATIC_INST(inst) inst.ac_init_named_inst(#inst);

// use this one to name objects that own ac_channels and are instanitated as members of a named class. 
#define AC_NAME_MEMBER(mbr) { reg_class_member (&(mbr), #mbr); }

class ac_named {
public:
	// ctr
	ac_named()
	: d_is_init(false)
	{}

	virtual ~ac_named() {}
	
	void reg_child(ac_named *ch) {
		d_children.push_back(ch);
	}
	
	// called on (or just after) construction objects that are member of a class
	// save my name until owner calls with context
	void ac_init_named_member(const char *n)
	{
		d_name.assign(n);
	}
	
	void reg_class_member(ac_named *ch, const char *n) {
		ch->ac_init_named_member(n);
		reg_child(ch);
	}	
	
	// called after construction of a static instance of a class
	void ac_init_named_inst(const char *n)
	{
		if (! d_is_init) {
			d_name.assign(n);
			init_children(n);
			d_is_init = true;
		}
	}
	
	void init_children(const char *ctxt) {
		deque<ac_named*>::iterator iter;
		for (iter = d_children.begin(); iter != d_children.end(); ++iter) {
			ac_named *cur = *iter;
			cur->set_member_context(ctxt);
		}
	}
	
	// base implementation appends name to context and calls children
	virtual void set_member_context(const char *ctxt){
		if (! d_is_init) {
			create_context_name(ctxt);
			init_children(get_context_name());
			d_is_init = true;
		}
	}
	
	const char *get_context_name() { return d_context.c_str(); }
	const char *get_named() { return d_name.c_str(); }
	
protected:
	void create_context_name(const char *ctxt){
			d_context = ctxt;
			d_context.push_back('.');
			d_context.append(d_name);
	}
	
// data
private:
	bool   d_is_init;
	string d_name;
	string d_context;
	deque< ac_named* > d_children;
};

#endif

