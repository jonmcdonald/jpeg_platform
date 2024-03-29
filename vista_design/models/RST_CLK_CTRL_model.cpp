
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
//* The RST_CLK_CTRL_pv will be derived from this class.
//*
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>


#include "RST_CLK_CTRL_model.h"

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


RST_CLK_CTRL_pv_base::RST_CLK_CTRL_pv_base(sc_module_name& module_name) : 
  RST_CLK_CTRL_pv_base_mb_compatibility(module_name),
  RST_CLK_CTRL_pv_base_parameters(this),
  VISTA_MB_PV_INIT_MEMBER(slave0),
  VISTA_MB_PV_INIT_MEMBER(nUART0_RST),
  VISTA_MB_PV_INIT_MEMBER(UART0_VIS_EN),
  VISTA_MB_PV_INIT_MEMBER(slave0_memory),
  uart_vis0_ctrl_reg("uart_vis0_ctrl_reg", &slave0_memory, ((0x0) / 4), ( (0) + (((0x0) % 4) << 3)), ( (0) + (((0x0) % 4) << 3)),  (32), mb::tlm20::WRITE_ACCESS) {


  slave0_memory.setPortIndex("slave0");
  payload_on_stack = 0;
  payload_for_token = 0;

  uart_vis0_ctrl_reg.setResetValue(0);
  VISTA_MB_PV_SET_SELF_TRANSPORT_DBG(uart_vis0_ctrl_reg);
  VISTA_MB_PV_SET_SELF_WRITE_CB(uart_vis0_ctrl_reg);
  VISTA_MB_PV_SET_TARGET_GET_DMI_DELAY_CB(slave0);
  VISTA_MB_PV_BIND_FW_PROCESS_TO_MEMORY_DEFAULT_IF(slave0);
  VISTA_MB_PV_REGISTER_SELF_FW_PROCESS_CALLBACKS(slave0);

}

void RST_CLK_CTRL_pv_base::reset_model() {
  // reset all registers


  uart_vis0_ctrl_reg.setResetValue(0);
  
  // reset value for output signals

  // reset value for input signals

}

RST_CLK_CTRL_pv_base_parameters::RST_CLK_CTRL_pv_base_parameters(sc_object* object) :
  m_object(object),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, clock, sc_core::sc_time(1000, sc_core::SC_PS)),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, signal_clock, clock),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, apb_clock, clock),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, nominal_voltage, 1),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, mb_debug, false),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, call_to_default_if, false),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, verbose_parameters, true),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, dmi_enabled, true),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, warning_level, "WARNING"),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, slave0_pipeline_length, 2)
{
  if(verbose_parameters) print_parameters();
}

void RST_CLK_CTRL_pv_base_parameters::print_parameters()
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
  std::cout << "\tsignal_clock = " << signal_clock << "\n";
  std::cout << "\tapb_clock = " << apb_clock << "\n";
  std::cout << "\tnominal_voltage = " << nominal_voltage << "\n";
  std::cout << "\tmb_debug = " << mb_debug << "\n";
  std::cout << "\tcall_to_default_if = " << call_to_default_if << "\n";
  std::cout << "\tverbose_parameters = " << verbose_parameters << "\n";
  std::cout << "\tdmi_enabled = " << dmi_enabled << "\n";
  std::cout << "\twarning_level = " << warning_level << "\n";
  std::cout << "\tslave0_pipeline_length = " << slave0_pipeline_length << "\n";
  std::cout.unsetf(ios::showbase);
  std::cout.setf(ios::dec, ios::basefield);
  std::cout << std::endl;
}


