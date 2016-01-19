
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
//* This file is write protected. 
//* DO NOT MODIFY THIS FILE.
//*
//* This file is generated according to the parameters in the 
//* Model Builder form.
//* This class contains the infrastructure to define the behavior of the component.
//* The IO_pv will be derived from this class.
//*
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>


#include "IO_model.h"

#ifdef MODEL_BUILDER_VERSION_NUMBER
#if 4100 != MODEL_BUILDER_VERSION_NUMBER
#error "This model was generated using Model Builder version: 4.1.0. Please regenerate model."
#endif
#else
#error "Please regenerate model."
#endif

using namespace sc_core;
using namespace sc_dt;
using namespace std;


IO_pv_base::IO_pv_base(sc_module_name& module_name) : 
  IO_pv_base_mb_compatibility(module_name),
  IO_pv_base_parameters(this),
  VISTA_MB_PV_INIT_MEMBER(master),
  VISTA_MB_PV_INIT_MEMBER(slave),
  VISTA_MB_PV_INIT_MEMBER(slave_memory),
  inputaddr("inputaddr", &slave_memory, ((0x0) / 4), ( (0) + (((0x0) % 4) << 3)), ( (31) + (((0x0) % 4) << 3)),  (32), mb::tlm20::WRITE_ACCESS),
  inputlength("inputlength", &slave_memory, ((0x4) / 4), ( (0) + (((0x4) % 4) << 3)), ( (31) + (((0x4) % 4) << 3)),  (32), mb::tlm20::WRITE_ACCESS),
  outputaddr("outputaddr", &slave_memory, ((0x8) / 4), ( (0) + (((0x8) % 4) << 3)), ( (31) + (((0x8) % 4) << 3)),  (32), mb::tlm20::WRITE_ACCESS),
  upload("upload", &slave_memory, ((0xC) / 4), ( (0) + (((0xC) % 4) << 3)), ( (31) + (((0xC) % 4) << 3)),  (32), mb::tlm20::WRITE_ACCESS),
  download("download", &slave_memory, ((0x10) / 4), ( (0) + (((0x10) % 4) << 3)), ( (31) + (((0x10) % 4) << 3)),  (32), mb::tlm20::WRITE_ACCESS) {


  slave_memory.setPortIndex("slave");
  payload_on_stack = 0;
  payload_for_token = 0;

  inputaddr.setResetValue(0);
  download.setResetValue(0);
  inputlength.setResetValue(0);
  outputaddr.setResetValue(0);
  upload.setResetValue(0);
  VISTA_MB_PV_SET_SELF_TRANSPORT_DBG(upload);
  VISTA_MB_PV_SET_SELF_TRANSPORT_DBG(download);
  VISTA_MB_PV_SET_SELF_WRITE_CB(upload);
  VISTA_MB_PV_SET_SELF_WRITE_CB(download);
  VISTA_MB_PV_SET_TARGET_GET_DMI_DELAY_CB(slave);
  VISTA_MB_PV_BIND_FW_PROCESS_TO_MEMORY_DEFAULT_IF(slave);
  VISTA_MB_PV_BIND_BW_PROCESS_TO_MASTER(master);
  VISTA_MB_PV_REGISTER_SELF_FW_PROCESS_CALLBACKS(slave);
  VISTA_MB_PV_REGISTER_SELF_BW_PROCESS_CALLBACKS(master);

}

void IO_pv_base::reset_model() {
  // reset all registers


  inputaddr.setResetValue(0);
  download.setResetValue(0);
  inputlength.setResetValue(0);
  outputaddr.setResetValue(0);
  upload.setResetValue(0);
  
  // reset value for output signals

  // reset value for input signals

}

IO_pv_base_parameters::IO_pv_base_parameters(sc_object* object) :
  m_object(object),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, clock, sc_core::sc_time(10, sc_core::SC_NS)),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, axi_clock, clock),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, nominal_voltage, 1),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, mb_debug, false),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, call_to_default_if, false),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, verbose_parameters, true),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, dmi_enabled, true),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, warning_level, "WARNING"),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, slave_pipeline_length, 2),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, slave_outstanding, 1),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, slave_read_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, slave_write_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, master_outstanding, 1),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, master_read_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, master_write_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, bmpfilename, "image_data/fractal_800x600.bmp"),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, jpgfilename, "generated_fractal.jpg")
{
  if(verbose_parameters) print_parameters();
}

