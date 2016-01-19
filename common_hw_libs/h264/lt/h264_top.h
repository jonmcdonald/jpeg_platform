#ifndef __H264_TOP_H__
#define __H264_TOP_H__

#include "tlm.h"
#include "init_wrapper.h" // simple socket-blocking transport
#include "sc_h264d_top.h" // h264 module

class h264_top 
: public sc_core::sc_module 
{
public:
    typedef tlm::tlm_generic_payload  *gp_ptr_type;
    //TLM 2.0
    // tlm::tlm_initiator_socket<> initiator_socket;
    // tlm::tlm_target_socket<>    target_socket;
    //signal
    // sc_core::sc_in <bool> clk;
    // sc_core::sc_in <bool> rst_n;
	//Constructor
    h264_top( sc_core::sc_module_name name
            , sc_core::sc_time        accept_delay         // accept delay (SC_TIME)
            , sc_core::sc_time        read_response_delay  // read response delay (SC_TIME)
            , sc_core::sc_time        write_response_delay // write response delay (SC_TIME)
            );    
    //interconnect fifo
    sc_core::sc_fifo <gp_ptr_type>  m_request_fifo;
    sc_core::sc_fifo <gp_ptr_type>  m_response_fifo;
    //SC Module   
    init_wrapper     m_init_wrapper;
    sc_h264d_top     m_h264_ecode;
    
private:

};

#endif /* __h264_top_H__*/