void RST_CLK_CTRL_pv_base::end_of_elaboration() {
  BASE_TYPE::end_of_elaboration();
  slave0_memory.bind_bw_direct_mem_if(slave0[0]);
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
//* It contains the machine architecture class for your RST_CLK_CTRL model.
//* 
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>

#include "RST_CLK_CTRL_model.h"
#include "model_builder.h"

using namespace ::mb::utl::minmax;

RST_CLK_CTRL_machine_arch::RST_CLK_CTRL_machine_arch()
    : MachineArch(3, 1), 
    uart_vis0_ctrl_reg(0x0) {
    sc_dt::uint64* values = m_values;
    uart_vis0_ctrl_reg.setValuePtr(values);
    
    reset();
}

////////////////////////////////////////////////////////////////////
//   function for port: slave0
////////////////////////////////////////////////////////////////////


//////
// functions for port: slave0, transaction: READ
//////

void RST_CLK_CTRL_machine_arch::transaction_instance__slave0__READ
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
// functions for port: slave0, transaction: WRITE
//////

void RST_CLK_CTRL_machine_arch::transaction_instance__slave0__WRITE
  (
    sc_dt::uint64 _address,
    sc_dt::uint64 _size,
    sc_dt::uint64 _data, 
    bool has_been_error, 
    sc_dt::uint64 currentTime
  )
{
  if ( _address == uart_vis0_ctrl_reg.getAddress()) { 
      uart_vis0_ctrl_reg = _data &(0x1);
  }
}

////////////////////////////////////////////////////////////////////
//   function for port: nUART0_RST
////////////////////////////////////////////////////////////////////


//////
// functions for port: nUART0_RST, transaction: WRITE
//////

void RST_CLK_CTRL_machine_arch::transaction_instance__nUART0_RST__WRITE
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
//   function for port: UART0_VIS_EN
////////////////////////////////////////////////////////////////////


//////
// functions for port: UART0_VIS_EN, transaction: WRITE
//////

void RST_CLK_CTRL_machine_arch::transaction_instance__UART0_VIS_EN__WRITE
  (
    sc_dt::uint64 _address,
    sc_dt::uint64 _size,
    sc_dt::uint64 _data, 
    bool has_been_error, 
    sc_dt::uint64 currentTime
  )
{
  
}

void RST_CLK_CTRL_machine_arch::startTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 currentTime) {
  switch(portIndex) {
    case(0) : break;
    case(1) : break;
    case(2) : break;
    default: break;
  }
}
VariableBase* RST_CLK_CTRL_machine_arch::getVariableBasePtrByName(const char* name) {
  return NULL;
}
Register* RST_CLK_CTRL_machine_arch::getRegisterByName(const char* name) {
  if (!strcmp(name, "uart_vis0_ctrl_reg")) {
    return &uart_vis0_ctrl_reg;
  }
  return 0;
}
void RST_CLK_CTRL_machine_arch::reset() { 
    MachineArch::reset();
}
bool RST_CLK_CTRL_machine_arch::getAddressRange(unsigned portIndex, sc_dt::uint64& begin, sc_dt::uint64& end) {
    switch(portIndex) {
        case(0) : begin = min(1, (sc_dt::uint64)(0x0));
            end = max(1, (sc_dt::uint64)(0x0 + ((32 + 7) >> 3)));
            return true;
        case(1) :   return false;
        case(2) :   return false;
        default: break;
    }
    return false; 
}
void RST_CLK_CTRL_machine_arch::callTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 address, sc_dt::uint64 size, sc_dt::uint64 data, bool has_been_error, sc_dt::uint64 currentTime) {
    switch(portIndex) {
        case(0) : { //port: slave0
            switch(transactionIndex) {
                case(0) : { //transaction: READ
                    this->transaction_instance__slave0__READ
                      (
                        address, 
                        size, 
                        data, 
                        has_been_error, 
                        currentTime
                      );
                    break;
                }
                case(1) : { //transaction: WRITE
                    this->transaction_instance__slave0__WRITE
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
        case(1) : { //port: nUART0_RST
            switch(transactionIndex) {
                case(0) : { //transaction: WRITE
                    this->transaction_instance__nUART0_RST__WRITE
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
        case(2) : { //port: UART0_VIS_EN
            switch(transactionIndex) {
                case(0) : { //transaction: WRITE
                    this->transaction_instance__UART0_VIS_EN__WRITE
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
//* The RST_CLK_CTRL_t will be derived from this class.
//*
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>




#include "RST_CLK_CTRL_model.h"
#include <math.h>
#include <stdlib.h>

using namespace esl::tlm_sim;
using namespace esl::sc_sim;
using namespace std;
using namespace sc_dt;
using namespace sc_core;


void RST_CLK_CTRL_t_base::schedule_signal_WRITE
(long port_index,
 sc_dt::uint64 delay,
 long& transactionSize,
 esl::include::transaction_power& power , long address, long* value_p, long block_size) {
  sc_dt::uint64 local_address = 0;
  local_address = address,
  scheduleGenericTransaction(port_index,
                             1,
                             local_address,
                             (unsigned char*)value_p,
                             transactionSize,
                             block_size,
                             0, papoulis::getSimulationTime() + delay,
                             0,
                             0);
}



void RST_CLK_CTRL_t_base::start_transaction_apb_READ
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  
  long burstSize = request->getSize();
  parameters[0] = request->getAddress();
  parameters[1] = request->getFirstDataBlock();
  parameters[2] = request->getBlockSize();
  
  this->getMachineArch()->startTransaction(port_index,
                                           0,
                                           startTime);
  
  this->accept_apb_READ
    (port_index, 
     startTime, 
     burstSize, 
     parameters[0], 
     parameters[1], 
     parameters[2]);
}

void RST_CLK_CTRL_t_base::end_transaction_apb_READ
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  long burstSize = request->getSize();
  parameters[0] = request->getAddress();
  parameters[1] = request->getFirstDataBlock();
  parameters[2] = request->getBlockSize();                                                                         
  this->getMachineArch()->callTransaction(port_index, 
                                          0, 
                                          request->getAddress(),
                                          burstSize,
                                          request->getFirstDataBlock(),
                                          hasBeenError,
                                          startTime);
  
  this->end_apb_READ
    (port_index,
     endTime,
     burstSize,
     parameters[0],
     parameters[1],
     parameters[2]);
}
void RST_CLK_CTRL_t_base::start_transaction_apb_WRITE
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  
  long burstSize = request->getSize();
  parameters[0] = request->getAddress();
  parameters[1] = request->getFirstDataBlock();
  parameters[2] = request->getBlockSize();
  
  this->getMachineArch()->startTransaction(port_index,
                                           1,
                                           startTime);
  
  this->accept_apb_WRITE
    (port_index, 
     startTime, 
     burstSize, 
     parameters[0], 
     parameters[1], 
     parameters[2]);
}

void RST_CLK_CTRL_t_base::end_transaction_apb_WRITE
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  long burstSize = request->getSize();
  parameters[0] = request->getAddress();
  parameters[1] = request->getFirstDataBlock();
  parameters[2] = request->getBlockSize();                                                                         
  this->getMachineArch()->callTransaction(port_index, 
                                          1, 
                                          request->getAddress(),
                                          burstSize,
                                          request->getFirstDataBlock(),
                                          hasBeenError,
                                          startTime);
  
  this->end_apb_WRITE
    (port_index,
     endTime,
     burstSize,
     parameters[0],
     parameters[1],
     parameters[2]);
}
void RST_CLK_CTRL_t_base::start_transaction_signal_WRITE
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  
  long burstSize = request->getSize();
  parameters[0] = request->getAddress();
  parameters[1] = request->getFirstDataBlock();
  parameters[2] = request->getBlockSize();
  
  this->getMachineArch()->startTransaction(port_index,
                                           0,
                                           startTime);
  
  this->accept_signal_WRITE
    (port_index, 
     startTime, 
     burstSize, 
     parameters[0], 
     parameters[1], 
     parameters[2]);
}

void RST_CLK_CTRL_t_base::end_transaction_signal_WRITE
(esl::include::TRequest* request,
 unsigned port_index,
 sc_dt::uint64 startTime,
 sc_dt::uint64 endTime,
 sc_dt::uint64* parameters,
 bool hasBeenError) {
  long burstSize = request->getSize();
  parameters[0] = request->getAddress();
  parameters[1] = request->getFirstDataBlock();
  parameters[2] = request->getBlockSize();                                                                         
  this->getMachineArch()->callTransaction(port_index, 
                                          0, 
                                          request->getAddress(),
                                          burstSize,
                                          request->getFirstDataBlock(),
                                          hasBeenError,
                                          startTime);
  
  this->end_signal_WRITE
    (port_index,
     endTime,
     burstSize,
     parameters[0],
     parameters[1],
     parameters[2]);
}

RST_CLK_CTRL_t_base::TransactionCallbackFunctionPointer RST_CLK_CTRL_t_base::getTransactionCallback(unsigned callbackIndex) {
  static TransactionCallbackFunctionPointer funcArray[] = {
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::start_transaction_apb_READ),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::start_transaction_apb_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::null_transaction_callback),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::start_transaction_signal_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::null_transaction_callback),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::start_transaction_signal_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::end_transaction_apb_READ),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::end_transaction_apb_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::null_transaction_callback),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::end_transaction_signal_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::null_transaction_callback),
    static_cast<TransactionCallbackFunctionPointer>(&RST_CLK_CTRL_t_base::end_transaction_signal_WRITE),
    0
  };
  if(callbackIndex >= sizeof(funcArray) / sizeof(*funcArray) - 1)
    return 0;
  return funcArray[callbackIndex];
}

