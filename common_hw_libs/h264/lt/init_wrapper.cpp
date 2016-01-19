#include "init_wrapper.h"
#include "tlm.h"

#define dbg_info 1
#define BLOCKING_TRANS  0

using namespace sc_core;
//=============================================================================
//                                Constructor                                //
//=============================================================================
init_wrapper::init_wrapper
( sc_module_name      name)
: sc_module           (name)
, initiator_socket    ("initiator_socket")
, delay               (SC_ZERO_TIME)
{                
    if(dbg_info) std::cout<<"====== init_wrapper: ver 1.0======"<<std::endl;
    //register socket methods
    initiator_socket.register_nb_transport_bw(this, &init_wrapper::nb_transport_bw);
    //sc process
    SC_THREAD(initiator_thread);
}

//=============================================================================
//                              Main Process                                 //
//=============================================================================
void init_wrapper::initiator_thread(void) 
{  
  while (true) 
  {
    payload_ptr = request_in_port->read();  // get request from input fifo
       
    if(BLOCKING_TRANS)            
      send_blocking_request();
    else
      send_nonblocking_request();
            
    response_out_port->write(payload_ptr); // push response to fifo
  
  }
}

//=============================================================================
//                         send_blocking_request()                           //
//=============================================================================
void init_wrapper::send_blocking_request()
{      
  delay = SC_ZERO_TIME;
  if(dbg_info) std::cout<<sc_core::sc_time_stamp()<<" \t"<<name()
                        <<".Initiator.b_transport(): \t"<<delay<<std::endl;
  initiator_socket->b_transport(*payload_ptr, delay);
  gp_status = payload_ptr->get_response_status();
  
  if(gp_status == tlm::TLM_OK_RESPONSE){
    if(delay != SC_ZERO_TIME)
      wait(delay);
  }
  else{
    std::cout<<"ERROR: gp_status != tlm::TLM_OK_RESPONSE"<<std::endl;
    sc_stop();
  }
}

//=============================================================================
//                         send_nonblocking_request()                        //
//=============================================================================
void init_wrapper::send_nonblocking_request()
{
  gp_phase  = tlm::BEGIN_REQ;
  delay = SC_ZERO_TIME;
  return_value = initiator_socket->nb_transport_fw(*payload_ptr, gp_phase, delay); 

  switch (return_value) 
  {
    case tlm::TLM_ACCEPTED:
      wait(event_EndResp);
      if(bw_delay != SC_ZERO_TIME) wait(bw_delay);// wait the annotated delay 
      
      gp_phase     = tlm::END_RESP;
      delay        = SC_ZERO_TIME;
      return_value = initiator_socket->nb_transport_fw(*payload_ptr, gp_phase, delay);  
      if(dbg_info) std::cout<<sc_core::sc_time_stamp()<<" \t"<<name()
                            <<".nb_transport_fw.reture: \t"<<return_value<<std::endl;
    break;        

    case tlm::TLM_UPDATED: 
      if(gp_phase == tlm::END_REQ)
      {
        wait(event_EndResp);        
        if(bw_delay != SC_ZERO_TIME) wait(bw_delay);// wait the annotated delay 
        
        gp_phase     = tlm::END_RESP;
        delay        = SC_ZERO_TIME;
        return_value = initiator_socket->nb_transport_fw(*payload_ptr, gp_phase, delay);  
        if(dbg_info) std::cout<<sc_core::sc_time_stamp()<<" \t"<<name()
                              <<"nb_transport_fw.reture: \t"<<return_value<<std::endl;
      }
      else {
        std::cout<<sc_core::sc_time_stamp()<<" \t"<<name()
                 <<" Unexpected phase for UPDATED return from target ";
        sc_stop();
      }
    break;

    case tlm::TLM_COMPLETED: 
      if(delay != SC_ZERO_TIME) wait(delay);// wait the annotated delay 
    break;

    default:
      std::cout<<sc_simulation_time()<<"  "<<name()<<".nb_transport_fw.return_value ERROR"<<std::endl;
      sc_stop();
    break;
    
  } // end case
  
}

//-----------------------------------------------------------------------------
//                     Socket Method::nb_transport_bw                        //
//-----------------------------------------------------------------------------
tlm::tlm_sync_enum    
init_wrapper::nb_transport_bw( tlm::tlm_generic_payload&  transaction_ref
                                   , tlm::tlm_phase&            gp_phase
                                   , sc_time&                   delay
                                   )
{
    if(dbg_info) std::cout<<sc_core::sc_time_stamp()<<" \t"<<name()<<".nb_transport_bw.phase: \t"<<gp_phase<<",  \t"<<delay<<std::endl;

    gp_sync  = tlm::TLM_ACCEPTED;
    bw_delay = delay;
    delay    = SC_ZERO_TIME;
    
    if(gp_phase == tlm::BEGIN_RESP)
      event_EndResp.notify(0,SC_NS);

    
    return gp_sync;
} // end backward nb transport

