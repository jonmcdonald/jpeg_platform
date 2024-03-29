
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
//* The sram_pv will be derived from this class.
//*
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>


#include "sram_model.h"

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


sram_pv_base::sram_pv_base(sc_module_name& module_name) : 
  sram_pv_base_mb_compatibility(module_name),
  sram_pv_base_parameters(this),
  VISTA_MB_PV_INIT_MEMBER(slave) {


  payload_on_stack = 0;
  payload_for_token = 0;

  VISTA_MB_PV_BIND_FW_PROCESS_TO_TARGET(slave);
  VISTA_MB_PV_REGISTER_SELF_FW_PROCESS_CALLBACKS(slave);

}

void sram_pv_base::reset_model() {
  // reset all registers


  
  // reset value for output signals

  // reset value for input signals

}

sram_pv_base_parameters::sram_pv_base_parameters(sc_object* object) :
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
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, size_in_bytes, 0x10000000),
  SD_INITIALIZE_PARAMETER_EXTERNALLY(object, endianness, "target")
{
  if(verbose_parameters) print_parameters();
}

void sram_pv_base_parameters::print_parameters()
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
  std::cout << "\tsize_in_bytes = " << size_in_bytes << "\n";
  std::cout << "\tendianness = " << endianness << "\n";
  std::cout.unsetf(ios::showbase);
  std::cout.setf(ios::dec, ios::basefield);
  std::cout << std::endl;
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


//constructor
sram_base1_pv::sram_base1_pv(sc_module_name module_name)
  : sram_pv_base(module_name),
  m_memory_if(this, size_in_bytes, mb::sysc::get_instance_endianness(), 64, this)
{
  if (::mb::sysc::sdParameterExists("read_latency")) {
    m_memory_if.set_read_latency(::mb::sysc::sdGetParameterAsTime("read_latency"));
  }

  if (::mb::sysc::sdParameterExists("write_latency")) {
    m_memory_if.set_write_latency(::mb::sysc::sdGetParameterAsTime("write_latency"));
  }

}

//virtual 
void sram_base1_pv::slave_callback(tlm::tlm_base_protocol_types::tlm_payload_type& trans, sc_core::sc_time& t) {
  sc_core::sc_time start_t = t;
  m_memory_if.b_transport(trans, t);
  if (t > start_t) {
    // when policy dynamic dont use read/write latency
    if (getPVTBaseModel()->getCurrentScene()) {
      t = start_t;
    }
  }
}

//virtual 
unsigned sram_base1_pv::slave_callback_dbg(tlm::tlm_base_protocol_types::tlm_payload_type& trans) {
  return m_memory_if.transport_dbg(trans);
}

//virtual 
bool sram_base1_pv::slave_get_direct_memory_ptr_callback(tlm::tlm_base_protocol_types::tlm_payload_type& trans,
                                                                           tlm::tlm_dmi& dmiData) {
  bool status = m_memory_if.get_direct_mem_ptr(trans, dmiData);
  bool latency_from_parameters = 
    dmiData.get_read_latency() > sc_core::SC_ZERO_TIME ||
    dmiData.get_write_latency() > sc_core::SC_ZERO_TIME;
  if (status && !latency_from_parameters)
    get_dmi_delay_cb(trans, dmiData, slave_idx);
  return status;
}