config::real80 RST_CLK_CTRL_t_base::get_state_power() { 
  config::real80 power_value = 0;

  

  return power_value;
}

RST_CLK_CTRL_t_base::RST_CLK_CTRL_t_base(sc_module_name& module_name, long simulation) :
  papoulis::SystemCBaseModel(module_name),
  m_machineArch(new RST_CLK_CTRL_machine_arch()),
  SD_INITIALIZE_PARAMETER(clock, sc_core::sc_time(1000, sc_core::SC_PS)),
  SD_INITIALIZE_PARAMETER(signal_clock, clock),
  SD_INITIALIZE_PARAMETER(apb_clock, clock),
  SD_INITIALIZE_PARAMETER(nominal_voltage, 1),
  SD_INITIALIZE_PARAMETER(mb_debug, false),
  SD_INITIALIZE_PARAMETER(call_to_default_if, false),
  SD_INITIALIZE_PARAMETER(verbose_parameters, true),
  SD_INITIALIZE_PARAMETER(dmi_enabled, true),
  SD_INITIALIZE_PARAMETER(warning_level, "WARNING"),
  SD_INITIALIZE_PARAMETER(slave0_pipeline_length, 2),
  m_simulation(simulation),
  uart_vis0_ctrl_reg("uart_vis0_ctrl_reg", this)
{
  bool separate_read_channel = false;
  bool separate_write_channel = false;
  fix_clock_parameter(apb_clock, "apb_clock");
  separate_read_channel = false;
  separate_write_channel = false;
  port_descriptions[slave0].port_name = "slave0";
  port_descriptions[slave0].port_kind = PortDescription::READ_WRITE_CHANNEL_PORT;
  if (separate_write_channel && separate_read_channel)
    port_descriptions[slave0].port_kind = PortDescription::SEPARATE_READ_WRIE_CHANNEL_PORT;
  port_descriptions[slave0].is_read_write_address_channel = 0;
  
  port_descriptions[slave0].is_read_write_address_channel = 1;
  port_descriptions[slave0].is_master_port = 0;
  port_descriptions[slave0].is_signal_port = 0;
  port_descriptions[slave0].port_width = ((32 + 7) >> 3);
  port_descriptions[slave0].port_width_in_bits = 32;
  port_descriptions[slave0].protocol_name = "apb";
  port_descriptions[slave0].ipxactBusDef.vendor = "amba.com";
  port_descriptions[slave0].ipxactBusDef.library = "AMBA2";
  port_descriptions[slave0].ipxactBusDef.name = "APB";
  port_descriptions[slave0].ipxactBusDef.version = "r2p0_4"; 
  port_descriptions[slave0].ipxactAbstrDef.vendor = "Mentor.com";
  port_descriptions[slave0].ipxactAbstrDef.library = "Lib_ipxact";
  port_descriptions[slave0].ipxactAbstrDef.name = "APB_tlm20";
  port_descriptions[slave0].ipxactAbstrDef.version = "1.0";
  port_descriptions[slave0].port_must_bind = 1;
  port_descriptions[slave0].params_count = 3;
  port_descriptions[slave0].clock = apb_clock;
  m_machineArch->configureClock(slave0, mb::sysc::sc_time_to_ps(apb_clock));
  port_descriptions[slave0].add_read_phase(tlm::tlm_phase(tlm::BEGIN_REQ), !0, 1);
  port_descriptions[slave0].add_read_phase(tlm::tlm_phase(tlm::BEGIN_RESP), !1, 1);

  fix_clock_parameter(signal_clock, "signal_clock");
  separate_read_channel = false;
  separate_write_channel = false;
  port_descriptions[nUART0_RST].port_name = "nUART0_RST";
  separate_write_channel = true;
  port_descriptions[nUART0_RST].port_kind = PortDescription::WRITE_CHANNEL_PORT;
  if (separate_write_channel && separate_read_channel)
    port_descriptions[nUART0_RST].port_kind = PortDescription::SEPARATE_READ_WRIE_CHANNEL_PORT;
  port_descriptions[nUART0_RST].is_read_write_address_channel = 0;
  
  port_descriptions[nUART0_RST].is_master_port = 1;
  port_descriptions[nUART0_RST].is_signal_port = 1;
  port_descriptions[nUART0_RST].port_width = ((1 + 7) >> 3);
  port_descriptions[nUART0_RST].port_width_in_bits = 1;
  port_descriptions[nUART0_RST].protocol_name = "signal";
  port_descriptions[nUART0_RST].ipxactBusDef.vendor = "Mentor.com";
  port_descriptions[nUART0_RST].ipxactBusDef.library = "Lib_ipxact";
  port_descriptions[nUART0_RST].ipxactBusDef.name = "signal";
  port_descriptions[nUART0_RST].ipxactBusDef.version = "1.0"; 
  port_descriptions[nUART0_RST].ipxactAbstrDef.vendor = "Mentor.com";
  port_descriptions[nUART0_RST].ipxactAbstrDef.library = "Lib_ipxact";
  port_descriptions[nUART0_RST].ipxactAbstrDef.name = "signal_tlm20";
  port_descriptions[nUART0_RST].ipxactAbstrDef.version = "1.0";
  port_descriptions[nUART0_RST].port_must_bind = 0;
  port_descriptions[nUART0_RST].params_count = 3;
  port_descriptions[nUART0_RST].clock = signal_clock;
  m_machineArch->configureClock(nUART0_RST, mb::sysc::sc_time_to_ps(signal_clock));
  port_descriptions[nUART0_RST].add_write_phase(tlm::tlm_phase(tlm::BEGIN_REQ), !1, 1);
  port_descriptions[nUART0_RST].add_write_phase(tlm::tlm_phase(tlm::BEGIN_RESP), !0, 1);

  fix_clock_parameter(signal_clock, "signal_clock");
  separate_read_channel = false;
  separate_write_channel = false;
  port_descriptions[UART0_VIS_EN].port_name = "UART0_VIS_EN";
  separate_write_channel = true;
  port_descriptions[UART0_VIS_EN].port_kind = PortDescription::WRITE_CHANNEL_PORT;
  if (separate_write_channel && separate_read_channel)
    port_descriptions[UART0_VIS_EN].port_kind = PortDescription::SEPARATE_READ_WRIE_CHANNEL_PORT;
  port_descriptions[UART0_VIS_EN].is_read_write_address_channel = 0;
  
  port_descriptions[UART0_VIS_EN].is_master_port = 1;
  port_descriptions[UART0_VIS_EN].is_signal_port = 1;
  port_descriptions[UART0_VIS_EN].port_width = ((1 + 7) >> 3);
  port_descriptions[UART0_VIS_EN].port_width_in_bits = 1;
  port_descriptions[UART0_VIS_EN].protocol_name = "signal";
  port_descriptions[UART0_VIS_EN].ipxactBusDef.vendor = "Mentor.com";
  port_descriptions[UART0_VIS_EN].ipxactBusDef.library = "Lib_ipxact";
  port_descriptions[UART0_VIS_EN].ipxactBusDef.name = "signal";
  port_descriptions[UART0_VIS_EN].ipxactBusDef.version = "1.0"; 
  port_descriptions[UART0_VIS_EN].ipxactAbstrDef.vendor = "Mentor.com";
  port_descriptions[UART0_VIS_EN].ipxactAbstrDef.library = "Lib_ipxact";
  port_descriptions[UART0_VIS_EN].ipxactAbstrDef.name = "signal_tlm20";
  port_descriptions[UART0_VIS_EN].ipxactAbstrDef.version = "1.0";
  port_descriptions[UART0_VIS_EN].port_must_bind = 1;
  port_descriptions[UART0_VIS_EN].params_count = 3;
  port_descriptions[UART0_VIS_EN].clock = signal_clock;
  m_machineArch->configureClock(UART0_VIS_EN, mb::sysc::sc_time_to_ps(signal_clock));
  port_descriptions[UART0_VIS_EN].add_write_phase(tlm::tlm_phase(tlm::BEGIN_REQ), !1, 1);
  port_descriptions[UART0_VIS_EN].add_write_phase(tlm::tlm_phase(tlm::BEGIN_RESP), !0, 1);


  port_descriptions[slave0].default_read_transaction_name = "READ";
  port_descriptions[slave0].default_write_transaction_name = "WRITE";
  port_descriptions[nUART0_RST].default_write_transaction_name = "WRITE";
  port_descriptions[UART0_VIS_EN].default_write_transaction_name = "WRITE";

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
  registerParameter("signal_clock", "sc_core::sc_time", sdGetParameterAsConstString("signal_clock"));
  registerParameter("apb_clock", "sc_core::sc_time", sdGetParameterAsConstString("apb_clock"));
  registerParameter("nominal_voltage", "double", sdGetParameterAsConstString("nominal_voltage"));
  registerParameter("mb_debug", "bool", sdGetParameterAsConstString("mb_debug"));
  registerParameter("call_to_default_if", "bool", sdGetParameterAsConstString("call_to_default_if"));
  registerParameter("verbose_parameters", "bool", sdGetParameterAsConstString("verbose_parameters"));
  registerParameter("dmi_enabled", "bool", sdGetParameterAsConstString("dmi_enabled"));
  registerParameter("warning_level", "const char*", sdGetParameterAsConstString("warning_level"));
  registerParameter("slave0_pipeline_length", "unsigned int", sdGetParameterAsConstString("slave0_pipeline_length"));
  set_sync_all(0); 
  set_sync_all(0);
  
  
  
  unsigned sequential_policy_index = 0;
  sequential_policy* sequential_policy_handle = 0;
  user_sequential_policy_descriptions[sequential_policy_index].firstTransaction = "slave0";
  user_sequential_policy_descriptions[sequential_policy_index].secondTransaction = "UART0_VIS_EN";
  user_sequential_policy_descriptions[sequential_policy_index].setLatency(1);
  user_sequential_policy_descriptions[sequential_policy_index].syncMode = mb::tlm20::NO_SYNC;
  sequential_policy_handle = new sequential_policy(userRunningModel, user_sequential_policy_descriptions[sequential_policy_index]);
  m_sequential_vector.push_back(sequential_policy_handle);
  sequential_policy_handle->set_power(5);
  sequential_policy_index++;
  user_sequential_policy_descriptions[sequential_policy_index].firstTransaction = "slave0";
  user_sequential_policy_descriptions[sequential_policy_index].secondTransaction = "nUART0_RST";
  user_sequential_policy_descriptions[sequential_policy_index].setLatency(1);
  user_sequential_policy_descriptions[sequential_policy_index].syncMode = mb::tlm20::NO_SYNC;
  sequential_policy_handle = new sequential_policy(userRunningModel, user_sequential_policy_descriptions[sequential_policy_index]);
  m_sequential_vector.push_back(sequential_policy_handle);
  sequential_policy_handle->set_power(5);
  sequential_policy_index++;
  set_user_sequential_policies(user_sequential_policy_count, user_sequential_policy_descriptions);
  
  

  
  

  


  

  constant_global_power_policy* p = new constant_global_power_policy(userRunningModel);
  p->set_leakage_power(0.01);
  p->set_clock_tree_power(0.1);
  set_nominal_clock(1000000/100); /* in ps */ 
  update_coefficients();
  set_global_power_policy(p);
}

