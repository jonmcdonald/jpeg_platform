#pragma once
#include "mgc_vista_schematics.h"
$includes_begin;
#include <systemc.h>
#include "../models/axi2apb_bridge_model.h"
#include "../models/apb_bus_model.h"
#include "models_catalogue.h"
$includes_end;

$module_begin("IO_SS_TLM_PREDICTOR");
SC_MODULE(IO_SS_TLM_PREDICTOR) {
public:
  IO_SS_TLM_PREDICTOR(::sc_core::sc_module_name name):
    ::sc_core::sc_module(name)
    $initialization_begin
$init("axi_slave"),
axi_slave("axi_slave")
$end
$init("UARTRXD"),
UARTRXD("UARTRXD")
$end
$init("UARTTXD"),
UARTTXD("UARTTXD")
$end
$init("AXI2APB_Bridge"),
AXI2APB_Bridge(0)
$end
$init("APB_BUS"),
APB_BUS(0)
$end
$init("UART"),
UART(0)
$end
    $initialization_end
{
    $elaboration_begin;
$create_component("AXI2APB_Bridge");
AXI2APB_Bridge = new axi2apb_bridge_pvt("AXI2APB_Bridge");
$end;
$create_component("APB_BUS");
APB_BUS = new apb_bus_pvt("APB_BUS");
$end;
$create_component("UART");
UART = new UART_PL011_pvt("UART");
$end;
$bind("axi_slave","AXI2APB_Bridge->axi_slave");
vista_bind(axi_slave, AXI2APB_Bridge->axi_slave);
$end;
$bind("UART->UARTTXD","UARTTXD");
vista_bind(UART->UARTTXD, UARTTXD);
$end;
$bind("UARTRXD","UART->UARTRXD");
vista_bind(UARTRXD, UART->UARTRXD);
$end;
$bind("AXI2APB_Bridge->apb_master","APB_BUS->bus_slave");
vista_bind(AXI2APB_Bridge->apb_master, APB_BUS->bus_slave);
$end;
$bind("APB_BUS->uart","UART->AMBA_APB");
vista_bind(APB_BUS->uart, UART->AMBA_APB);
$end;
    $elaboration_end;
  $vlnv_assign_begin;
m_library = "Schematics";
m_vendor = "";
m_version = "";
  $vlnv_assign_end;
  }
  ~IO_SS_TLM_PREDICTOR() {
    $destructor_begin;
$destruct_component("AXI2APB_Bridge");
delete AXI2APB_Bridge; AXI2APB_Bridge = 0;
$end;
$destruct_component("APB_BUS");
delete APB_BUS; APB_BUS = 0;
$end;
$destruct_component("UART");
delete UART; UART = 0;
$end;
    $destructor_end;
  }
public:
  $fields_begin;
$socket("axi_slave");
tlm::tlm_target_socket< 64U,axi_protocol_types,1,sc_core::SC_ONE_OR_MORE_BOUND > axi_slave;
$end;
$socket("UARTRXD");
tlm::tlm_target_socket< 8U,tlm::tlm_base_protocol_types,1,sc_core::SC_ONE_OR_MORE_BOUND > UARTRXD;
$end;
$socket("UARTTXD");
tlm::tlm_initiator_socket< 8U,tlm::tlm_base_protocol_types,1,sc_core::SC_ONE_OR_MORE_BOUND > UARTTXD;
$end;
$component("AXI2APB_Bridge");
axi2apb_bridge_pvt *AXI2APB_Bridge;
$end;
$component("APB_BUS");
apb_bus_pvt *APB_BUS;
$end;
$component("UART");
UART_PL011_pvt *UART;
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
