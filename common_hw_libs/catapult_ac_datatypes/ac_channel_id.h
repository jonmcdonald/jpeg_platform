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

#ifndef __AC_CHANNEL_ID_H
#define __AC_CHANNEL_ID_H

#include <ac_named.h>
#include <string>
// For compatibilty with gcc 4.3.3
#include <cstring>

#define AC_CHAN_CTXT const char *
#define AC_CHAN_OWNER ac_named *

// Definitions for non-synthesis compilation,
#define AC_QUOTED_STRING(x) #x
#define AC_CHAN_CTOR(varname) varname(AC_QUOTED_STRING(varname), NULL,__FILE__,__LINE__)
#define AC_CHAN_CTOR_INIT(varname,init) varname(AC_QUOTED_STRING(varname), NULL,__FILE__,__LINE__,init)
#define AC_CHAN_CTOR_VAL(varname,init,val) varname(AC_QUOTED_STRING(varname), NULL,__FILE__,__LINE__,init,val);

// for compatibility with 9a
#define AC_CHAN_CPP AC_CHAN_SINGLE_OBJ
// use these macros to construct channels in objects NOT derived from ac_named
// WARNING:  there may be only one instance of this object in the design
#define AC_CHAN_SINGLE_OBJ(varname) varname(AC_QUOTED_STRING(varname),NULL,"class",-1)
#define AC_CHAN_SINGLE_OBJ_INIT(varname,init) varname(AC_QUOTED_STRING(varname),NULL,"class",-1, init)
#define AC_CHAN_SINGLE_OBJ_VAL(varname,init,val) varname(AC_QUOTED_STRING(varname),NULL,"class",-1,init,val)
// used these macros to construct channels of object derived from ac_named -- multiple instance supported -- be sure to "name " each instance in a unique way 
// Note: compiler warning regarding: 'this' : used in base member initializer list ... can be ignored
#define AC_CHAN_NAMED_OBJ(varname) varname(AC_QUOTED_STRING(varname),this,"class",-1)
#define AC_CHAN_NAMED_OBJ_INIT(varname,init) varname(AC_QUOTED_STRING(varname),this,"class",-1, init)
#define AC_CHAN_NAMED_OBJ_VAL(varname,init,val) varname(AC_QUOTED_STRING(varname),this,"class",-1,init,val)


class ac_channel_id : public ac_named {
public:

	// default ctr
	ac_channel_id()
	: ac_named()
	, d_identity()
	, d_name_string()
	, d_is_class_member(false)
	{}

	ac_channel_id(const char *varname, AC_CHAN_OWNER owner, AC_CHAN_CTXT filepath, int lineno)
	: ac_named()
	, d_identity()
	, d_name_string()
	, d_is_class_member(false)
	{
		// setup ac_named
		ac_init_named_member(varname);
		
		char lineBuf[20];
		sprintf(lineBuf,"%d",lineno);
		
		// if there is a valid line number the context is the file path
		if (lineno >= 0) {
			const char *fileleaf = strrchr(filepath, '/');
			if (fileleaf == NULL) {
				// HACK: might be back slash delimiters
				fileleaf = strrchr(filepath, '\\');
				if (fileleaf == NULL) {
					fileleaf = filepath;
				}
				else fileleaf++;
			}
			// incr past divider
			else fileleaf++;
			std::string leaf(fileleaf);
			// go to lower case to hack around an issue found on windows
			// where case does not seem to be presented consistently
			std::transform(leaf.begin(), leaf.end(), leaf.begin(), ::tolower); 
			
			// sprintf(d_identity,"%s@%s@%s",varname,leaf.data(),lineBuf);
			d_identity.assign(varname);
			d_identity.push_back('@');
			d_identity.append(leaf);
			d_identity.push_back('@');
			d_identity.append(lineBuf);
			
			// name string for output messages
			// sprintf(d_name_string, "%s (line %s file %s)", varname, lineBuf, leaf.data());
			d_name_string.assign(varname);
			d_name_string.append(" (line ");
			d_name_string.append(lineBuf);
			d_name_string.append(" file ");
			d_name_string.append(leaf);
			d_name_string.push_back(')');
		}
		else {  
			d_is_class_member = true;
			// now register for a call back for when the TR init call is made 
			if (owner) owner->reg_child(this);
			set_member_context(NULL);
		}
   	}

	virtual ~ac_channel_id() {}
 
   	// override implementation appends name to context and set ID 
	virtual void set_member_context(const char *ctxt){
		// we now know the name of the owner and its context
		const char *chan_member_name = get_named();
		
		// sprintf(d_identity,"%s@%s@-1", chan_member_name, ctxt);
		if (ctxt && (*ctxt != '\0')) {
			d_identity.assign(ctxt);
			d_identity.push_back('.');
			d_identity.append(chan_member_name);
			d_name_string.assign(ctxt);
			d_name_string.push_back('.');
			d_name_string.append(chan_member_name);
		}
		else {
			d_identity.assign(chan_member_name);
			d_name_string.assign(chan_member_name);
		}
		d_identity.push_back('@');
		d_identity.append("class");
		d_identity.append("@-1");
	
		// and finish name string
		d_name_string.append(" (class member)");
	}

	void id_init(const char *varname) {
   		// setup ac_named (may be redundant here ?
		ac_init_named_member(varname);

		d_identity.assign(varname);
		// name string for output messages
		// sprintf(d_name_string, "%s (primary input)", varname);	
		d_name_string.assign(varname);
		d_name_string.append("(primary input)");
	}
   
	bool is_class_member() { return d_is_class_member; }
	bool has_identity() { return d_identity.length() ? true : false; }
	const char *identity() const { return d_identity.length() ?d_identity.c_str() :"<none>"; } 
	const char *print_name() const { return d_name_string.length() ?d_name_string.c_str() :"<name not registered>"; } 
private:
   string d_identity;
   string d_name_string;
   bool d_is_class_member;
};

#endif
