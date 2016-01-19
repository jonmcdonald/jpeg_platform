
#ifndef TLM_H264D_TOP_H
#define TLM_H264D_TOP_H

//#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc.h"
#include "tlm_utils/simple_target_socket.h"
#include "ldecod.h"
#include "sc_define.h"
#include "global.h"
#include "global_sc.h"
#include <iomanip>

//=========================================================================
// H264 Decoder Control Top Module
//=========================================================================
//SC_MODULE( sc_h264d_top )
class sc_h264d_top
: public sc_module
, public tlm::tlm_mm_interface
{
  typedef tlm::tlm_generic_payload  *gp_ptr_type;
public:
  //-----------------------------------------------------------------------
  // Port/Channel
  //-----------------------------------------------------------------------
  // Clock/Reset
  // sc_in <bool> clk;
  // sc_in <bool> rst_n;
  //TLM2.0 socket
  tlm_utils::simple_target_socket<sc_h264d_top>  target_socket;
  //SC_FIFO port for send transaction and receive response to/from socket manager
  sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr_type> > request_out_port; 
  sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr_type> > response_in_port;
  //-----------------------------------------------------------------------
  // Constructor/Destructor
  //-----------------------------------------------------------------------
  SC_HAS_PROCESS(sc_h264d_top);
  sc_h264d_top( const sc_module_name name,
                const sc_time        accept_delay,            // accept delay (SC_TIME)
                const sc_time        read_response_delay,     // read response delay (SC_TIME)
                const sc_time        write_response_delay     // write response delay (SC_TIME)
              );

  //-----------------------------------------------------------------------
  // Thread/Method
  //-----------------------------------------------------------------------
  // Hardware reset, no uesd
  void m_rst_n();  
  // Decoder main function
  void t_decoder_main();
  void t_decoder_dump_frame();
  void t_decoder_deblock_thread();

  //b_transport() - Blocking Transport
  void
  b_transport
  ( tlm::tlm_generic_payload  &payload
  , sc_time                   &time
  );
  //nb_transport_fw - Non Blocking Transport
  tlm::tlm_sync_enum
  nb_transport_fw                    
  ( tlm::tlm_generic_payload &payload
  , tlm::tlm_phase           &phase
  , sc_core::sc_time         &time
  );  
  // AHB slave wrapper channel
  bool read (sc_uint<32> addr, sc_uint<32> *data);
  bool write(sc_uint<32> addr, sc_uint<32> data);
  bool check_address(sc_uint<32> addr);
  //
  void extract_payload();
  void call_nb_transport_bw();
  void transaction_req();

  //================
  //initiator_socket
  //================
  unsigned int count_trans;  
  unsigned int haddr,hdata;
  unsigned int FrameOutAddr;
  tlm::tlm_generic_payload  *transaction_ptr;
  unsigned char             *data_buffer_ptr;
  unsigned char * MEM;

  void master_burst_write (unsigned int &addr, unsigned char *data, unsigned int length);
  void master_burst_read (unsigned int &addr, unsigned char *data, unsigned int length);
  void deblock_rw(unsigned int addr,unsigned char *data,unsigned int len,bool rw);

  void check_response();
  void test_burst(); //for test socket
  void free(tlm::tlm_generic_payload *transaction_ptr){
        //transaction_ptr->reset();
  }  

  unsigned int STATE;

private:
  //-----------------------------------------------------------------------
  // Private Data
  //-----------------------------------------------------------------------
  //Control Registers
  unsigned int     frame_in_addr;
  unsigned int     frame_out_addr;
  //
  const sc_time    accept_delay;
  const sc_time    read_response_delay;
  const sc_time    write_response_delay;

  // Command reigster
  int dec_start;
  int dec_stop;
  int dec_resume;
  int dec_software_rst;
  
  //tlm variable
  tlm::tlm_generic_payload  *payload_ptr;
  tlm::tlm_phase            cur_phase, next_phase;
  tlm::tlm_sync_enum        return_sync_enum;
  sc_time                   delay;
  sc_event                  event_call_nb_bw;
  sc_event                  event_start;
  sc_event                  event_dump_next_frame;
  bool                      flag_dump_frame;
  //--bus semaphore
  bool flag_in_req_bus;
  bool flag_out_req_bus;
  bool flag_deblock_req_bus;
  int  flag_sem_in0_deblock1_out2;
  int  show_cur_tran;
};

#endif