void IO_pv_base_parameters::print_parameters()
{
  const char* name;
  if (!strcmp(m_object->basename(), "PV"))
    name = m_object->get_parent()->name();
  else
    name = m_object->name();

  std::cout.setf(ios::hex, ios::basefield);
  std::cout.setf(ios::showbase);
  std::cout << name << "::parameters:\n";
  std::cout << "\tclock = " << clock << "\n";
  std::cout << "\taxi_clock = " << axi_clock << "\n";
  std::cout << "\tnominal_voltage = " << nominal_voltage << "\n";
  std::cout << "\tmb_debug = " << mb_debug << "\n";
  std::cout << "\tcall_to_default_if = " << call_to_default_if << "\n";
  std::cout << "\tverbose_parameters = " << verbose_parameters << "\n";
  std::cout << "\tdmi_enabled = " << dmi_enabled << "\n";
  std::cout << "\twarning_level = " << warning_level << "\n";
  std::cout << "\tslave_pipeline_length = " << slave_pipeline_length << "\n";
  std::cout << "\tslave_outstanding = " << slave_outstanding << "\n";
  std::cout << "\tslave_read_data_queue_size = " << slave_read_data_queue_size << "\n";
  std::cout << "\tslave_write_data_queue_size = " << slave_write_data_queue_size << "\n";
  std::cout << "\tmaster_outstanding = " << master_outstanding << "\n";
  std::cout << "\tmaster_read_data_queue_size = " << master_read_data_queue_size << "\n";
  std::cout << "\tmaster_write_data_queue_size = " << master_write_data_queue_size << "\n";
  std::cout << "\tbmpfilename = " << bmpfilename << "\n";
  std::cout << "\tjpgfilename = " << jpgfilename << "\n";
  std::cout.unsetf(ios::showbase);
  std::cout.setf(ios::dec, ios::basefield);
  std::cout << std::endl;
}


void IO_pv_base::end_of_elaboration() {
  BASE_TYPE::end_of_elaboration();
  slave_memory.bind_bw_direct_mem_if(slave[0]);
}


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
//* This file is write protected.
//* DO NOT MODIFY THIS FILE.
//*
//* This file is used only for learning the component.
//* It contains the machine architecture class for your IO model.
//* 
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>

#include "IO_model.h"
#include "model_builder.h"

using namespace ::mb::utl::minmax;

IO_machine_arch::IO_machine_arch()
    : MachineArch(2, 5), 
    inputaddr(0x0), 
    inputlength(0x4), 
    outputaddr(0x8), 
    upload(0xC), 
    download(0x10) {
    sc_dt::uint64* values = m_values;
    upload.setValuePtr(values);
    outputaddr.setValuePtr(values);
    inputlength.setValuePtr(values);
    inputaddr.setValuePtr(values);
    download.setValuePtr(values);
    
    reset();
}

////////////////////////////////////////////////////////////////////
//   function for port: slave
////////////////////////////////////////////////////////////////////


//////
// functions for port: slave, transaction: WRITE
//////

void IO_machine_arch::transaction_instance__slave__WRITE
  (
    sc_dt::uint64 _address,
    sc_dt::uint64 _size,
    sc_dt::uint64 _data, 
    bool has_been_error, 
    sc_dt::uint64 currentTime
  )
{
  if ( _address == inputaddr.getAddress()) { 
      inputaddr = _data &(0xffffffff);
  }
  if ( _address == inputlength.getAddress()) { 
      inputlength = _data &(0xffffffff);
  }
  if ( _address == outputaddr.getAddress()) { 
      outputaddr = _data &(0xffffffff);
  }
  if ( _address == upload.getAddress()) { 
      upload = _data &(0xffffffff);
  }
  if ( _address == download.getAddress()) { 
      download = _data &(0xffffffff);
  }
}

//////
// functions for port: slave, transaction: READ
//////

void IO_machine_arch::transaction_instance__slave__READ
  (
    sc_dt::uint64 _address,
    sc_dt::uint64 _size,
    sc_dt::uint64 _data, 
    bool has_been_error, 
    sc_dt::uint64 currentTime
  )
{
  
}

////////////////////////////////////////////////////////////////////
//   function for port: master
////////////////////////////////////////////////////////////////////


//////
// functions for port: master, transaction: WRITE
//////

void IO_machine_arch::transaction_instance__master__WRITE
  (
    sc_dt::uint64 _address,
    sc_dt::uint64 _size,
    sc_dt::uint64 _data, 
    bool has_been_error, 
    sc_dt::uint64 currentTime
  )
{
  
}

//////
// functions for port: master, transaction: READ
//////

void IO_machine_arch::transaction_instance__master__READ
  (
    sc_dt::uint64 _address,
    sc_dt::uint64 _size,
    sc_dt::uint64 _data, 
    bool has_been_error, 
    sc_dt::uint64 currentTime
  )
{
  
}

