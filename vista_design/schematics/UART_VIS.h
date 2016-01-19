#pragma once
#include "mgc_vista_schematics.h"
$includes_begin;
#include <systemc.h>
#include "models_catalogue.h"
#include "../models/UART_Visualization_Linux_model.h"
$includes_end;

$module_begin("UART_VIS");
SC_MODULE(UART_VIS) {
public:
  UART_VIS(::sc_core::sc_module_name name):
    ::sc_core::sc_module(name)
    $initialization_begin
$init("AMBA_APB"),
AMBA_APB("AMBA_APB")
$end
$init("UARTEN"),
UARTEN("UARTEN")
$end
$init("UARTINTR"),
UARTINTR("UARTINTR")
$end
$init("IP"),
IP(0)
$end
$init("VIS"),
VIS(0)
$end
    $initialization_end
{
    $elaboration_begin;
$create_component("IP");
IP = new UART_PL011_pvt("IP");
$end;
$create_component("VIS");
VIS = new UART_Visualization_Linux_pvt("VIS");
$end;
$bind("UARTEN","VIS->UARTEN");
UARTEN.bind(VIS->UARTEN);
$end;
$bind("AMBA_APB","IP->AMBA_APB");
AMBA_APB.bind(IP->AMBA_APB);
$end;
$bind("IP->UARTTXD","VIS->UARTRXD");
IP->UARTTXD.bind(VIS->UARTRXD);
$end;
$bind("VIS->UARTTXD","IP->UARTRXD");
VIS->UARTTXD.bind(IP->UARTRXD);
$end;
$bind("IP->nUARTRTS","VIS->nUARTCTS");
IP->nUARTRTS.bind(VIS->nUARTCTS);
$end;
$bind("VIS->nUARTRTS","IP->nUARTCTS");
VIS->nUARTRTS.bind(IP->nUARTCTS);
$end;
$bind("IP->UARTINTR","UARTINTR");
vista_bind(IP->UARTINTR, UARTINTR);
$end;
    $elaboration_end;
  $vlnv_assign_begin;
m_library = "Schematics";
m_vendor = "Mentor.com";
m_version = "1.0";
  $vlnv_assign_end;
  }
  ~UART_VIS() {
    $destructor_begin;
$destruct_component("IP");
delete IP; IP = 0;
$end;
$destruct_component("VIS");
delete VIS; VIS = 0;
$end;
    $destructor_end;
  }
public:
  $fields_begin;
$socket("AMBA_APB");
tlm::tlm_target_socket< 16u,tlm::tlm_base_protocol_types,1,SC_ONE_OR_MORE_BOUND > AMBA_APB;
$end;
$socket("UARTEN");
tlm::tlm_target_socket< 1u,tlm::tlm_base_protocol_types,1,SC_ZERO_OR_MORE_BOUND > UARTEN;
$end;
$socket("UARTINTR");
tlm::tlm_initiator_socket< 1U,tlm::tlm_base_protocol_types,1,sc_core::SC_ZERO_OR_MORE_BOUND > UARTINTR;
$end;
$component("IP");
UART_PL011_pvt *IP;
$end;
$component("VIS");
UART_Visualization_Linux_pvt *VIS;
$end;
  $fields_end;
  $vlnv_decl_begin;
public:
const char* m_library;
const char* m_vendor;
const char* m_version;
  $vlnv_decl_end;
};
$module_end;