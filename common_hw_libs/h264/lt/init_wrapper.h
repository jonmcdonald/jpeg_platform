#ifndef __INIT_WRAPPER_H__
#define __INIT_WRAPPER_H__

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

class init_wrapper
: public sc_core::sc_module
{
    typedef tlm::tlm_generic_payload  *gp_ptr_type;

    public:
    //TLM simple initiator socket
    tlm_utils::simple_initiator_socket<init_wrapper> initiator_socket;
    //sc fifo 
    sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr_type> > request_in_port;  
    sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr_type> > response_out_port;

    //Constructor
    SC_HAS_PROCESS(init_wrapper);
    init_wrapper( sc_core::sc_module_name name);
    
    //main process 
    void initiator_thread (void);                    

    private:
    //socket method::nb_transport_bw
    tlm::tlm_sync_enum 
    nb_transport_bw( tlm::tlm_generic_payload& transaction,
                     tlm::tlm_phase&           phase,
                     sc_core::sc_time&         time
                   );  
    //function
    void send_blocking_request();
    void send_nonblocking_request();
    //variable
    gp_ptr_type              payload_ptr;    ///< transaction pointer
    tlm::tlm_response_status gp_status;
    tlm::tlm_sync_enum       gp_sync;
    tlm::tlm_sync_enum       return_value;
    tlm::tlm_phase           gp_phase;
    sc_core::sc_time         m_end_rsp_delay;
    sc_core::sc_time         delay,bw_delay;  
    sc_core::sc_event        event_EndResp;
}; 
#endif /* __INIT_WRAPPER_H__ */