void RST_CLK_CTRL_t_base::update_sync_all() {
  set_sync_all(0);
  set_sync_all(0);
}

void RST_CLK_CTRL_t_base::update_pipeline_parameters(esl::sc_sim::pipeline_policy* handle) {
  if (m_pipeline_vector.size() == 0)
    return;


}

void RST_CLK_CTRL_t_base::update_sequential_parameters(esl::sc_sim::sequential_policy* handle) {
  if (m_sequential_vector.size() == 0)
    return;

  if (handle == m_sequential_vector[0]) {
    handle->setLatency(1);
    handle->setSyncMode(mb::tlm20::NO_SYNC);
    handle->set_power(5);
    return;
  }
  if (handle == m_sequential_vector[1]) {
    handle->setLatency(1);
    handle->setSyncMode(mb::tlm20::NO_SYNC);
    handle->set_power(5);
    return;
  }
}

void RST_CLK_CTRL_t_base::update_delay_parameters(esl::sc_sim::delay_policy* handle) {
  if (m_delay_vector.size() == 0)
    return;

}

void RST_CLK_CTRL_t_base::update_bus_parameters(esl::sc_sim::uniform_bus_policy* handle) {
  if (m_bus_vector.size() == 0)
    return;

}

void RST_CLK_CTRL_t_base::update_power_parameters(esl::sc_sim::power_policy* handle) {
  if (m_power_vector.size() == 0)
    return;

}

