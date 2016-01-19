#include "h264_top.h"

// Constructor
h264_top::h264_top
(
  sc_core::sc_module_name name,
  sc_core::sc_time        accept_delay,            // accept delay (SC_TIME)
  sc_core::sc_time        read_response_delay,     // read response delay (SC_TIME)
  sc_core::sc_time        write_response_delay     // write response delay (SC_TIME)
)
: sc_module        (name)
// , initiator_socket ("at_init_socket")// TLM socket
// , rst_n ("rst_n")
, m_init_wrapper   ("m_init_wrapper")
, m_h264_ecode     ("m_h264_ecode"  
                   , accept_delay
                   , read_response_delay
                   , write_response_delay
                   )
{
    //send requesting payload to wrapper 
    m_h264_ecode.request_out_port (m_request_fifo);
    m_init_wrapper.request_in_port(m_request_fifo);
    //check wrppaer response
    m_init_wrapper.response_out_port(m_response_fifo);
    m_h264_ecode.response_in_port   (m_response_fifo);
    //system signal
    // m_h264_ecode.clk (clk);
    // m_h264_ecode.rst_n (rst_n);
    //TLM2.0 sokcket
    // m_init_wrapper.initiator_socket (initiator_socket); // frame out, stream in
    // target_socket(m_h264_ecode.target_socket); //control register access
    m_init_wrapper.initiator_socket(m_h264_ecode.target_socket);

}

