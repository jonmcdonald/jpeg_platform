
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
//* This file contains the PV class for UART_Visualization_Linux.
//* This is a template file: You may modify this file to implement the 
//* behavior of your component. 
//* 
//* Model Builder version: 3.0.2.beta1
//* Generated on: Oct. 24, 2010 12:15:47 PM, (user: monas)
//* Automatically merged on: Feb. 04, 2011 10:36:55 AM, (user: zamir)
//* Automatically merged on: Feb. 27, 2011 07:18:07 PM, (user: zamir)
//* Automatically merged on: Jul. 06, 2011 07:25:14 PM, (user: ibirger)
//* Automatically merged on: Oct. 28, 2011 02:11:14 PM, (user: zamir)
//* Automatically merged on: Feb. 22, 2012 07:00:44 PM, (user: zamir)
//* Automatically merged on: Aug. 16, 2012 12:58:12 PM, (user: araskin)
//* Automatically merged on: Dec. 03, 2012 05:56:35 PM, (user: mbradley)
//* Automatically merged on: Aug. 26, 2013 11:45:50 AM, (user: ahanga)
//* Automatically merged on: Oct. 11, 2013 04:28:23 PM, (user: johns)
//* Automatically merged on: Jan. 09, 2014 07:31:19 PM, (user: johns)
//* Automatically merged on: Jan. 18, 2016 04:09:46 PM, (user: mbradley)
//*>


#pragma once

#include "UART_Visualization_Linux_model.h"

#include <unistd.h>
#include <errno.h>

using namespace tlm;

//This class inherits from the UART_Visualization_Linux_pv_base class
class UART_Visualization_Linux_pv : public UART_Visualization_Linux_pv_base {
 public:
  typedef esl::tlm_types::Address mb_address_type;
 public:
  // Constructor
  // Do not add parameters here.
  // To add parameters - use the Model Builder form (under PV->Parameters tab)
  SC_HAS_PROCESS(UART_Visualization_Linux_pv);
  UART_Visualization_Linux_pv(sc_core::sc_module_name module_name);    

 protected:
  ////////////////////////////////////////
  // target ports read callbacks
  //////////////////////////////////////// 
  bool UARTRXD_callback_read(mb_address_type address, unsigned char* data, unsigned size);
  
  unsigned UARTRXD_callback_read_dbg(mb_address_type address, unsigned char* data, unsigned size); 

 protected:
  ////////////////////////////////////////
  // target ports write callbacks
  //////////////////////////////////////// 
  bool UARTRXD_callback_write(mb_address_type address, unsigned char* data, unsigned size);
  
  unsigned UARTRXD_callback_write_dbg(mb_address_type address, unsigned char* data, unsigned size);  
  bool UARTRXD_get_direct_memory_ptr(mb_address_type address, tlm::tlm_dmi& dmiData);  

 protected:
  ////////////////////////////////////////
  // signals callbacks
  //////////////////////////////////////// 
  void nUARTCTS_callback(); 
  void UARTEN_callback();

private:
  mb::physical_io::console* con_io; 
  mb::physical_io::buffered_write* m_buffered_write;
 
  sc_time m_console_read_wait_period;
  bool  m_console_receive_enable;  

  void receive_from_console();
  void to_UART(char *buf, int len);
  void set_receive_from_console_polling_time();
};

