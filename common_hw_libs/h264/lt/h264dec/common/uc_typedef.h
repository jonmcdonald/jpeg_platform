//+FHDR-----------------------------------------------------------------------
// Copyright (c) 2006, Global UniChip Corporation (UniChip).
// UniChip's Proprietary/Confidential
//
// All rights reserved. No part of this design may be reproduced or stored
// in a retrieval system, or transmitted, in any form or by any means,
// electronic, mechanical, photocopying, recording, or otherwise,
// without prior written permission of the Global UniChip Corporation.
// Unauthorized reproduction, duplication, use, or disclosure of this
// design will be deemed as infringement.
//----------------------------------------------------------------------------
// FILE NAME            : uc_typedef.h
// TYPE                 : Type Define Header File
// AUTHOR               : HS Liao
//----------------------------------------------------------------------------
// DESCRIPTION          : Unichip DataType Definition
//-FHDR-----------------------------------------------------------------------

  #ifndef UC_TYPEDEF_H
  #define UC_TYPEDEF_H

  typedef unsigned char  UC_U8;
  typedef unsigned short UC_U16;
  typedef unsigned int   UC_U32;
  typedef unsigned long  UC_U64;

  typedef char  UC_S8;
  typedef short UC_S16;
  typedef int   UC_S32;
  typedef long  UC_S64;
  
  enum HTRANS_MODE 
  {
    IDLE         = 0x0,
    BUSY         = 0x1,  
    NONSEQ       = 0x2,
    SEQ          = 0x3
  };

  enum BURST_MODE
  {
    BURST_SINGLE       = 0x0,
    BURST_INCR         = 0x1,
    BURST_WRAP4        = 0x2,
    BURST_INCR4        = 0x3,
    BURST_WRAP8        = 0x4,
    BURST_INCR8        = 0x5,
    BURST_WRAP16       = 0x6,
    BURST_INCR16       = 0x7
  };

  enum RESP_MODE 
  {
    RESP_OKAY        = 0x0,
    RESP_ERROR       = 0x1,
    RESP_RETRY       = 0x2,
    RESP_SPLIT       = 0x3   
  };

  enum BUS_WIDTH 
  {
    BYTE                = 0x0,
    HALFWORD            = 0x1,
    WORD                = 0x2,
    TWO_WORD            = 0x3,
    FOUR_WORD           = 0x4,
    EIGHT_WORD          = 0x5,
    SIXTEEN_WORD        = 0x6,
    THIRTY_TWO_WORD     = 0x7
  };

  enum READ_WRITE
  {
    READ         = 0x0,
    WRITE        = 0x1
  };



  
  #endif // UC_TYPEDEF_H

//----------------------------------------------------------------------------
// REVISION HISTORY
// $Date: 2007/12/19 09:28:30 $
// $Log: uc_typedef.h,v $
// Revision 1.1  2007/12/19 09:28:30  eng01e
// *** empty log message ***
//
// Revision 1.4  2007/07/16 08:21:00  eng01c
// *** empty log message ***
//
// Revision 1.3  2007/06/13 03:32:31  eng01c
// Define some enums : HTRANS_MODE   BURST_MODE   RESP_MODE   BUS_WIDTH and READ_WRITE
//
// Revision 1.2  2007/02/08 02:58:41  eng01b
// *** empty log message ***
//
// Revision 1.1.1.1  2006/11/23 07:49:38  eng01
// import sources
//
