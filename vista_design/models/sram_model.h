
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



#pragma once

#include "model_builder.h"

#ifdef MODEL_BUILDER_VERSION_NUMBER
#if 4100 != MODEL_BUILDER_VERSION_NUMBER
#error "This model was generated using Model Builder version: 4.1.0. Please regenerate model."
#endif
#else
#error "Please regenerate model."
#endif




#include "axi_protocol.h"

using namespace ::tlm;

class sram_pv_base;
class sram_pv_base_mb_compatibility : public mb::mb_module {
  friend class sram_pv_base;
 public:
  typedef unsigned int mb_address_type;
 protected:
  sram_pv_base_mb_compatibility(sc_core::sc_module_name& module_name)
    : mb::mb_module(module_name) {}

  ////////////////////////////////////////
  // functions for target port: slave
  ////////////////////////////////////////
  protected:
   virtual bool slave_callback_read(mb_address_type address,
                                                      unsigned char* data,
                                                      unsigned size) {
     return true;
   } 
   virtual bool slave_callback_write(mb_address_type address,
                                                       unsigned char* data,
                                                       unsigned size) {
     return true;
   } 
   virtual unsigned slave_callback_read_dbg(mb_address_type address,
                                                              unsigned char* data,
                                                              unsigned size) {
     return 0;
   } 
   virtual unsigned slave_callback_write_dbg(mb_address_type address,
                                                               unsigned char* data,
                                                               unsigned size) {
     return 0;
   }
   virtual bool slave_get_direct_memory_ptr(mb_address_type address,
                                                              tlm::tlm_dmi& dmiData) {
     return false;
   } 
};

class sram_pv_base_parameters {
 public :
  sram_pv_base_parameters(sc_object* object);
  void print_parameters();

 protected:
  sc_object* m_object;

  sc_core::sc_time clock;
  sc_core::sc_time axi_clock;
  double nominal_voltage;
  bool mb_debug;
  bool call_to_default_if;
  bool verbose_parameters;
  bool dmi_enabled;
  const char* warning_level;
  unsigned int slave_pipeline_length;
  unsigned int slave_outstanding;
  unsigned int slave_read_data_queue_size;
  unsigned int slave_write_data_queue_size;
  unsigned long long size_in_bytes;
  const char* endianness;
};

