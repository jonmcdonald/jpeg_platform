#pragma once
#include "mgc_vista_schematics.h"
$includes_begin;
#include <systemc.h>
#include "../models/RST_CLK_CTRL_model.h"
#include "UART_VIS.h"
$includes_end;

$module_begin("UART_SUBSYSTEM");
SC_MODULE(UART_SUBSYSTEM) {
public:
  UART_SUBSYSTEM(::sc_core::sc_module_name name):
    ::sc_core::sc_module(name)
    $initialization_begin
$init("uart_regs"),
uart_regs("uart_regs")
$end
$init("uart_ctrl"),
uart_ctrl("uart_ctrl")
$end
$init("UARTINTR"),
UARTINTR("UARTINTR")
$end
$init("RST_CLK_CTRL"),
RST_CLK_CTRL(0)
$end
$init("UARTV"),
UARTV(0)
$end
    $initialization_end
{
    $elaboration_begin;
$create_component("RST_CLK_CTRL");
RST_CLK_CTRL = new RST_CLK_CTRL_pvt("RST_CLK_CTRL");
$end;
$create_component("UARTV");
UARTV = new UART_VIS("UARTV");
$end;
$bind("uart_ctrl","RST_CLK_CTRL->slave0");
vista_bind(uart_ctrl, RST_CLK_CTRL->slave0);
$end;
$bind("UARTV->UARTINTR","UARTINTR");
vista_bind(UARTV->UARTINTR, UARTINTR);
$end;
$bind("uart_regs","UARTV->AMBA_APB");
vista_bind(uart_regs, UARTV->AMBA_APB);
$end;
$bind("RST_CLK_CTRL->UART0_VIS_EN","UARTV->UARTEN");
vista_bind(RST_CLK_CTRL->UART0_VIS_EN, UARTV->UARTEN);
$end;
    $elaboration_end;
  $vlnv_assign_begin;
m_library = "Schematics";
m_vendor = "";
m_version = "";
  $vlnv_assign_end;
  }
  ~UART_SUBSYSTEM() {
    $destructor_begin;
$destruct_component("RST_CLK_CTRL");
delete RST_CLK_CTRL; RST_CLK_CTRL = 0;
$end;
$destruct_component("UARTV");
delete UARTV; UARTV = 0;
$end;
    $destructor_end;
  }
public:
  $fields_begin;
$socket("uart_regs");
tlm::tlm_target_socket< 16u,tlm::tlm_base_protocol_types,1,SC_ONE_OR_MORE_BOUND > uart_regs;
$end;
$socket("uart_ctrl");
tlm::tlm_target_socket< 32u,tlm::tlm_base_protocol_types,1,SC_ONE_OR_MORE_BOUND > uart_ctrl;
$end;
$socket("UARTINTR");
tlm::tlm_initiator_socket< 1u,tlm::tlm_base_protocol_types,1,SC_ZERO_OR_MORE_BOUND > UARTINTR;
$end;
$component("RST_CLK_CTRL");
RST_CLK_CTRL_pvt *RST_CLK_CTRL;
$end;
$component("UARTV");
UART_VIS *UARTV;
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
