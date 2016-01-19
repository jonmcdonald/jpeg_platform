//+FHDR--------------------------------------------------------------------
// Copyright (c) 2006, Global UniChip Corporation (UniChip).
// UniChip's Proprietary/Confidential
//
// All rights reserved. No part of this design may be reproduced or stored
// in a retrieval system, or transmitted, in any form or by any means,
// electronic, mechanical, photocopying, recording, or otherwise,
// without prior written permission of the Global UniChip Corporation.
// Unauthorized reproduction, duplication, use, or disclosure of this
// design will be deemed as infringement.
//-------------------------------------------------------------------------
// FILE NAME            : uc_mem_map.h
// TYPE                 : SystemC Header File
// AUTHOR               : Gecko Lin
//-FHDR--------------------------------------------------------------------

  #ifndef _UC_MEM_MAP_H
  #define _UC_MEM_MAP_H
  #include "uc_typedef.h"
  #include "string.h"
  
  struct struct_mem_map
  {
    std::string name;
    UC_U32 base_address;
    UC_U32 size_address;
  };
  
  static const struct_mem_map AHB_decoding_table[] =
  { {"i_mem_smc",       0x28000000,       0x4000000},
    {"i_mem_ssram",     0x38000000,       0x4000000},
    {"i_mem_sdram",     0x3c000000,       0x4000000},
    {"i_vic1",          0x30020000,       0x1000},
    {"i_vic2",          0x30021000,       0x1000},
    {"i_dmac",          0x300b0000,       0x10000},
    {"i_apb_brg",       0x32000000,       0x190000},
    {"",                0,                0}
  };
    
  static const struct_mem_map APB_decoding_table[] =
  {  
    {"i_sysc",          0x32000000,       0x10000},
    {"i_i2c_vfe",       0x32080000,       0x10000},
    {"i_i2c_i2s",       0x32090000,       0x10000},
    {"i_i2c_led",       0x320a0000,       0x10000},
    {"i_uart",          0x320b0000,       0x10000},
    {"i_dai",           0x320f0000,       0x10000},
    {"i_gpio",          0x32140000,       0x10000},
    {"i_timer1",        0x32150000,       0x10000},
    {"i_timer2",        0x32160000,       0x10000},
    {"i_wdt",           0x32170000,       0x10000},
    {"",                0,                0}
  };
  
  struct struct_master_map
  {
    std::string name;
    UC_U32 Rsv;
  };
    
  static const struct_master_map AHB_master_map[] =
  {  
    {"i_ahb_master",    0},
    {"i_dmac_m1",       0},
    {"i_dmac_m2",       0},
    {"",                0}
  };
    
  static const struct_master_map APB_master_map[] =
  {  
    {"i_ahb_top",       0},
    {"",                0}
  };

  #endif // _UC_MEM_MAP_H

//-------------------------------------------------------------------------
// REVISION HISTORY
// $Date: 2007/12/19 09:28:29 $
// $Log: uc_mem_map.h,v $
// Revision 1.1  2007/12/19 09:28:29  eng01e
// *** empty log message ***
//
// Revision 1.1  2007/07/16 08:21:59  eng01c
// *** empty log message ***
//
