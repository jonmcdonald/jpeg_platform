#ifndef _GLOBAL_SC_H_
#define _GLOBAL_SC_H_

#include "systemc.h"

extern sc_event event_transaction_req;
extern sc_event event_transaction_end;
extern unsigned char tlmDataBuffer[48];
extern unsigned int  tlmDataLenByte;

extern sc_event event_dump_one_frame_req;
extern sc_event event_dump_one_frame_end;
extern unsigned int  num_pending_frame;
extern bool flag_start_dump_frame;

extern sc_event event_deblock_rw_req;
extern sc_event event_deblock_rw_end;
extern unsigned int num_pending_block;
#endif