void IO_machine_arch::startTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 currentTime) {
  switch(portIndex) {
    case(0) : break;
    case(1) : break;
    default: break;
  }
}
VariableBase* IO_machine_arch::getVariableBasePtrByName(const char* name) {
  return NULL;
}
Register* IO_machine_arch::getRegisterByName(const char* name) {
  if (!strcmp(name, "upload")) {
    return &upload;
  }
  if (!strcmp(name, "outputaddr")) {
    return &outputaddr;
  }
  if (!strcmp(name, "inputlength")) {
    return &inputlength;
  }
  if (!strcmp(name, "inputaddr")) {
    return &inputaddr;
  }
  if (!strcmp(name, "download")) {
    return &download;
  }
  return 0;
}
void IO_machine_arch::reset() { 
    MachineArch::reset();
}
bool IO_machine_arch::getAddressRange(unsigned portIndex, sc_dt::uint64& begin, sc_dt::uint64& end) {
    switch(portIndex) {
        case(0) : begin = min(5, (sc_dt::uint64)(0x0), (sc_dt::uint64)(0x4), (sc_dt::uint64)(0x8), (sc_dt::uint64)(0xC), (sc_dt::uint64)(0x10));
            end = max(5, (sc_dt::uint64)(0x0 + ((32 + 7) >> 3)), (sc_dt::uint64)(0x4 + ((32 + 7) >> 3)), (sc_dt::uint64)(0x8 + ((32 + 7) >> 3)), (sc_dt::uint64)(0xC + ((32 + 7) >> 3)), (sc_dt::uint64)(0x10 + ((32 + 7) >> 3)));
            return true;
        case(1) :   return false;
        default: break;
    }
    return false; 
}
void IO_machine_arch::callTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 address, sc_dt::uint64 size, sc_dt::uint64 data, bool has_been_error, sc_dt::uint64 currentTime) {
    switch(portIndex) {
        case(0) : { //port: slave
            switch(transactionIndex) {
                case(0) : { //transaction: WRITE
                    this->transaction_instance__slave__WRITE
                      (
                        address, 
                        size, 
                        data, 
                        has_been_error, 
                        currentTime
                      );
                    break;
                }
                case(1) : { //transaction: READ
                    this->transaction_instance__slave__READ
                      (
                        address, 
                        size, 
                        data, 
                        has_been_error, 
                        currentTime
                      );
                    break;
                }
            }
            break;
        }
        case(1) : { //port: master
            switch(transactionIndex) {
                case(0) : { //transaction: WRITE
                    this->transaction_instance__master__WRITE
                      (
                        address, 
                        size, 
                        data, 
                        has_been_error, 
                        currentTime
                      );
                    break;
                }
                case(1) : { //transaction: READ
                    this->transaction_instance__master__READ
                      (
                        address, 
                        size, 
                        data, 
                        has_been_error, 
                        currentTime
                      );
                    break;
                }
            }
            break;
        }
        default: break;
    }
}


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
//* This file is write protected. 
//* DO NOT MODIFY THIS FILE.
//*
//* This file is generated according to the parameters in the 
//* Model Builder form.
//* 
//* Any change in parameters, policies, ports, protocols, etc. will change this file
//* upon generation of the timing model (using generate_timing_model command).
//* The IO_t will be derived from this class.
//*
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>




#include "IO_model.h"
#include <math.h>
#include <stdlib.h>

using namespace esl::tlm_sim;
using namespace esl::sc_sim;
using namespace std;
using namespace sc_dt;
using namespace sc_core;


void IO_t_base::schedule_axi_READ
(long port_index,
 sc_dt::uint64 delay,
 long& transactionSize,
 esl::include::transaction_power& power , long TR_ID, long ARADDR, long* RDATA, long ARSIZE, long block_size, long ARBURST, long ARLEN, long ARLOCK) {
  sc_dt::uint64 local_address = 0;
  local_address = ARADDR,
  scheduleGenericTransaction(port_index,
                             0,
                             local_address,
                             (unsigned char*)RDATA,
                             transactionSize,
                             block_size,
                             0, papoulis::getSimulationTime() + delay,
                             0,
                             0);
}

void IO_t_base::schedule_axi_WRITE
(long port_index,
 sc_dt::uint64 delay,
 long& transactionSize,
 esl::include::transaction_power& power , long TR_ID, long AWADDR, long* WDATA, long AWSIZE, long block_size, long AWBURST, long AWLEN, long AWLOCK) {
  sc_dt::uint64 local_address = 0;
  local_address = AWADDR,
  scheduleGenericTransaction(port_index,
                             1,
                             local_address,
                             (unsigned char*)WDATA,
                             transactionSize,
                             block_size,
                             0, papoulis::getSimulationTime() + delay,
                             0,
                             0);
}