//virtual
void sram_base1_pv::do_invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {

  slave->invalidate_direct_mem_ptr(start_range, end_range);

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
//* It contains the machine architecture class for your sram model.
//* 
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>

#include "sram_model.h"
#include "model_builder.h"

using namespace ::mb::utl::minmax;

sram_machine_arch::sram_machine_arch()
    : MachineArch(1, 0) {
        reset();
    }
    
    ////////////////////////////////////////////////////////////////////
    //   function for port: slave
    ////////////////////////////////////////////////////////////////////
    
    
    //////
    // functions for port: slave, transaction: WRITE
    //////
    
    void sram_machine_arch::transaction_instance__slave__WRITE
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
    // functions for port: slave, transaction: READ
    //////
    
    void sram_machine_arch::transaction_instance__slave__READ
      (
        sc_dt::uint64 _address,
        sc_dt::uint64 _size,
        sc_dt::uint64 _data, 
        bool has_been_error, 
        sc_dt::uint64 currentTime
      )
    {
      
    }

void sram_machine_arch::startTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 currentTime) {
  switch(portIndex) {
    case(0) : break;
    default: break;
  }
}
VariableBase* sram_machine_arch::getVariableBasePtrByName(const char* name) {
  return NULL;
}
Register* sram_machine_arch::getRegisterByName(const char* name) {
  return 0;
}
void sram_machine_arch::reset() { 
    MachineArch::reset();
}
bool sram_machine_arch::getAddressRange(unsigned portIndex, sc_dt::uint64& begin, sc_dt::uint64& end) {
    switch(portIndex) {
        case(0) :   return false;
        default: break;
    }
    return false; 
}
void sram_machine_arch::callTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 address, sc_dt::uint64 size, sc_dt::uint64 data, bool has_been_error, sc_dt::uint64 currentTime) {
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
//* The sram_t will be derived from this class.
//*
//* Model Builder version: 4.1.0
//* Generated on: Jan. 18, 2016 04:09:44 PM, (user: mbradley)
//*>




#include "sram_model.h"
#include <math.h>
#include <stdlib.h>

using namespace esl::tlm_sim;
using namespace esl::sc_sim;
using namespace std;
using namespace sc_dt;
using namespace sc_core;




void sram_t_base::start_transaction_axi_READ
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

void sram_t_base::end_transaction_axi_READ
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
void sram_t_base::start_transaction_axi_WRITE
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

void sram_t_base::end_transaction_axi_WRITE
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

sram_t_base::TransactionCallbackFunctionPointer sram_t_base::getTransactionCallback(unsigned callbackIndex) {
  static TransactionCallbackFunctionPointer funcArray[] = {
    static_cast<TransactionCallbackFunctionPointer>(&sram_t_base::start_transaction_axi_READ),
    static_cast<TransactionCallbackFunctionPointer>(&sram_t_base::start_transaction_axi_WRITE),
    static_cast<TransactionCallbackFunctionPointer>(&sram_t_base::end_transaction_axi_READ),
    static_cast<TransactionCallbackFunctionPointer>(&sram_t_base::end_transaction_axi_WRITE),
    0
  };
  if(callbackIndex >= sizeof(funcArray) / sizeof(*funcArray) - 1)
    return 0;
  return funcArray[callbackIndex];
}

config::real80 sram_t_base::get_state_power() { 
  config::real80 power_value = 0;

  

  return power_value;
}

sram_t_base::sram_t_base(sc_module_name& module_name, long simulation) :
  papoulis::SystemCBaseModel(module_name),
  m_machineArch(new sram_machine_arch()),
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
  SD_INITIALIZE_PARAMETER(size_in_bytes, 0x10000000),
  SD_INITIALIZE_PARAMETER(endianness, "target"),
  m_simulation(simulation)
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
  port_descriptions[slave].port_width = ((64 + 7) >> 3);
  port_descriptions[slave].port_width_in_bits = 64;
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


  port_descriptions[slave].default_read_transaction_name = "READ";
  port_descriptions[slave].default_write_transaction_name = "WRITE";

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
  registerParameter("size_in_bytes", "unsigned long long", sdGetParameterAsConstString("size_in_bytes"));
  registerParameter("endianness", "const char*", sdGetParameterAsConstString("endianness"));
  set_sync_all(0); 
  set_sync_all(0);
  
  
  
  

  
  

  delay_policy* delay_policy_handle = 0;

  delay_policy_handle = new delay_policy(userRunningModel,
                                         "slave.READ",
                                         3,
                                         0,
                                         mb::tlm20::NO_SYNC);
  delay_policy_handle->set_power(3);
  m_delay_vector.push_back(delay_policy_handle);
  delay_policy_handle = new delay_policy(userRunningModel,
                                         "slave.WRITE",
                                         5,
                                         0,
                                         mb::tlm20::NO_SYNC);
  delay_policy_handle->set_power(4);
  m_delay_vector.push_back(delay_policy_handle);

  

  constant_global_power_policy* p = new constant_global_power_policy(userRunningModel);
  p->set_leakage_power(0.05);
  p->set_clock_tree_power(0.1);
  set_nominal_clock(1000000/100); /* in ps */ 
  update_coefficients();
  set_global_power_policy(p);
}

void sram_t_base::update_sync_all() {
  set_sync_all(0);
  set_sync_all(0);
}

void sram_t_base::update_pipeline_parameters(esl::sc_sim::pipeline_policy* handle) {
  if (m_pipeline_vector.size() == 0)
    return;


}

void sram_t_base::update_sequential_parameters(esl::sc_sim::sequential_policy* handle) {
  if (m_sequential_vector.size() == 0)
    return;

}