class sram_pv_base : public sram_pv_base_mb_compatibility,
                           public sram_pv_base_parameters
{
 private:
  typedef sram_pv_base_mb_compatibility mb_compatibility_class;
  typedef sram_pv_base_mb_compatibility BASE_TYPE;
 public:
  typedef esl::tlm_types::Address mb_address_type;
 public:
  typedef sram_pv_base self;
  SC_HAS_PROCESS(sram_pv_base);
  sram_pv_base(sc_core::sc_module_name& module_name);
  virtual void reset_model();
  enum port_enum {slave_idx };
  
 private:

  mb::tlm20::fw_process<tlm::tlm_base_protocol_types> m_slave_fw_process;

 protected:

  virtual void slave_callback(tlm::tlm_base_protocol_types::tlm_payload_type& trans, sc_core::sc_time& t) {
    bool status;

    if (mb_debug) {
      std::cout.unsetf(std::ios::showbase);
      bool isWrite = trans.is_write();
      unsigned char* data = trans.get_data_ptr();
      sc_dt::uint64 address = trans.get_address();
      unsigned size = trans.get_data_length();
      std::cout << this->name() << "::slave_callback_" <<
        (isWrite ? "write" : "read") << ", address = 0x" << std::hex << address;
      if (isWrite) {
        std::cout << ", data = {";
        std::cout << std::hex;
        for (unsigned u=0; u<size; u++) {
          std::cout << "0x" << (unsigned int)(data[u]);
          if (u+1 != size) {
            std::cout << ", ";
          }
        }
        std::cout << std::dec << "}";
      }
      std::cout << ", size = " << std::dec << size << std::endl;
    }
    if (trans.get_byte_enable_ptr() && trans.get_byte_enable_length()) {
      std::stringstream str;
      str << std::showbase << std::hex;
      if (trans.is_write())
        str << "writing to address " << trans.get_address() << " : byte enable pointer defined but not used ";
      else
        str << "reading from address " << trans.get_address() << " : byte enable pointer defined but not used ";
      mb::sysc::complain(str.str().c_str(), this->name());
    }
    payload_for_token = &trans;
    
    if (trans.is_write()) {
      status = slave_callback_write(trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else if (trans.is_read()) {
      status = slave_callback_read(trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
      status = false;
    }
    payload_for_token = 0;

    tlm::tlm_response_status trans_status = trans.get_response_status();
    if (trans_status == tlm::TLM_INCOMPLETE_RESPONSE ||
        trans_status == tlm::TLM_OK_RESPONSE)
      trans.set_response_status(status ? TLM_OK_RESPONSE : TLM_GENERIC_ERROR_RESPONSE);
  }

  virtual unsigned slave_callback_dbg(tlm::tlm_base_protocol_types::tlm_payload_type& trans) {
    
    unsigned status = 0;
    if (trans.is_write()) {
      status = slave_callback_write_dbg(trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else if (trans.is_read()) {
      status = slave_callback_read_dbg(trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } 

    return status;
  }

  virtual bool slave_get_direct_memory_ptr_callback(tlm::tlm_base_protocol_types::tlm_payload_type& trans,
                                                                      tlm::tlm_dmi& dmiData) {
    if (trans.get_command() == tlm::TLM_IGNORE_COMMAND)
      return false;
    payload_on_stack = &trans;
    bool ret_value = slave_get_direct_memory_ptr(trans.get_address(), dmiData);
    if (ret_value) {
      get_dmi_delay_cb(trans, dmiData, slave_idx);
    }
    payload_on_stack = 0;
    return ret_value;
  }

  virtual void get_dmi_delay_cb(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmiData, unsigned port_index) {
    sc_time t = sc_core::SC_ZERO_TIME;
    
    getPVTBaseModel()->get_write_delay_for_dmi(port_index, trans, t);
    dmiData.set_write_latency(t);
    t = sc_core::SC_ZERO_TIME;
    getPVTBaseModel()->get_read_delay_for_dmi(port_index, trans, t);
    dmiData.set_read_latency(t);
    
  }

  virtual void invalidate_direct_mem_ptr_callback(unsigned master_index, sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
  }

  

  



  

  
  

 public:




  tlm::tlm_generic_payload* payload_on_stack;
  tlm::tlm_generic_payload* payload_for_token;



  ////////////////////////////////////////
  // functions for target port: slave
  ////////////////////////////////////////
  protected:
  virtual bool slave_callback_read(mb_address_type address,
                                                      unsigned char* data,
                                                      unsigned size) {
    mb_address_type local_address = address;
     mb_compatibility_class *This = this;
     return This->slave_callback_read(mb_compatibility_class::mb_address_type(local_address),
                                                        data,
                                                        size);
   }
   virtual bool slave_callback_write(mb_address_type address,
                                                       unsigned char* data,
                                                       unsigned size) {
     mb_address_type local_address = address;
     mb_compatibility_class *This = this;
     return This->slave_callback_write(mb_compatibility_class::mb_address_type(local_address),
                                                         data,
                                                         size);
   }

   virtual unsigned slave_callback_read_dbg(mb_address_type address,
                                                              unsigned char* data,
                                                              unsigned size) {
     mb_compatibility_class *This = this;
     return This->slave_callback_read_dbg(mb_compatibility_class::mb_address_type(address),
                                                            data,
                                                            size);
   }
   virtual unsigned slave_callback_write_dbg(mb_address_type address,
                                                               unsigned char* data,
                                                               unsigned size) {
     mb_compatibility_class *This = this;
     return This->slave_callback_write_dbg(mb_compatibility_class::mb_address_type(address),
                                                             data,
                                                             size);
   }
   virtual bool slave_get_direct_memory_ptr(mb_address_type address, tlm::tlm_dmi& dmiData) {
     mb_compatibility_class *This = this;
     return This->slave_get_direct_memory_ptr(mb_compatibility_class::mb_address_type(address), dmiData);
   } 

   
  
  
 

 public:
  typedef tlm::tlm_base_protocol_types slave_protocol_types;
  typedef tlm::tlm_base_protocol_types::tlm_payload_type slave_payload_type;

 public:
  // port declarations 

  tlm::tlm_target_socket <64, tlm::tlm_base_protocol_types> slave;  

 
  
  

 public:
  mb::mb_token_ptr get_current_token() {
    mb::mb_token_ptr token_ptr = mb_module::get_current_token();
    if (token_ptr)
      return token_ptr;
    if (payload_for_token) {
      mb::tlm20::pvt_ext* extension = mb::tlm20::setup_pvt_ext(*payload_for_token);
      if (extension) {
        mb_module::set_current_token(extension->getToken());
        return extension->getToken();
      }
    }
    return 0;
  }
  

 protected:
};



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


#pragma once
#include "sram_model.h"

using namespace tlm;

//This class inherits from the sram_pv_base class
class sram_base1_pv : public sram_pv_base,
                               public mb::models::memory_if::Invalidator,
                               public virtual mb::tlm20::mmap::Memory
{

 public:
  // Constructor
  // Do not add parameters here.
  // To add parameters - use the Model Builder form (under PV->Parameters tab)
  SC_HAS_PROCESS(sram_base1_pv);
  sram_base1_pv(sc_module_name module_name); 

public: 
  virtual void slave_callback(tlm::tlm_base_protocol_types::tlm_payload_type& trans, sc_core::sc_time& t);
  virtual unsigned slave_callback_dbg(tlm::tlm_base_protocol_types::tlm_payload_type& trans);
  virtual bool slave_get_direct_memory_ptr_callback(tlm::tlm_base_protocol_types::tlm_payload_type& trans,
                                                        tlm::tlm_dmi& dmiData);

public:
  virtual void do_invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);

protected:

  virtual uint64_t mmap_get_size() const {
    return m_memory_if.get_data_size();
  }

  virtual mb::tlm20::Access mmap_get_access() const {
    return m_memory_if.get_access();
  }

 protected:
  mb::models::memory_if m_memory_if;
};

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


#pragma once

#include "model_builder.h"


class sram_machine_arch : public MachineArch { 
    
    public:
    
    public:
        
        ////////////////////////////////////////////////////////////////////
        //   function for port: slave
        ////////////////////////////////////////////////////////////////////
        
        //////
        // functions for port: slave, transaction: WRITE
        //////
        
        void transaction_instance__slave__WRITE
          (
            sc_dt::uint64 _address,
            sc_dt::uint64 _size,
            sc_dt::uint64 _data, 
            bool has_been_error, 
            sc_dt::uint64 currentTime
          );
        
        //////
        // functions for port: slave, transaction: READ
        //////
        
        void transaction_instance__slave__READ
          (
            sc_dt::uint64 _address,
            sc_dt::uint64 _size,
            sc_dt::uint64 _data, 
            bool has_been_error, 
            sc_dt::uint64 currentTime
          );
        
        
    
    public:
        void configureClock(unsigned portIndex, sc_dt::uint64 clock) {
          m_clocks[portIndex] = clock;
        }
        void startTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 currentTime);
        VariableBase* getVariableBasePtrByName(const char* name);
        Register* getRegisterByName(const char* name);
        virtual void reset();
        
        virtual bool getAddressRange(unsigned portIndex, sc_dt::uint64& begin, sc_dt::uint64& end);
        
    
    public:
        void callTransaction(unsigned portIndex, unsigned transactionIndex, sc_dt::uint64 address, sc_dt::uint64 size, sc_dt::uint64 data, bool has_been_error, sc_dt::uint64 currentTime);
    
    public:
        //constructor
        sram_machine_arch();
    
};

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


#pragma once

#include "model_builder.h"
#include "axi_protocol.h"
 


class sram_t_base : public papoulis::SystemCBaseModel {
public:
  static sram_t_base* create_t(const char* _name, long simulation);
public:
  sram_t_base(sc_core::sc_module_name& module_name, long simulation);
 
  enum port_enum {slave};

protected:
  // The following callbacks are called whenever there is a transaction start or end in the port
  // If you want to add your own code (like updating the machine architecture object), 
  // you have to overload these functions in your derived class.

  virtual void accept_axi_READ
    (long port_index,
     sc_dt::int64 time,
     long& burst_size,
     sc_dt::uint64& TR_ID,
     sc_dt::uint64 ARADDR,
     sc_dt::uint64 RDATA,
     sc_dt::uint64& ARSIZE,
     sc_dt::uint64 block_size,
     sc_dt::uint64& ARBURST,
     sc_dt::uint64& ARLEN,
     sc_dt::uint64& ARLOCK) {};
  virtual void end_axi_READ
    (long port_index,
     sc_dt::uint64 time,
     long& burst_size,
     sc_dt::uint64& TR_ID,
     sc_dt::uint64 ARADDR,
     sc_dt::uint64 RDATA,
     sc_dt::uint64& ARSIZE,
     sc_dt::uint64 block_size,
     sc_dt::uint64& ARBURST,
     sc_dt::uint64& ARLEN,
     sc_dt::uint64& ARLOCK) {};

  virtual void accept_axi_WRITE
    (long port_index,
     sc_dt::int64 time,
     long& burst_size,
     sc_dt::uint64& TR_ID,
     sc_dt::uint64 AWADDR,
     sc_dt::uint64 WDATA,
     sc_dt::uint64& AWSIZE,
     sc_dt::uint64 block_size,
     sc_dt::uint64& AWBURST,
     sc_dt::uint64& AWLEN,
     sc_dt::uint64& AWLOCK) {};
  virtual void end_axi_WRITE
    (long port_index,
     sc_dt::uint64 time,
     long& burst_size,
     sc_dt::uint64& TR_ID,
     sc_dt::uint64 AWADDR,
     sc_dt::uint64 WDATA,
     sc_dt::uint64& AWSIZE,
     sc_dt::uint64 block_size,
     sc_dt::uint64& AWBURST,
     sc_dt::uint64& AWLEN,
     sc_dt::uint64& AWLOCK) {};


public:
  virtual sram_machine_arch* getMachineArch() const {
    return m_machineArch;
  }

  virtual bool portHasRegisters(unsigned portIndex);
  
  virtual bool triggerRegistersGotHit(unsigned portIndex, tlm::tlm_generic_payload& trans);

  virtual const char* get_library_name() {return "Models";}
  virtual const char* get_model_vendor() {return "Mentor.com";}
  virtual const char* get_model_version() {return "1.0";}

protected:
  /* schedule functions */
public:
  /* called when "params.m_phase" is sent through the "port_index" port */
  virtual void sent_phase_cb(unsigned port_index,
                             esl::sc_sim::mb_cb_params& params) {
    
  }
                             
  /* called when "params.m_phase" is received by the "port_index" port */
  virtual void received_phase_cb(unsigned port_index,
                                 esl::sc_sim::mb_cb_params& params) {
    
  }

  
protected:
  /* machine architecture */
  sram_machine_arch* m_machineArch;

protected:
  friend class sram_model;
  
  

protected:
  // parameters
  sc_core::sc_time clock;
  sc_core::sc_time axi_clock;
  double nominal_voltage;
  bool mb_debug;
  bool call_to_default_if;
  bool verbose_parameters;
  bool dmi_enabled;
  const char* warning_level;
  unsigned int slave_pipeline_length;
  unsigned int slave_outstanding;
  unsigned int slave_read_data_queue_size;
  unsigned int slave_write_data_queue_size;
  unsigned long long size_in_bytes;
  const char* endianness;

protected:
  long m_simulation;

 
  
  
  

  static const unsigned port_count = 1;
  papoulis::SystemCBaseModel::PortDescription port_descriptions[port_count];
  
  

protected:
  std::vector<esl::sc_sim::pipeline_policy*> m_pipeline_vector;
  std::vector<esl::sc_sim::sequential_policy*> m_sequential_vector;
  std::vector<esl::sc_sim::delay_policy*> m_delay_vector;
  std::vector<esl::sc_sim::uniform_bus_policy*> m_bus_vector;
  std::vector<esl::sc_sim::power_policy*> m_power_vector;
  std::vector<esl::sc_sim::state_power_policy*> m_state_power_vector;
public:
  virtual void update_sync_all();
  virtual void update_pipeline_parameters(esl::sc_sim::pipeline_policy* handle);
  virtual void update_sequential_parameters(esl::sc_sim::sequential_policy* handle);
  virtual void update_delay_parameters(esl::sc_sim::delay_policy* handle);
  virtual void update_bus_parameters(esl::sc_sim::uniform_bus_policy* handle);
  virtual void update_power_parameters(esl::sc_sim::power_policy* handle);
  virtual void update_global_power_parameters(esl::sc_sim::constant_global_power_policy* handle);
  virtual void update_state_power_parameters(esl::sc_sim::state_power_policy* handle);
protected: /* InterfaceFunctions */
  virtual TransactionCallbackFunctionPointer getTransactionCallback(unsigned callbackIndex);
  config::real80 get_state_power();
protected:

  void start_transaction_axi_READ
    (esl::include::TRequest* request,
     unsigned port_index,
     sc_dt::uint64 startTime,
     sc_dt::uint64 endTime,
     sc_dt::uint64* parameters,
     bool hasBeenError);
  void end_transaction_axi_READ
    (esl::include::TRequest* request,
     unsigned port_index,
     sc_dt::uint64 startTime,
     sc_dt::uint64 endTime,
     sc_dt::uint64* parameters,
     bool hasBeenError);
  void start_transaction_axi_WRITE
    (esl::include::TRequest* request,
     unsigned port_index,
     sc_dt::uint64 startTime,
     sc_dt::uint64 endTime,
     sc_dt::uint64* parameters,
     bool hasBeenError);
  void end_transaction_axi_WRITE
    (esl::include::TRequest* request,
     unsigned port_index,
     sc_dt::uint64 startTime,
     sc_dt::uint64 endTime,
     sc_dt::uint64* parameters,
     bool hasBeenError);
};

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


#pragma once

#include "model_builder.h"



 


// forward declaration for pv class
class sram_pv;



using namespace std;

class sram_pvt_param_defaults {
  
public:
  sram_pvt_param_defaults(const char* hier_name) {

    sc_core::sc_time clock;
    sc_core::sc_time axi_clock;
    double nominal_voltage;
    bool mb_debug;
    bool call_to_default_if;
    bool verbose_parameters;
    bool dmi_enabled;
    const char* warning_level;
    unsigned int slave_pipeline_length;
    unsigned int slave_outstanding;
    unsigned int slave_read_data_queue_size;
    unsigned int slave_write_data_queue_size;
    unsigned long long size_in_bytes;
    const char* endianness;
    clock = ::mb::sysc::sdInitParameter<sc_core::sc_time>("clock", sc_core::sc_time(10, sc_core::SC_NS), hier_name);
    axi_clock = ::mb::sysc::sdInitParameter<sc_core::sc_time>("axi_clock", clock, hier_name);
    nominal_voltage = ::mb::sysc::sdInitParameter<double>("nominal_voltage", 1, hier_name);
    mb_debug = ::mb::sysc::sdInitParameter<bool>("mb_debug", false, hier_name);
    call_to_default_if = ::mb::sysc::sdInitParameter<bool>("call_to_default_if", false, hier_name);
    verbose_parameters = ::mb::sysc::sdInitParameter<bool>("verbose_parameters", true, hier_name);
    dmi_enabled = ::mb::sysc::sdInitParameter<bool>("dmi_enabled", true, hier_name);
    warning_level = ::mb::sysc::sdInitParameter<const char*>("warning_level", "WARNING", hier_name);
    slave_pipeline_length = ::mb::sysc::sdInitParameter<unsigned int>("slave_pipeline_length", 2, hier_name);
    slave_outstanding = ::mb::sysc::sdInitParameter<unsigned int>("slave_outstanding", 1, hier_name);
    slave_read_data_queue_size = ::mb::sysc::sdInitParameter<unsigned int>("slave_read_data_queue_size", 0, hier_name);
    slave_write_data_queue_size = ::mb::sysc::sdInitParameter<unsigned int>("slave_write_data_queue_size", 0, hier_name);
    size_in_bytes = ::mb::sysc::sdInitParameter<unsigned long long>("size_in_bytes", 0x10000000, hier_name);
    endianness = ::mb::sysc::sdInitParameter<const char*>("endianness", "target", hier_name);
    ::mb::sysc::sdPropagateParameters();
  }

  sram_pvt_param_defaults(const char* hier_name, sc_core::sc_time clock_init, sc_core::sc_time axi_clock_init, double nominal_voltage_init, bool mb_debug_init, bool call_to_default_if_init, bool verbose_parameters_init, bool dmi_enabled_init, const char* warning_level_init, unsigned int slave_pipeline_length_init, unsigned int slave_outstanding_init, unsigned int slave_read_data_queue_size_init, unsigned int slave_write_data_queue_size_init, unsigned long long size_in_bytes_init, const char* endianness_init) {

    sc_core::sc_time clock;
    sc_core::sc_time axi_clock;
    double nominal_voltage;
    bool mb_debug;
    bool call_to_default_if;
    bool verbose_parameters;
    bool dmi_enabled;
    const char* warning_level;
    unsigned int slave_pipeline_length;
    unsigned int slave_outstanding;
    unsigned int slave_read_data_queue_size;
    unsigned int slave_write_data_queue_size;
    unsigned long long size_in_bytes;
    const char* endianness;
    clock = ::mb::sysc::sdInitParameter<sc_core::sc_time>("clock", clock_init, hier_name);
    axi_clock = ::mb::sysc::sdInitParameter<sc_core::sc_time>("axi_clock", axi_clock_init, hier_name);
    nominal_voltage = ::mb::sysc::sdInitParameter<double>("nominal_voltage", nominal_voltage_init, hier_name);
    mb_debug = ::mb::sysc::sdInitParameter<bool>("mb_debug", mb_debug_init, hier_name);
    call_to_default_if = ::mb::sysc::sdInitParameter<bool>("call_to_default_if", call_to_default_if_init, hier_name);
    verbose_parameters = ::mb::sysc::sdInitParameter<bool>("verbose_parameters", verbose_parameters_init, hier_name);
    dmi_enabled = ::mb::sysc::sdInitParameter<bool>("dmi_enabled", dmi_enabled_init, hier_name);
    warning_level = ::mb::sysc::sdInitParameter<const char*>("warning_level", warning_level_init, hier_name);
    slave_pipeline_length = ::mb::sysc::sdInitParameter<unsigned int>("slave_pipeline_length", slave_pipeline_length_init, hier_name);
    slave_outstanding = ::mb::sysc::sdInitParameter<unsigned int>("slave_outstanding", slave_outstanding_init, hier_name);
    slave_read_data_queue_size = ::mb::sysc::sdInitParameter<unsigned int>("slave_read_data_queue_size", slave_read_data_queue_size_init, hier_name);
    slave_write_data_queue_size = ::mb::sysc::sdInitParameter<unsigned int>("slave_write_data_queue_size", slave_write_data_queue_size_init, hier_name);
    size_in_bytes = ::mb::sysc::sdInitParameter<unsigned long long>("size_in_bytes", size_in_bytes_init, hier_name);
    endianness = ::mb::sysc::sdInitParameter<const char*>("endianness", endianness_init, hier_name);

    ::mb::sysc::sdPropagateParameters();
    }

  sram_pvt_param_defaults(const char* hier_name, std::vector<std::pair<char*, unsigned int> > &change_parameters){

    sc_core::sc_time clock;
    sc_core::sc_time axi_clock;
    double nominal_voltage;
    bool mb_debug;
    bool call_to_default_if;
    bool verbose_parameters;
    bool dmi_enabled;
    const char* warning_level;
    unsigned int slave_pipeline_length;
    unsigned int slave_outstanding;
    unsigned int slave_read_data_queue_size;
    unsigned int slave_write_data_queue_size;
    unsigned long long size_in_bytes;
    const char* endianness;
    for (unsigned i =0; i<change_parameters.size(); i++){
      ::mb::sysc::sdInitParameter<unsigned int>(change_parameters[i].first, change_parameters[i].second, hier_name);
    }

    clock = ::mb::sysc::sdInitParameter<sc_core::sc_time>("clock", sc_core::sc_time(10, sc_core::SC_NS), hier_name);
    axi_clock = ::mb::sysc::sdInitParameter<sc_core::sc_time>("axi_clock", clock, hier_name);
    nominal_voltage = ::mb::sysc::sdInitParameter<double>("nominal_voltage", 1, hier_name);
    mb_debug = ::mb::sysc::sdInitParameter<bool>("mb_debug", false, hier_name);
    call_to_default_if = ::mb::sysc::sdInitParameter<bool>("call_to_default_if", false, hier_name);
    verbose_parameters = ::mb::sysc::sdInitParameter<bool>("verbose_parameters", true, hier_name);
    dmi_enabled = ::mb::sysc::sdInitParameter<bool>("dmi_enabled", true, hier_name);
    warning_level = ::mb::sysc::sdInitParameter<const char*>("warning_level", "WARNING", hier_name);
    slave_pipeline_length = ::mb::sysc::sdInitParameter<unsigned int>("slave_pipeline_length", 2, hier_name);
    slave_outstanding = ::mb::sysc::sdInitParameter<unsigned int>("slave_outstanding", 1, hier_name);
    slave_read_data_queue_size = ::mb::sysc::sdInitParameter<unsigned int>("slave_read_data_queue_size", 0, hier_name);
    slave_write_data_queue_size = ::mb::sysc::sdInitParameter<unsigned int>("slave_write_data_queue_size", 0, hier_name);
    size_in_bytes = ::mb::sysc::sdInitParameter<unsigned long long>("size_in_bytes", 0x10000000, hier_name);
    endianness = ::mb::sysc::sdInitParameter<const char*>("endianness", "target", hier_name);  

    ::mb::sysc::sdPropagateParameters();
  }
  
  //void set_clock (

};

class sram_pvt : public esl::sc_sim::PVTBaseModel, 
                          public mb::utl::cb_owner,
                          private sram_pvt_param_defaults {

 public:
  typedef sram_pvt self_type;

 public: 
  // Constructor
  sram_pvt(sc_core::sc_module_name module_name); 
  sram_pvt(sc_core::sc_module_name module_name, sc_core::sc_time clock_init, sc_core::sc_time axi_clock_init, double nominal_voltage_init, bool mb_debug_init, bool call_to_default_if_init, bool verbose_parameters_init, bool dmi_enabled_init, const char* warning_level_init, unsigned int slave_pipeline_length_init, unsigned int slave_outstanding_init, unsigned int slave_read_data_queue_size_init, unsigned int slave_write_data_queue_size_init, unsigned long long size_in_bytes_init, const char* endianness_init); 
  sram_pvt(sc_core::sc_module_name module_name, std::vector<std::pair<char*, unsigned int> > &change_parameters);
  ~sram_pvt(); 

 
 public:
    //access the T instance
  inline sram_t_base* getT() const {
    return m_sram_t;
  }
  
  //access the PV instance
  inline sram_pv* getPV() const {
    return m_sram_pv;
  }

  virtual const SystemCBaseModel* getSystemCBaseModel() const {
    return getT();
  }
  virtual SystemCBaseModel* getSystemCBaseModel() {
    return getT();
  }
  

 private:
  void bind();
  
 

 private:
  
  //T instance
  sram_t_base* m_sram_t;
  
  //PV instance
  sram_pv* m_sram_pv;

 public:
  // External ports

  typedef esl::sc_sim::pvt_target_socket<64, axi_protocol_types, 1, sc_core::SC_ONE_OR_MORE_BOUND> slave_type;
  slave_type slave;

 private:
  // PV connection ports

  typedef mb::tlm20::initiator_socket_cb<64> slave_entry_type;
  slave_entry_type slave_entry;      

 private:



  struct slave_nb_connector : public tlm::tlm_bw_nonblocking_transport_if<>, public ::mb::tlm20::socket_tracer_holder {
    sram_pvt& m_owner;
    slave_nb_connector(sram_pvt& owner) : m_owner(owner) {
    }
    virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, 
                                               tlm::tlm_phase& phase, 
                                               sc_core::sc_time& t) {
      return m_owner.slave.nb_transport_bw((axi_protocol_types::tlm_payload_type&)trans, phase, t);
    }
    virtual ::mb::tlm20::socket_tracer *get_socket_tracer() {
      return &m_owner.slave;
    }
  };
  friend struct slave_nb_connector;
  slave_nb_connector m_slave_nb_connector;

  

  tlm::tlm_sync_enum receive_nb_transport_slave(tlm::tlm_generic_payload& trans,
                                                    tlm::tlm_phase& phase,
                                                    sc_core::sc_time& t) {
    return getT()->receive_nb_transport(0, trans, phase, t);
  }

};