void IO_t_base::start_transaction_axi_READ
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  
  long burstSize = request->getSize();
  parameters[1] = request->getAddress();
  parameters[2] = request->getFirstDataBlock();
  parameters[4] = request->getBlockSize();
  
  this->getMachineArch()->startTransaction(port_index,
                                           1,
                                           startTime);
  
  this->accept_axi_READ
    (port_index, 
     startTime, 
     burstSize, 
     parameters[0], 
     parameters[1], 
     parameters[2], 
     parameters[3], 
     parameters[4], 
     parameters[5], 
     parameters[6], 
     parameters[7]);
}

void IO_t_base::end_transaction_axi_READ
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  long burstSize = request->getSize();
  parameters[1] = request->getAddress();
  parameters[2] = request->getFirstDataBlock();
  parameters[4] = request->getBlockSize();                                                                         
  this->getMachineArch()->callTransaction(port_index, 
                                          1, 
                                          request->getAddress(),
                                          burstSize,
                                          request->getFirstDataBlock(),
                                          hasBeenError,
                                          startTime);
  
  this->end_axi_READ
    (port_index,
     endTime,
     burstSize,
     parameters[0],
     parameters[1],
     parameters[2],
     parameters[3],
     parameters[4],
     parameters[5],
     parameters[6],
     parameters[7]);
}
void IO_t_base::start_transaction_axi_WRITE
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  
  long burstSize = request->getSize();
  parameters[1] = request->getAddress();
  parameters[2] = request->getFirstDataBlock();
  parameters[4] = request->getBlockSize();
  
  this->getMachineArch()->startTransaction(port_index,
                                           0,
                                           startTime);
  
  this->accept_axi_WRITE
    (port_index, 
     startTime, 
     burstSize, 
     parameters[0], 
     parameters[1], 
     parameters[2], 
     parameters[3], 
     parameters[4], 
     parameters[5], 
     parameters[6], 
     parameters[7]);
}

void IO_t_base::end_transaction_axi_WRITE
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  long burstSize = request->getSize();
  parameters[1] = request->getAddress();
  parameters[2] = request->getFirstDataBlock();
  parameters[4] = request->getBlockSize();                                                                         
  this->getMachineArch()->callTransaction(port_index, 
                                          0, 
                                          request->getAddress(),
                                          burstSize,
                                          request->getFirstDataBlock(),
                                          hasBeenError,
                                          startTime);
  
  this->end_axi_WRITE
    (port_index,
     endTime,
     burstSize,
     parameters[0],
     parameters[1],
     parameters[2],
     parameters[3],
     parameters[4],
     parameters[5],
     parameters[6],
     parameters[7]);
}

IO_t_base::TransactionCallbackFunctionPointer IO_t_base::getTransactionCallback(unsigned callbackIndex) {
  static TransactionCallbackFunctionPointer funcArray[] = {
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::start_transaction_axi_READ),
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::start_transaction_axi_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::start_transaction_axi_READ),
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::start_transaction_axi_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::end_transaction_axi_READ),
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::end_transaction_axi_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::end_transaction_axi_READ),
    static_cast<TransactionCallbackFunctionPointer>(&IO_t_base::end_transaction_axi_WRITE),
    0
  };
  if(callbackIndex >= sizeof(funcArray) / sizeof(*funcArray) - 1)
    return 0;
  return funcArray[callbackIndex];
}

config::real80 IO_t_base::get_state_power() { 
  config::real80 power_value = 0;

  

  return power_value;
}