void sram_t_base::update_delay_parameters(esl::sc_sim::delay_policy* handle) {
  if (m_delay_vector.size() == 0)
    return;

  if (handle == m_delay_vector[0]) {
    handle->set_inner_delay(3);
    handle->set_start_delay(0);
    handle->setSyncMode(mb::tlm20::NO_SYNC);
    handle->set_power(3);
    return;
  }
  if (handle == m_delay_vector[1]) {
    handle->set_inner_delay(5);
    handle->set_start_delay(0);
    handle->setSyncMode(mb::tlm20::NO_SYNC);
    handle->set_power(4);
    return;
  }
}

void sram_t_base::update_bus_parameters(esl::sc_sim::uniform_bus_policy* handle) {
  if (m_bus_vector.size() == 0)
    return;

}

void sram_t_base::update_power_parameters(esl::sc_sim::power_policy* handle) {
  if (m_power_vector.size() == 0)
    return;

}

void sram_t_base::update_state_power_parameters(esl::sc_sim::state_power_policy* handle) {
  if (m_state_power_vector.size() == 0)
    return;

}

void sram_t_base::update_global_power_parameters(esl::sc_sim::constant_global_power_policy* handle) {
  handle->set_leakage_power(0.05);
  handle->set_clock_tree_power(0.1);
  return;  
}

bool sram_t_base::triggerRegistersGotHit(unsigned portIndex, tlm::tlm_generic_payload& trans) {
  mb::utl::Segment<uint64_t> transactionRange(trans.get_address(), trans.get_address() + trans.get_data_length());

  return false;
}

bool sram_t_base::portHasRegisters(unsigned portIndex) {
  switch (portIndex) {

  }
  return false;
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
//* This file contains the PVT class for sram.
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

#include "sram_model.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;


#include "sram_pv.h"




// Constructor
sram_pvt::sram_pvt(sc_module_name module_name)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
    sram_pvt_param_defaults(this->name()),
    slave("slave"),
    slave_entry("unvisible_slave_entry"),
    m_slave_nb_connector(*this)

{


  m_sram_t = sram_t_base::create_t("T", 1);
  m_sram_pv = new sram_pv("PV");
  bind();
}  


sram_pvt::sram_pvt(sc_module_name module_name, sc_core::sc_time clock_init, sc_core::sc_time axi_clock_init, double nominal_voltage_init, bool mb_debug_init, bool call_to_default_if_init, bool verbose_parameters_init, bool dmi_enabled_init, const char* warning_level_init, unsigned int slave_pipeline_length_init, unsigned int slave_outstanding_init, unsigned int slave_read_data_queue_size_init, unsigned int slave_write_data_queue_size_init, unsigned long long size_in_bytes_init, const char* endianness_init)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
  sram_pvt_param_defaults(this->name(), clock_init, axi_clock_init, nominal_voltage_init, mb_debug_init, call_to_default_if_init, verbose_parameters_init, dmi_enabled_init, warning_level_init, slave_pipeline_length_init, slave_outstanding_init, slave_read_data_queue_size_init, slave_write_data_queue_size_init, size_in_bytes_init, endianness_init),
    slave("slave"),
    slave_entry("unvisible_slave_entry"),
    m_slave_nb_connector(*this)
{


  m_sram_t = sram_t_base::create_t("T", 1);
  m_sram_pv = new sram_pv("PV");
  bind();
} 

sram_pvt::sram_pvt(sc_module_name module_name,  std::vector<std::pair<char*, unsigned int> > &change_parameters)
  : esl::sc_sim::PVTBaseModel(module_name, 0),
  sram_pvt_param_defaults(this->name(), change_parameters),
    slave("slave"),
    slave_entry("unvisible_slave_entry"),
    m_slave_nb_connector(*this)
{


  m_sram_t = sram_t_base::create_t("T", 1);
  m_sram_pv = new sram_pv("PV");
  bind();
} 

sram_pvt::~sram_pvt() {
  remove_pending_scenes();
  delete m_sram_t; delete m_sram_pv;
}




void sram_pvt::bind()
{
  /* connect pv to entry ports */
  
  
  slave_entry.bind(getPV()->slave);   

  /* set callbacks on external and entry ports */ 

  slave.b_transport_cb.set(&slave_entry, &slave_entry_type::b_transport);
  slave.transport_dbg_cb.set(&slave_entry, &slave_entry_type::transport_dbg);
  slave.get_direct_mem_ptr_cb.set(&slave_entry, &slave_entry_type::get_direct_mem_ptr);

  
  /* connect t non blocking callbacks */

  slave.nb_transport_fw_cb.set(this, &self_type::receive_nb_transport_slave);
  getT()->set_port_cb(0, &m_slave_nb_connector);

  slave_entry.invalidate_direct_mem_ptr_cb.set(&slave, &slave_type::invalidate_direct_mem_ptr);

  /* check mb_event policies */
  m_sram_t->check_mb_events();
}