void RST_CLK_CTRL_t_base::update_state_power_parameters(esl::sc_sim::state_power_policy* handle) {
  if (m_state_power_vector.size() == 0)
    return;

}

void RST_CLK_CTRL_t_base::update_global_power_parameters(esl::sc_sim::constant_global_power_policy* handle) {
  handle->set_leakage_power(0.01);
  handle->set_clock_tree_power(0.1);
  return;  
}

bool RST_CLK_CTRL_t_base::triggerRegistersGotHit(unsigned portIndex, tlm::tlm_generic_payload& trans) {
  mb::utl::Segment<uint64_t> transactionRange(trans.get_address(), trans.get_address() + trans.get_data_length());

  {
    if (0 == portIndex) {
      mb::utl::Segment<uint64_t> current((1 + ((0x0 % 4) << 3)), (1 + ((0x0 % 4) << 3)));
      mb::utl::Segment<uint64_t> intersect = transactionRange.intersect(current);
      if (!intersect.empty())
        return true;
    }
  }
  return false;
}

bool RST_CLK_CTRL_t_base::portHasRegisters(unsigned portIndex) {
  switch (portIndex) {

    {
    case 0:
      return true;
    }
  }
  return false;
}


unsigned RST_CLK_CTRL_t_base::getPortCountForRegister(unsigned regIndex) {
  if (!strcmp(get_register_name(regIndex), "uart_vis0_ctrl_reg"))
    return 1;
  return 0;
}

