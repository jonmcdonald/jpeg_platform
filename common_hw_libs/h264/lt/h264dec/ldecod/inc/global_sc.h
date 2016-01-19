#ifndef _GLOBAL_SC_H_
#define _GLOBAL_SC_H_

#include "systemc.h"

extern sc_event event_transaction_req;
extern sc_event event_transaction_end;
extern unsigned char *tlmDataBuffer;
extern unsigned int  tlmDataLenByte;
extern unsigned int tlmDataRows;
extern unsigned int tlmDataColumns;
extern sc_event      event_frame_done;
// extern 1bool          glb_frame_done;
extern sc_event      event_frame_ready;

#endif
