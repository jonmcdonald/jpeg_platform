
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
//* This file contains the timing class for H264_DECODER.
//*
//* This is a template file: You may modify this file to implement the 
//* timing behavior of your component.
//* The functions below allows advance users to customize the timing behavior of your model.
//* 
//* Model Builder version: 3.1.0
//* Generated on: Apr. 21, 2011 05:46:23 PM, (user: awinterg)
//* Automatically merged on: Sep. 07, 2011 11:39:31 AM, (user: awinterg)
//* Automatically merged on: Feb. 22, 2012 07:00:33 PM, (user: zamir)
//* Automatically merged on: Apr. 16, 2014 02:34:35 AM, (user: awinterg)
//*>


#include "H264_DECODER_t.h"


#include <iostream>

using namespace std;
using namespace sc_dt;
using namespace sc_core;

H264_DECODER_t_base* H264_DECODER_t_base::create_t(const char* name, long simulation) {
  return new H264_DECODER_t(name, simulation);
}

H264_DECODER_t::H264_DECODER_t(sc_module_name _name, long simulation)
  : H264_DECODER_t_base(_name, simulation) {
  
}