void RST_CLK_CTRL_t_base::getPortNamesForRegister(unsigned regIndex, const char **names) {
  long i = 0;
  if (!strcmp(get_register_name(regIndex), "uart_vis0_ctrl_reg")) {
    names[i++] = "slave0";
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
//* This file contains the PVT class for RST_CLK_CTRL.
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

#include "RST_CLK_CTRL_model.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;


#include "RST_CLK_CTRL_pv.h"




// Constructor
RST_CLK_CTRL_pvt::RST_CLK_CTRL_pvt(sc_module_name module_name)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
    RST_CLK_CTRL_pvt_param_defaults(this->name()),
    slave0("slave0"),
    nUART0_RST("nUART0_RST"),
    UART0_VIS_EN("UART0_VIS_EN"),
    slave0_entry("unvisible_slave0_entry"),
    m_nUART0_RST_nb_connector(*this),
    m_UART0_VIS_EN_nb_connector(*this),
    m_slave0_nb_connector(*this)

{


  m_RST_CLK_CTRL_t = RST_CLK_CTRL_t_base::create_t("T", 1);
  m_RST_CLK_CTRL_pv = new RST_CLK_CTRL_pv("PV");
  bind();
}  


RST_CLK_CTRL_pvt::RST_CLK_CTRL_pvt(sc_module_name module_name, sc_core::sc_time clock_init, sc_core::sc_time signal_clock_init, sc_core::sc_time apb_clock_init, double nominal_voltage_init, bool mb_debug_init, bool call_to_default_if_init, bool verbose_parameters_init, bool dmi_enabled_init, const char* warning_level_init, unsigned int slave0_pipeline_length_init)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
  RST_CLK_CTRL_pvt_param_defaults(this->name(), clock_init, signal_clock_init, apb_clock_init, nominal_voltage_init, mb_debug_init, call_to_default_if_init, verbose_parameters_init, dmi_enabled_init, warning_level_init, slave0_pipeline_length_init),
    slave0("slave0"),
    nUART0_RST("nUART0_RST"),
    UART0_VIS_EN("UART0_VIS_EN"),
    slave0_entry("unvisible_slave0_entry"),
    m_nUART0_RST_nb_connector(*this),
    m_UART0_VIS_EN_nb_connector(*this),
    m_slave0_nb_connector(*this)
{


  m_RST_CLK_CTRL_t = RST_CLK_CTRL_t_base::create_t("T", 1);
  m_RST_CLK_CTRL_pv = new RST_CLK_CTRL_pv("PV");
  bind();
} 

RST_CLK_CTRL_pvt::RST_CLK_CTRL_pvt(sc_module_name module_name,  std::vector<std::pair<char*, unsigned int> > &change_parameters)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
  RST_CLK_CTRL_pvt_param_defaults(this->name(), change_parameters),
    slave0("slave0"),
    nUART0_RST("nUART0_RST"),
    UART0_VIS_EN("UART0_VIS_EN"),
    slave0_entry("unvisible_slave0_entry"),
    m_nUART0_RST_nb_connector(*this),
    m_UART0_VIS_EN_nb_connector(*this),
    m_slave0_nb_connector(*this)
{


  m_RST_CLK_CTRL_t = RST_CLK_CTRL_t_base::create_t("T", 1);
  m_RST_CLK_CTRL_pv = new RST_CLK_CTRL_pv("PV");
  bind();
} 

RST_CLK_CTRL_pvt::~RST_CLK_CTRL_pvt() {
  remove_pending_scenes();
  delete m_RST_CLK_CTRL_t; delete m_RST_CLK_CTRL_pv;
}




void RST_CLK_CTRL_pvt::bind()
{
  /* connect pv to entry ports */
  
  
  slave0_entry.bind(getPV()->slave0);
  getPV()->nUART0_RST.bind(nUART0_RST_entry);
  getPV()->UART0_VIS_EN.bind(UART0_VIS_EN_entry);   

  /* set callbacks on external and entry ports */ 

  slave0.b_transport_cb.set(&slave0_entry, &slave0_entry_type::b_transport);
  slave0.transport_dbg_cb.set(&slave0_entry, &slave0_entry_type::transport_dbg);
  slave0.get_direct_mem_ptr_cb.set(&slave0_entry, &slave0_entry_type::get_direct_mem_ptr);

  nUART0_RST_entry.write_cb.set(this, &self_type::b_transport_outside_nUART0_RST);

  UART0_VIS_EN_entry.write_cb.set(this, &self_type::b_transport_outside_UART0_VIS_EN);

  
  /* connect t non blocking callbacks */

  nUART0_RST.nb_transport_bw_cb.set(this, &self_type::receive_nb_transport_nUART0_RST);
  getT()->set_port_cb(1, &m_nUART0_RST_nb_connector);

  UART0_VIS_EN.nb_transport_bw_cb.set(this, &self_type::receive_nb_transport_UART0_VIS_EN);
  getT()->set_port_cb(2, &m_UART0_VIS_EN_nb_connector);

  slave0.nb_transport_fw_cb.set(this, &self_type::receive_nb_transport_slave0);
  getT()->set_port_cb(0, &m_slave0_nb_connector);

  slave0_entry.invalidate_direct_mem_ptr_cb.set(&slave0, &slave0_type::invalidate_direct_mem_ptr);

  /* check mb_event policies */
  m_RST_CLK_CTRL_t->check_mb_events();
}



void RST_CLK_CTRL_pvt::b_transport_outside_nUART0_RST(bool data)
{
  static tlm::tlm_generic_payload payload;

  payload.reset();
  payload.set_command(tlm::TLM_WRITE_COMMAND);
  payload.set_data_ptr((unsigned char*)&data);
  payload.set_data_length(sizeof(bool));
  payload.set_streaming_width(sizeof(bool));

  mb::tlm20::clear_pvt_ext(payload);
  mb::tlm20::setup_pvt_ext(payload)->set_signal(true);

  if (!nUART0_RST.get_interface()) {
    return;
  }

  mb::mb_module* current_model = dynamic_cast<mb::mb_module*>(m_RST_CLK_CTRL_pv);
  if (current_model)
    mb::tlm20::setup_pvt_ext(payload)->setToken(current_model->get_current_token());
  sc_time t;
  nUART0_RST.b_transport(payload, t);
}

void RST_CLK_CTRL_pvt::b_transport_outside_UART0_VIS_EN(bool data)
{
  static tlm::tlm_generic_payload payload;

  payload.reset();
  payload.set_command(tlm::TLM_WRITE_COMMAND);
  payload.set_data_ptr((unsigned char*)&data);
  payload.set_data_length(sizeof(bool));
  payload.set_streaming_width(sizeof(bool));

  mb::tlm20::clear_pvt_ext(payload);
  mb::tlm20::setup_pvt_ext(payload)->set_signal(true);

  

  mb::mb_module* current_model = dynamic_cast<mb::mb_module*>(m_RST_CLK_CTRL_pv);
  if (current_model)
    mb::tlm20::setup_pvt_ext(payload)->setToken(current_model->get_current_token());
  sc_time t;
  UART0_VIS_EN.b_transport(payload, t);
}