IO_t_base::IO_t_base(sc_module_name& module_name, long simulation) :
  papoulis::SystemCBaseModel(module_name),
  m_machineArch(new IO_machine_arch()),
  SD_INITIALIZE_PARAMETER(clock, sc_core::sc_time(10, sc_core::SC_NS)),
  SD_INITIALIZE_PARAMETER(axi_clock, clock),
  SD_INITIALIZE_PARAMETER(nominal_voltage, 1),
  SD_INITIALIZE_PARAMETER(mb_debug, false),
  SD_INITIALIZE_PARAMETER(call_to_default_if, false),
  SD_INITIALIZE_PARAMETER(verbose_parameters, true),
  SD_INITIALIZE_PARAMETER(dmi_enabled, true),
  SD_INITIALIZE_PARAMETER(warning_level, "WARNING"),
  SD_INITIALIZE_PARAMETER(slave_pipeline_length, 2),
  SD_INITIALIZE_PARAMETER(slave_outstanding, 1),
  SD_INITIALIZE_PARAMETER(slave_read_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER(slave_write_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER(master_outstanding, 1),
  SD_INITIALIZE_PARAMETER(master_read_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER(master_write_data_queue_size, 0),
  SD_INITIALIZE_PARAMETER(bmpfilename, "image_data/fractal_800x600.bmp"),
  SD_INITIALIZE_PARAMETER(jpgfilename, "generated_fractal.jpg"),
  m_simulation(simulation),
  inputaddr("inputaddr", this),
  inputlength("inputlength", this),
  outputaddr("outputaddr", this),
  upload("upload", this),
  download("download", this)
{
  bool separate_read_channel = false;
  bool separate_write_channel = false;
  fix_clock_parameter(axi_clock, "axi_clock");
  separate_read_channel = false;
  separate_write_channel = false;
  port_descriptions[slave].port_name = "slave";
  separate_read_channel = true;
  port_descriptions[slave].port_kind = PortDescription::READ_CHANNEL_PORT;
  separate_write_channel = true;
  port_descriptions[slave].port_kind = PortDescription::WRITE_CHANNEL_PORT;
  if (separate_write_channel && separate_read_channel)
    port_descriptions[slave].port_kind = PortDescription::SEPARATE_READ_WRIE_CHANNEL_PORT;
  port_descriptions[slave].is_read_write_address_channel = 0;
  
  port_descriptions[slave].is_master_port = 0;
  port_descriptions[slave].is_signal_port = 0;
  port_descriptions[slave].port_width = ((32 + 7) >> 3);
  port_descriptions[slave].port_width_in_bits = 32;
  port_descriptions[slave].protocol_name = "axi";
  port_descriptions[slave].ipxactBusDef.vendor = "amba.com";
  port_descriptions[slave].ipxactBusDef.library = "AMBA3";
  port_descriptions[slave].ipxactBusDef.name = "AXI";
  port_descriptions[slave].ipxactBusDef.version = "r1p0_5"; 
  port_descriptions[slave].ipxactAbstrDef.vendor = "Mentor.com";
  port_descriptions[slave].ipxactAbstrDef.library = "Lib_ipxact";
  port_descriptions[slave].ipxactAbstrDef.name = "AXI_tlm20";
  port_descriptions[slave].ipxactAbstrDef.version = "1.0";
  port_descriptions[slave].port_must_bind = 1;
  port_descriptions[slave].params_count = 8;
  if (slave_outstanding == 0) {
    slave_outstanding = 1;
    printf("WARNING: in \"%s\" the \"slave_outstanding\" parameter is 0, a default value of 1 is used\n", name());
  }
  port_descriptions[slave].pending = slave_outstanding;
  port_descriptions[slave].read_data_queue_size = slave_read_data_queue_size;
  port_descriptions[slave].write_data_queue_size = slave_write_data_queue_size;
  port_descriptions[slave].clock = axi_clock;
  m_machineArch->configureClock(slave, mb::sysc::sc_time_to_ps(axi_clock));
  port_descriptions[slave].add_read_phase(tlm::tlm_phase(tlm::BEGIN_REQ), !0, 0);
  port_descriptions[slave].add_read_phase(tlm::tlm_phase(tlm::END_REQ), !1, 0);
  port_descriptions[slave].add_read_phase(tlm::tlm_phase(tlm::BEGIN_RESP), !1, 1);
  port_descriptions[slave].add_read_phase(tlm::tlm_phase(tlm::END_RESP), !0, 1);
  port_descriptions[slave].add_write_phase(tlm::tlm_phase(tlm::BEGIN_REQ), !0, 0);
  port_descriptions[slave].add_write_phase(tlm::tlm_phase(tlm::END_REQ), !1, 0);
  port_descriptions[slave].add_write_phase(tlm::tlm_phase(tlm::BEGIN_RESP), !1, 1);
  port_descriptions[slave].add_write_phase(tlm::tlm_phase(tlm::END_RESP), !0, 1);

  fix_clock_parameter(axi_clock, "axi_clock");
  separate_read_channel = false;
  separate_write_channel = false;
  port_descriptions[master].port_name = "master";
  separate_read_channel = true;
  port_descriptions[master].port_kind = PortDescription::READ_CHANNEL_PORT;
  separate_write_channel = true;
  port_descriptions[master].port_kind = PortDescription::WRITE_CHANNEL_PORT;
  if (separate_write_channel && separate_read_channel)
    port_descriptions[master].port_kind = PortDescription::SEPARATE_READ_WRIE_CHANNEL_PORT;
  port_descriptions[master].is_read_write_address_channel = 0;
  
  port_descriptions[master].is_master_port = 1;
  port_descriptions[master].is_signal_port = 0;
  port_descriptions[master].port_width = ((32 + 7) >> 3);
  port_descriptions[master].port_width_in_bits = 32;
  port_descriptions[master].protocol_name = "axi";
  port_descriptions[master].ipxactBusDef.vendor = "amba.com";
  port_descriptions[master].ipxactBusDef.library = "AMBA3";
  port_descriptions[master].ipxactBusDef.name = "AXI";
  port_descriptions[master].ipxactBusDef.version = "r1p0_5"; 
  port_descriptions[master].ipxactAbstrDef.vendor = "Mentor.com";
  port_descriptions[master].ipxactAbstrDef.library = "Lib_ipxact";
  port_descriptions[master].ipxactAbstrDef.name = "AXI_tlm20";
  port_descriptions[master].ipxactAbstrDef.version = "1.0";
  port_descriptions[master].port_must_bind = 1;
  port_descriptions[master].params_count = 8;
  if (master_outstanding == 0) {
    master_outstanding = 1;
    printf("WARNING: in \"%s\" the \"master_outstanding\" parameter is 0, a default value of 1 is used\n", name());
  }
  port_descriptions[master].pending = master_outstanding;
  port_descriptions[master].read_data_queue_size = master_read_data_queue_size;
  port_descriptions[master].write_data_queue_size = master_write_data_queue_size;
  port_descriptions[master].clock = axi_clock;
  m_machineArch->configureClock(master, mb::sysc::sc_time_to_ps(axi_clock));
  port_descriptions[master].add_read_phase(tlm::tlm_phase(tlm::BEGIN_REQ), !1, 0);
  port_descriptions[master].add_read_phase(tlm::tlm_phase(tlm::END_REQ), !0, 0);
  port_descriptions[master].add_read_phase(tlm::tlm_phase(tlm::BEGIN_RESP), !0, 1);
  port_descriptions[master].add_read_phase(tlm::tlm_phase(tlm::END_RESP), !1, 1);
  port_descriptions[master].add_write_phase(tlm::tlm_phase(tlm::BEGIN_REQ), !1, 0);
  port_descriptions[master].add_write_phase(tlm::tlm_phase(tlm::END_REQ), !0, 0);
  port_descriptions[master].add_write_phase(tlm::tlm_phase(tlm::BEGIN_RESP), !0, 1);
  port_descriptions[master].add_write_phase(tlm::tlm_phase(tlm::END_RESP), !1, 1);


  port_descriptions[slave].default_read_transaction_name = "READ";
  port_descriptions[master].default_read_transaction_name = "READ";
  port_descriptions[slave].default_write_transaction_name = "WRITE";
  port_descriptions[master].default_write_transaction_name = "WRITE";

  fix_internal_clock_parameter(clock, "clock");
  set_ports(mb::sysc::sc_time_to_ps(clock), port_count, port_descriptions);

  bool isCpu = false;
  
  esl::tlm_sim::UserRunningModel* userRunningModel = Papoulis_CreateUserRunningModel(name(),
                                                                                     0,
                                                                                     this,
                                                                                     simulation,
                                                                                     false,
                                                                                     isCpu);
  setUserRunningModel(userRunningModel, isCpu);
  set_nominal_voltage(nominal_voltage);
  registerParameter("clock", "sc_core::sc_time", sdGetParameterAsConstString("clock"));
  registerParameter("axi_clock", "sc_core::sc_time", sdGetParameterAsConstString("axi_clock"));
  registerParameter("nominal_voltage", "double", sdGetParameterAsConstString("nominal_voltage"));
  registerParameter("mb_debug", "bool", sdGetParameterAsConstString("mb_debug"));
  registerParameter("call_to_default_if", "bool", sdGetParameterAsConstString("call_to_default_if"));
  registerParameter("verbose_parameters", "bool", sdGetParameterAsConstString("verbose_parameters"));
  registerParameter("dmi_enabled", "bool", sdGetParameterAsConstString("dmi_enabled"));
  registerParameter("warning_level", "const char*", sdGetParameterAsConstString("warning_level"));
  registerParameter("slave_pipeline_length", "unsigned int", sdGetParameterAsConstString("slave_pipeline_length"));
  registerParameter("slave_outstanding", "unsigned int", sdGetParameterAsConstString("slave_outstanding"));
  registerParameter("slave_read_data_queue_size", "unsigned int", sdGetParameterAsConstString("slave_read_data_queue_size"));
  registerParameter("slave_write_data_queue_size", "unsigned int", sdGetParameterAsConstString("slave_write_data_queue_size"));
  registerParameter("master_outstanding", "unsigned int", sdGetParameterAsConstString("master_outstanding"));
  registerParameter("master_read_data_queue_size", "unsigned int", sdGetParameterAsConstString("master_read_data_queue_size"));
  registerParameter("master_write_data_queue_size", "unsigned int", sdGetParameterAsConstString("master_write_data_queue_size"));
  registerParameter("bmpfilename", "const char*", sdGetParameterAsConstString("bmpfilename"));
  registerParameter("jpgfilename", "const char*", sdGetParameterAsConstString("jpgfilename"));
  set_sync_all(0); 
  set_sync_all(0);
  
  
  
  

  
  

  delay_policy* delay_policy_handle = 0;

  delay_policy_handle = new delay_policy(userRunningModel,
                                         "slave",
                                         0,
                                         0,
                                         mb::tlm20::SYNC_BEGIN);
  delay_policy_handle->set_power(5);
  m_delay_vector.push_back(delay_policy_handle);

  

  constant_global_power_policy* p = new constant_global_power_policy(userRunningModel);
  
  
  set_nominal_clock(1000000/100); /* in ps */ 
  update_coefficients();
  set_global_power_policy(p);
}

void IO_t_base::update_sync_all() {
  set_sync_all(0);
  set_sync_all(0);
}

void IO_t_base::update_pipeline_parameters(esl::sc_sim::pipeline_policy* handle) {
  if (m_pipeline_vector.size() == 0)
    return;


}

void IO_t_base::update_sequential_parameters(esl::sc_sim::sequential_policy* handle) {
  if (m_sequential_vector.size() == 0)
    return;

}

void IO_t_base::update_delay_parameters(esl::sc_sim::delay_policy* handle) {
  if (m_delay_vector.size() == 0)
    return;

  if (handle == m_delay_vector[0]) {
    handle->set_inner_delay(0);
    handle->set_start_delay(0);
    handle->setSyncMode(mb::tlm20::SYNC_BEGIN);
    handle->set_power(5);
    return;
  }
}

void IO_t_base::update_bus_parameters(esl::sc_sim::uniform_bus_policy* handle) {
  if (m_bus_vector.size() == 0)
    return;

}

void IO_t_base::update_power_parameters(esl::sc_sim::power_policy* handle) {
  if (m_power_vector.size() == 0)
    return;

}

void IO_t_base::update_state_power_parameters(esl::sc_sim::state_power_policy* handle) {
  if (m_state_power_vector.size() == 0)
    return;

}

void IO_t_base::update_global_power_parameters(esl::sc_sim::constant_global_power_policy* handle) {
  
  
  return;  
}

bool IO_t_base::triggerRegistersGotHit(unsigned portIndex, tlm::tlm_generic_payload& trans) {
  mb::utl::Segment<uint64_t> transactionRange(trans.get_address(), trans.get_address() + trans.get_data_length());

  {
    if (0 == portIndex) {
      mb::utl::Segment<uint64_t> current((1 + ((0xC % 4) << 3)), (1 + ((0xC % 4) << 3)));
      mb::utl::Segment<uint64_t> intersect = transactionRange.intersect(current);
      if (!intersect.empty())
        return true;
    }
  }
  {
    if (0 == portIndex) {
      mb::utl::Segment<uint64_t> current((1 + ((0x10 % 4) << 3)), (1 + ((0x10 % 4) << 3)));
      mb::utl::Segment<uint64_t> intersect = transactionRange.intersect(current);
      if (!intersect.empty())
        return true;
    }
  }
  return false;
}

bool IO_t_base::portHasRegisters(unsigned portIndex) {
  switch (portIndex) {

    {
    case 0:
      return true;
    }
  }
  return false;
}


unsigned IO_t_base::getPortCountForRegister(unsigned regIndex) {
  if (!strcmp(get_register_name(regIndex), "inputaddr"))
    return 1;
  if (!strcmp(get_register_name(regIndex), "inputlength"))
    return 1;
  if (!strcmp(get_register_name(regIndex), "outputaddr"))
    return 1;
  if (!strcmp(get_register_name(regIndex), "upload"))
    return 1;
  if (!strcmp(get_register_name(regIndex), "download"))
    return 1;
  return 0;
}

void IO_t_base::getPortNamesForRegister(unsigned regIndex, const char **names) {
  long i = 0;
  if (!strcmp(get_register_name(regIndex), "inputaddr")) {
    names[i++] = "slave";
    return;
  }
  if (!strcmp(get_register_name(regIndex), "inputlength")) {
    names[i++] = "slave";
    return;
  }
  if (!strcmp(get_register_name(regIndex), "outputaddr")) {
    names[i++] = "slave";
    return;
  }
  if (!strcmp(get_register_name(regIndex), "upload")) {
    names[i++] = "slave";
    return;
  }
  if (!strcmp(get_register_name(regIndex), "download")) {
    names[i++] = "slave";
    return;
  }
}


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
//* This file is write protected.
//* DO NOT MODIFY THIS FILE.
//*
//* This file contains the PVT class for IO.
//* It connects between the PV and T models.
//* Your top-level design should instantiate this model.
//* 
//* In order to synchronize the activty between the PV and the T models, every 
//* PV transaction is monitored and queued in the T sync ports.
//* Whenever a synchronization point is reached, the T models are executed and 
//* the corresponding T transactions are launched.
//* A synchronization point is reached whenever there is a wait statement on a testbench thread. 
//*
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>

#include "IO_model.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;


#include "IO_pv.h"




// Constructor
IO_pvt::IO_pvt(sc_module_name module_name)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
    IO_pvt_param_defaults(this->name()),
    master("master"),
    slave("slave"), 
    master_entry("unvisible_master_entry"),
    slave_entry("unvisible_slave_entry"),
    m_master_nb_connector(*this),
    m_slave_nb_connector(*this)

{


  m_IO_t = IO_t_base::create_t("T", 1);
  m_IO_pv = new IO_pv("PV");
  bind();
}  


IO_pvt::IO_pvt(sc_module_name module_name, sc_core::sc_time clock_init, sc_core::sc_time axi_clock_init, double nominal_voltage_init, bool mb_debug_init, bool call_to_default_if_init, bool verbose_parameters_init, bool dmi_enabled_init, const char* warning_level_init, unsigned int slave_pipeline_length_init, unsigned int slave_outstanding_init, unsigned int slave_read_data_queue_size_init, unsigned int slave_write_data_queue_size_init, unsigned int master_outstanding_init, unsigned int master_read_data_queue_size_init, unsigned int master_write_data_queue_size_init, const char* bmpfilename_init, const char* jpgfilename_init)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
  IO_pvt_param_defaults(this->name(), clock_init, axi_clock_init, nominal_voltage_init, mb_debug_init, call_to_default_if_init, verbose_parameters_init, dmi_enabled_init, warning_level_init, slave_pipeline_length_init, slave_outstanding_init, slave_read_data_queue_size_init, slave_write_data_queue_size_init, master_outstanding_init, master_read_data_queue_size_init, master_write_data_queue_size_init, bmpfilename_init, jpgfilename_init),
    master("master"),
    slave("slave"), 
    master_entry("unvisible_master_entry"),
    slave_entry("unvisible_slave_entry"),
    m_master_nb_connector(*this),
    m_slave_nb_connector(*this)
{


  m_IO_t = IO_t_base::create_t("T", 1);
  m_IO_pv = new IO_pv("PV");
  bind();
} 

IO_pvt::IO_pvt(sc_module_name module_name,  std::vector<std::pair<char*, unsigned int> > &change_parameters)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
  IO_pvt_param_defaults(this->name(), change_parameters),
    master("master"),
    slave("slave"), 
    master_entry("unvisible_master_entry"),
    slave_entry("unvisible_slave_entry"),
    m_master_nb_connector(*this),
    m_slave_nb_connector(*this)
{


  m_IO_t = IO_t_base::create_t("T", 1);
  m_IO_pv = new IO_pv("PV");
  bind();
} 

IO_pvt::~IO_pvt() {
  remove_pending_scenes();
  delete m_IO_t; delete m_IO_pv;
}




void IO_pvt::bind()
{
  /* connect pv to entry ports */
  
  
  getPV()->master.bind(master_entry);
  slave_entry.bind(getPV()->slave);   

  /* set callbacks on external and entry ports */ 

  slave.b_transport_cb.set(&slave_entry, &slave_entry_type::b_transport);
  slave.transport_dbg_cb.set(&slave_entry, &slave_entry_type::transport_dbg);
  slave.get_direct_mem_ptr_cb.set(&slave_entry, &slave_entry_type::get_direct_mem_ptr);

  master_entry.b_transport_cb.set(this, &self_type::b_transport_outside_master);
  master_entry.transport_dbg_cb.set(this, &self_type::transport_dbg_outside_master);
  master_entry.get_direct_mem_ptr_cb.set(&master, &master_type::get_direct_mem_ptr);

  
  /* connect t non blocking callbacks */

  master.nb_transport_bw_cb.set(this, &self_type::receive_nb_transport_master);
  getT()->set_port_cb(1, &m_master_nb_connector);

  slave.nb_transport_fw_cb.set(this, &self_type::receive_nb_transport_slave);
  getT()->set_port_cb(0, &m_slave_nb_connector);

  slave_entry.invalidate_direct_mem_ptr_cb.set(&slave, &slave_type::invalidate_direct_mem_ptr);

  master.invalidate_direct_mem_ptr_cb.set(&master_entry, &master_entry_type::invalidate_direct_mem_ptr);

  /* check mb_event policies */
  m_IO_t->check_mb_events();
}



void IO_pvt::b_transport_outside_master(tlm::tlm_generic_payload& p, sc_core::sc_time& t)
{
  
  master.b_transport(p, t);
  
} 

unsigned int IO_pvt::transport_dbg_outside_master(tlm::tlm_generic_payload& p)
{
  
  return master.transport_dbg(p);
  
}
