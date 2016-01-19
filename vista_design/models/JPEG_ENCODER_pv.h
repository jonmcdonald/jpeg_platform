
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
//* This file contains the PV class for JPEG_ENCODER.
//* This is a template file: You may modify this file to implement the
//* behavior of your component.
//*
//* Model Builder version: 3.1.0
//* Generated on: Mar. 23, 2011 03:35:18 PM, (user: dvolinsk)
//* Automatically merged on: Feb. 22, 2012 07:00:35 PM, (user: zamir)
//*>


#pragma once

#include "JPEG_ENCODER_model.h"
#include "jpeg.h"

using namespace tlm;

// This class inherits from the JPEG_ENCODER_pv_base class
class JPEG_ENCODER_pv : public JPEG_ENCODER_pv_base {
 public:
  typedef esl::tlm_types::Address mb_address_type;
 public:
  // Constructor
  // Do not add parameters here.
  // To add parameters - use the Model Builder form (under PV->Parameters tab)
  SC_HAS_PROCESS(JPEG_ENCODER_pv);
  JPEG_ENCODER_pv(sc_module_name module_name);

 protected:
  ////////////////////////////////////////
  // read callbacks of registers
  //////////////////////////////////////////
  unsigned int cb_read_status();
  unsigned int cb_read_start();

 protected:
  /////////////////////////////////////////
  // write callbacks of registers
  //////////////////////////////////////////
  void cb_write_status(unsigned int newValue);
  void cb_write_start(unsigned int newValue);

 protected:
  ////////////////////////////////////////
  // target ports read callbacks
  ////////////////////////////////////////
  bool slave_callback_read(mb_address_type address, unsigned char* data, unsigned size);
  unsigned slave_callback_read_dbg(mb_address_type address, unsigned char* data, unsigned size);

 protected:
  ////////////////////////////////////////
  // target ports write callbacks
  ////////////////////////////////////////
  bool slave_callback_write(mb_address_type address, unsigned char* data, unsigned size);
  unsigned slave_callback_write_dbg(mb_address_type address, unsigned char* data, unsigned size);
  bool slave_get_direct_memory_ptr(mb_address_type address, tlm::tlm_dmi& dmiData);

 private:
  rgb_t full_rgb_temp [3 * MAX_ROWS * MAX_COLUMNS];
  ac_channel <bool> start_jpeg_temp;
  ac_channel <bool> done_jpeg_temp;
  unsigned char jpeg_data_temp [3 * MAX_ROWS * MAX_COLUMNS];
  uint21 jpeg_data_len_temp;

};

