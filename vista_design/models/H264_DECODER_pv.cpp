
/**************************************************************/
/*                                                            */
/*      Copyright Mentor Graphics Corporation 2006 - 2012     */
/*                  All Rights Reserved                       */
/*                                                            */
/*       THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY      */
/*         INFORMATION WHICH IS THE PROPERTY OF MENTOR        */
/*         GRAPHICS CORPORATION OR ITS LICENSORS AND IS       */
/*                 SUBJECT TO LICENSE TERMS.                  */
/*                                                            */
/**************************************************************/

//*<
//* Generated By Model Builder, Mentor Graphics Computer Systems, Inc.
//*
//* This file contains the PV class for H264_DECODER.
//* This is a template file: You may modify this file to implement the
//* behavior of your component.
//*
//* Model Builder version: 3.1.0
//* Generated on: Apr. 10, 2011 02:32:49 PM, (user: dvolinsk)
//* Automatically merged on: Apr. 11, 2011 11:34:07 AM, (user: dvolinsk)
//* Automatically merged on: Apr. 12, 2011 01:39:08 PM, (user: dvolinsk)
//* Automatically merged on: Apr. 12, 2011 01:57:29 PM, (user: dvolinsk)
//* Automatically merged on: Apr. 12, 2011 01:58:06 PM, (user: dvolinsk)
//* Automatically merged on: Apr. 12, 2011 01:59:01 PM, (user: dvolinsk)
//* Automatically merged on: Apr. 12, 2011 02:10:48 PM, (user: dvolinsk)
//* Automatically merged on: Apr. 12, 2011 02:11:13 PM, (user: dvolinsk)
//* Automatically merged on: Feb. 22, 2012 07:00:33 PM, (user: zamir)
//* Automatically merged on: Aug. 16, 2012 12:58:03 PM, (user: araskin)
//*>



#include "H264_DECODER_pv.h"
#include "global_sc.h"

#include <iostream>

// Constructor
H264_DECODER_pv::H264_DECODER_pv(sc_module_name module_name) : H264_DECODER_pv_base(module_name) {
  cout << this->name () << " Constructor" << endl;
  //  irq.initialize (false);

  SC_THREAD(algorithm_thread);
  SC_THREAD(frame_ready_thread);
}

void H264_DECODER_pv::algorithm_thread() {
  while(true) {
    sc_core::wait(algorithm_event);
    master_read (BASE_BITSTREAM_ADDR, Frame, LENGTH);
    interrupt = 0;
    frame_done = 0;
    ldecod (Frame, LENGTH);
    interrupt = 1;
  }
}

void H264_DECODER_pv::frame_ready_thread() {
  while(true) {
    sc_core::wait(event_frame_ready);
    master_write(BASE_ADDR_Y, tlmDataBuffer, tlmDataLenByte);
    frame_done =  1;
  }
}


/////////////////////////////////////////////////////////////////////////////////
// Use these functions to define the behavior of your model when there is a
// write event on one of the registers as defined in the Model Builder form.
// These functions are called before the write callbacks on the port.
/////////////////////////////////////////////////////////////////////////////////

// Read callback for frame_done register.
unsigned long long H264_DECODER_pv::cb_read_frame_done()
{
  return frame_done;
}

// Write callback for start register.
// The newValue has been already assigned to the start register.
void H264_DECODER_pv::cb_write_start(unsigned long long newValue) 
{
    //cout << name () << " Function " << __FUNCTION__  << " newValue=" << newValue << endl;

  if (newValue == 1) {
    frame_done = 0;
    algorithm_event.notify();
  }
}

// Write callback for stop register.
// The newValue has been already assigned to the stop register.
void H264_DECODER_pv::cb_write_stop(unsigned long long newValue) 
{
   //cout << name () << " Function " << __FUNCTION__  << " newValue=" << newValue << endl;
}

// Write callback for frame_done register.
// The newValue has been already assigned to the frame_done register.
 void H264_DECODER_pv::cb_write_frame_done(unsigned long long newValue) 
{
  //cout << name () << " Function " << __FUNCTION__  << " newValue=" << newValue << endl;
  frame_done = 0;
  event_frame_done.notify();
 }


// Read callback for slave port.
// Returns true when successful.
bool H264_DECODER_pv::slave_callback_read(mb_address_type address, unsigned char* data, unsigned size) 
{
  //cout << name () << " Function " << __FUNCTION__  << endl;
  return true;
}

// Write callback for slave port.
// Returns true when successful.
bool H264_DECODER_pv::slave_callback_write(mb_address_type address, unsigned char* data, unsigned size) 
{
  //cout << name () << " Function " << __FUNCTION__  << endl;
  return true;
} 



unsigned H264_DECODER_pv::slave_callback_read_dbg(mb_address_type address, unsigned char* data, unsigned size) 
{
  //cout << name () << " Function " << __FUNCTION__  << endl;
  return 0;
} 

unsigned H264_DECODER_pv::slave_callback_write_dbg(mb_address_type address, unsigned char* data, unsigned size) 
{
  //cout << name () << " Function " << __FUNCTION__  << endl;
  return 0;
} 

bool H264_DECODER_pv::slave_get_direct_memory_ptr(mb_address_type address, tlm::tlm_dmi& dmiData) 
{
  //cout << name () << " Function " << __FUNCTION__  << endl;
  return false;
}

