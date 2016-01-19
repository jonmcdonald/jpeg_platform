#pragma once
#include "mgc_vista_schematics.h"
$includes_begin;
#include <systemc.h>
#include "../models/JPEG_ENCODER_model.h"
#include "../models/IO_model.h"
$includes_end;

$module_begin("JPEG_SUBSYSTEM");
SC_MODULE(JPEG_SUBSYSTEM) {
public:
  JPEG_SUBSYSTEM(::sc_core::sc_module_name name):
    ::sc_core::sc_module(name)
    $initialization_begin
$init("jpeg_slave"),
jpeg_slave("jpeg_slave")
$end
$init("jpeg_master"),
jpeg_master("jpeg_master")
$end
$init("irq"),
irq("irq")
$end
$init("JPEG"),
JPEG(0)
$end
    $initialization_end
{
    $elaboration_begin;
$create_component("JPEG");
JPEG = new JPEG_ENCODER_pvt("JPEG");
$end;
$bind("jpeg_slave","JPEG->slave");
jpeg_slave.bind(JPEG->slave);
$end;
$bind("JPEG->master","jpeg_master");
JPEG->master.bind(jpeg_master);
$end;
$bind("JPEG->irq","irq");
JPEG->irq.bind(irq);
$end;
    $elaboration_end;
  $vlnv_assign_begin;
m_library = "Schematics";
m_vendor = "";
m_version = "";
  $vlnv_assign_end;
  }
  ~JPEG_SUBSYSTEM() {
    $destructor_begin;
$destruct_component("JPEG");
delete JPEG; JPEG = 0;
$end;
    $destructor_end;
  }
public:
  $fields_begin;
$socket("jpeg_slave");
tlm::tlm_target_socket< 32U,axi_protocol_types,1,sc_core::SC_ONE_OR_MORE_BOUND > jpeg_slave;
$end;
$socket("jpeg_master");
tlm::tlm_initiator_socket< 32U,axi_protocol_types,1,sc_core::SC_ONE_OR_MORE_BOUND > jpeg_master;
$end;
$socket("irq");
tlm::tlm_initiator_socket< 1U,tlm::tlm_base_protocol_types,1,sc_core::SC_ONE_OR_MORE_BOUND > irq;
$end;
$component("JPEG");
JPEG_ENCODER_pvt *JPEG;
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