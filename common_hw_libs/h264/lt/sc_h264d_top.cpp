#include "sc_h264d_top.h"
#include <iomanip>

#define dbg_info 1
//extern unsigned int  BitsLength;
//=============================================================================
//                                Constructor
//(author:cylin)===============================================================
sc_h264d_top::sc_h264d_top( 
                const sc_module_name name,
                const sc_time        accept_delay,            // accept delay (SC_TIME)
                const sc_time        read_response_delay,     // read response delay (SC_TIME)
                const sc_time        write_response_delay     // write response delay (SC_TIME)
)
: sc_module            (name)
, accept_delay         (accept_delay)
, read_response_delay  (read_response_delay)
, write_response_delay (write_response_delay)
{
  MEM = (unsigned char *) calloc(29885*0xc, 1);//local memory in H264
  if(dbg_info) std::cout<<"====== H264_target_socket: ver 1.0 ======"<<std::endl;
  //register socket methods
  target_socket.register_b_transport(this, &sc_h264d_top::b_transport);
  target_socket.register_nb_transport_fw(this, &sc_h264d_top::nb_transport_fw );

  //sc process
  //SC_CTHREAD(t_decoder_main, clk.pos());
  //reset_signal_is(rst_n, false);

  SC_THREAD(t_decoder_main);
  SC_THREAD(transaction_req);

  SC_METHOD(call_nb_transport_bw);
  sensitive<<event_call_nb_bw;
  dont_initialize();
  
  //initialization tlm2 payload ,initiator_socket
  transaction_ptr  = new tlm::tlm_generic_payload(this);
  data_buffer_ptr  = new unsigned char [4];
  glb_frame_done   = 0;
  //transaction_ptr->set_data_ptr(data_buffer_ptr);
  //transaction_ptr->set_data_ptr(MEM);
}


//=============================================================================
//                         Initiator Interface                               //
//--send payload to initiator socket wrapper via SC_FIFO                     //
//(author:cylin)===============================================================
void sc_h264d_top::check_response()
{
    response_in_port->read(transaction_ptr);

    if(transaction_ptr ->get_response_status() != tlm::TLM_OK_RESPONSE){
        cout<<"sc_h264d_top ERROR:response_status() != TLM_OK_RESPONSE"<<endl;
        sc_core::sc_stop();
    }
       
    transaction_ptr->release();
    
} // end check_complete

void 
sc_h264d_top::master_burst_write( unsigned int &addr, unsigned char * data, unsigned int length)
{

    // write transaction
    transaction_ptr->acquire();
    transaction_ptr->set_command          ( tlm::TLM_WRITE_COMMAND       );
    transaction_ptr->set_address          ( addr );
    //transaction_ptr->set_data_length      ( 4 );
    transaction_ptr->set_data_length      ( length );//boy
    transaction_ptr->set_streaming_width  ( length );//boy
    transaction_ptr->set_data_ptr         (data);
    transaction_ptr->set_response_status  ( tlm::TLM_INCOMPLETE_RESPONSE );
    // send request to fifo
    request_out_port->write (transaction_ptr);
    // check response
    check_response();

}

//===================================
//   master burst read
void 
sc_h264d_top::master_burst_read( unsigned int &addr, unsigned char * data, unsigned int length)
{
    // write transaction 
    transaction_ptr->acquire();
    transaction_ptr->set_command          ( tlm::TLM_READ_COMMAND );
    transaction_ptr->set_address          ( addr );
    //transaction_ptr->set_data_length      ( 4 );
    transaction_ptr->set_data_length      ( length );//boy
    transaction_ptr->set_streaming_width  ( length );//boy
    transaction_ptr->set_data_ptr         ( data );
    transaction_ptr->set_response_status  ( tlm::TLM_INCOMPLETE_RESPONSE );
    // send request to fifo
    request_out_port->write (transaction_ptr);
    // check response
    check_response();
}

//=============================================================================
//=============================================================================
//                              nb_transport_fw
//(author:cylin)===============================================================
tlm::tlm_sync_enum 
sc_h264d_top::nb_transport_fw
( tlm::tlm_generic_payload &payload
, tlm::tlm_phase           &phase
, sc_core::sc_time         &time)
{
  if(dbg_info) std::cout<<sc_core::sc_time_stamp()<<"\tH264.nb_transport_fw: "<<phase<<", \t"<<time<<std::endl;
  switch(phase)
  {
    case tlm::BEGIN_REQ:
      payload_ptr = &payload;
      payload_ptr->acquire();
       return_sync_enum = tlm::TLM_ACCEPTED;
      //next transfer
      next_phase  = tlm::END_REQ;
      event_call_nb_bw.notify(accept_delay+time);
      time  = SC_ZERO_TIME;
    break;

    case tlm::END_RESP:      
      payload_ptr->release();
      time = SC_ZERO_TIME;
      //return_sync_enum = tlm::TLM_ACCEPTED;
      return_sync_enum = tlm::TLM_COMPLETED; //for tlm20.example.simpleAT bus
    break;

    default:
      cout<<"h264: error phase  "<<endl;
      sc_stop();
    break;
  }//end switch

  return return_sync_enum;  
} //end nb_transport_fw


//(author:cylin)===============================================================
void sc_h264d_top::call_nb_transport_bw()
{
  if(dbg_info) std::cout<<sc_core::sc_time_stamp()<<"\tH264.call_nb_bw.next_phase: \t"<<next_phase<<std::endl;
  switch(next_phase)
  {
    case tlm::END_REQ:
    {
      delay = SC_ZERO_TIME;
      cur_phase = next_phase;
      return_sync_enum = target_socket->nb_transport_bw(*payload_ptr, cur_phase, delay);
      if(return_sync_enum != tlm::TLM_ACCEPTED){
        std::cout<<sc_core::sc_time_stamp()<<"\tH264.call_nb_bw.return error: \t"<<return_sync_enum <<std::endl;
        sc_stop();
      }
      //next transfer
      next_phase = tlm::BEGIN_RESP;
      if(payload_ptr->get_command() == tlm::TLM_WRITE_COMMAND){
        event_call_nb_bw.notify(write_response_delay+delay);
      }
      else{
        event_call_nb_bw.notify(read_response_delay+delay);
      }
      break;
    }
    case tlm::BEGIN_RESP:
    {
      //extract payload
      extract_payload();
      //payload_ptr->set_response_status(tlm::TLM_OK_RESPONSE);
      //---------------
      delay = SC_ZERO_TIME;
      cur_phase = next_phase;
      return_sync_enum = target_socket->nb_transport_bw(*payload_ptr, cur_phase, delay);
      if(return_sync_enum != tlm::TLM_ACCEPTED){
        std::cout<<sc_core::sc_time_stamp()<<"\tH264.call_nb_bw.return error: \t"<<return_sync_enum <<std::endl;
        sc_stop();
      }      
      break;
    }
    default:
    break;
  }//end switch
}

//=============================================================================
//                               b_transport
//(author:cylin)===============================================================
void                                        
sc_h264d_top::b_transport
( tlm::tlm_generic_payload  &payload                // ref to  Generic Payload 
, sc_core::sc_time          &time             // delay time 
)
{
  //Extract Payload
  payload_ptr = &payload;
  //extract_payload();  
    
  if(payload_ptr->get_command() == tlm::TLM_WRITE_COMMAND)
    time = time + accept_delay + write_response_delay;
  else
    time = time + accept_delay + read_response_delay;
  
  wait(time);
  time = SC_ZERO_TIME;
  
  extract_payload();    
  return;     
}

//-(Method)----------------------------------------------------------------
// Extract Payload
//-------------------------------------------------------------------------
void sc_h264d_top::extract_payload()
{
  if(dbg_info) std::cout<<"H264: ===extract_payload()==="<<std::endl;
  tlm::tlm_response_status response_status;
  sc_uint<32> tmp;
  uint64 previous_addr, masked_addr;
  // Access the required attributes from the payload
  uint64                   address   = payload_ptr->get_address();     // memory address
  tlm::tlm_command         command   = payload_ptr->get_command();     // memory command
  unsigned char            *data_ptr = payload_ptr->get_data_ptr();    // data pointer
  unsigned int             length    = payload_ptr->get_data_length(); // data length
  
  uint64 every_addr = address;
  bool check = 1;
  for (unsigned int i = 0; i < length; i++)
  {
    ///if(!check_address(every_addr))
    if(check_address(every_addr))
      check = 0;
    every_addr = every_addr + i;
  }

  if(check == 1)
    response_status = tlm::TLM_OK_RESPONSE;
  else
  {
    cout << name() << ": decoding error, addr=0x" << hex << address << ", length=0x" << length << "\n";
    payload_ptr->set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE);
    return;
  }

  if (payload_ptr->get_byte_enable_ptr())
  {
    payload_ptr->set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    return;
  }
  else if (payload_ptr->get_streaming_width() != payload_ptr->get_data_length()) 
  {
    payload_ptr->set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return;
  }

  switch (command)
  {
    default:
    {
      payload_ptr->set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
      cout << name() <<": Unsupport GP command extension\n";
      //time = sc_core::SC_ZERO_TIME;
      break;
    }
    // Setup a TLM_WRITE_COMMAND Informational Message and Write the Data from
    // the Generic Payload Data pointer to Memory
    case tlm::TLM_WRITE_COMMAND:
    {
      if (response_status == tlm::TLM_OK_RESPONSE)
      {
        tmp = 0;
        for (unsigned int i = 0; i < length; i++)
        {
          // Call to write must be a WORD based, one cannot call write BYTE by BYTE.
          //Collect data[i] until a word-aligned data is full
          if(address % 4 == 0)
            tmp = data_ptr[i];
          else if(address % 4 == 1)
            tmp = tmp | data_ptr[i] << 8;
          else if(address % 4 == 2)
            tmp = tmp | data_ptr[i] << 16;
          else if(address % 4 == 3)
            tmp = tmp | data_ptr[i] << 24;

          if(i == length-1 || address % 4 == 3){
            write(address & 0xFFFFFFFC, tmp);
            tmp = 0;
            //time = time + write_response_delay;
          }
          address++;
        }
      }
      break;
    }
    case tlm::TLM_READ_COMMAND:
    {
      if (response_status == tlm::TLM_OK_RESPONSE)
      {
        previous_addr = 0x0;
        for (unsigned int i = 0; i < length; i++)
        {
          masked_addr = address & 0xFFFFFFFC;
          if(i == 0 || masked_addr != previous_addr){
            read(masked_addr, &tmp);
          }

          if(address % 4 == 0)
            data_ptr[i] = tmp;
          else if(address % 4 == 1)
            data_ptr[i] = tmp >> 8;
          else if(address % 4 == 2)
            data_ptr[i] = tmp >> 16;
          else if(address % 4 == 3)
            data_ptr[i] = tmp >> 24;

          previous_addr = masked_addr;
          address++;
        }
        //time = time + read_response_delay;
      }
      break;
    }
  } // end switch

  payload_ptr->set_response_status(response_status);
}

//-(Method)----------------------------------------------------------------
// Hardware Reset
//-------------------------------------------------------------------------
void sc_h264d_top::m_rst_n()
{
  //dec_start=0;
  dec_stop=1;
  dec_resume=0;
  dec_software_rst=0;
}

//-(Channel)---------------------------------------------------------------
// AHB Slave Wrapper Channel Read
//-------------------------------------------------------------------------
bool sc_h264d_top::read( 
  sc_uint<32> addr, sc_uint<32> *data)
{
//  int flag_1bit_frame_done;
//  int flag_3bit_frame_type;
//  int flag_1bit_bitstream_buff_low;

  // Read register
  switch (addr & 0xFFFF)
  {
  case H264D_ADR_START:
    *data = 0x0; // write-only register
    break;

  case H264D_ADR_STOP:
    *data = 0x0; // write-only register
    break;

  case H264D_ADR_RESUME:
    break;

  case H264D_ADR_SW_RST:
    break;

  case H264D_ADR_INTERRUPT:
    *data = flag_interrupt;
    break;

  case H264D_ADR_FRAME_DONE:
    cout<<name()<<": check frame_done:"<<glb_frame_done<<endl;
    *data = glb_frame_done;  // read-only register
    break;

  case H264D_ADR_FRAME_TYPE:
    *data = flag_3bit_frame_type;  // read-only register
    break;

  case H264D_ADR_BITSTREAM_BUFF_LOW:
    *data = flag_1bit_bitstream_buff_low;  // read-only register
    break;

  case H264D_ADR_BASE_ADDR_Y:
    break;

  case H264D_ADR_BASE_ADDR_U:
    break;

  case H264D_ADR_BASE_ADDR_V:
    break;

  case H264D_ADR_BASE_ADDR_BITSTREAM:
    break;

  case H264D_ADR_BASE_BITSTREAM_SIZE:
    break;

  default:
    break;
  }
  return true;
}

//-(Channel)---------------------------------------------------------------
// AHB Slave Wrapper Channel Write
//-------------------------------------------------------------------------
bool sc_h264d_top::write(
  sc_uint<32> addr, sc_uint<32> data)
{ 
  // Write register
  switch (addr & 0xFFFF)
  {
  case H264D_ADR_START:
    FrameOutAddr = frame_out_addr;
    if(data == 0x1){ 
      //dec_start=1;
      event_start.notify();
      cout << "H264 start" <<data<<endl;
    }
    break;

  case H264D_ADR_STOP:
    dec_stop = data;
    break;

  case H264D_ADR_RESUME:
    break;

  case H264D_ADR_SW_RST:
    break;

  case H264D_ADR_INTERRUPT:
    break;

  case H264D_ADR_FRAME_DONE:
    FrameOutAddr   = frame_out_addr;
    glb_frame_done = data;
    cout<<name()<<":turn off frame_done:"<<glb_frame_done<<endl;
    event_frame_done.notify();
    break;

  case H264D_ADR_FRAME_TYPE:
    break;

  case H264D_ADR_BITSTREAM_BUFF_LOW:
    break;

  case H264D_ADR_BASE_ADDR_Y:
    frame_out_addr = data;
    break;

  case H264D_ADR_BASE_ADDR_U:
    break;

  case H264D_ADR_BASE_ADDR_V:
    break;

  case H264D_ADR_BASE_ADDR_BITSTREAM:
    frame_in_addr = data;
    break;

  case H264D_ADR_BASE_BITSTREAM_SIZE:
    break;

  default:
    break;
  }
  return true;
}

//-(Channel)---------------------------------------------------------------
// check_address
//-------------------------------------------------------------------------
bool sc_h264d_top::check_address(sc_uint<32> addr)
{
  // Read register
/*
  switch (addr & 0xFFFC)
  {
  case H264D_ADR_START:
  case H264D_ADR_INTR_FLAGENA:
  case H264D_ADR_PIC_WIDTH:
  case H264D_ADR_PIC_HEIGHT:
  case H264D_ADR_STR_0_ADDR:
  case H264D_ADR_STR_0_SIZE:
  case H264D_ADR_OUTPUT_ADDR:
  case H264D_ADR_DISPLAY:
    return 1;
    break;
  default:
    return 0;
    break;
  }
*/
  return 0;
}

//-(Thread)----------------------------------------------------------------
// Decoder Main Function
//-------------------------------------------------------------------------
void sc_h264d_top::t_decoder_main()
{
  int done;
//=================================================================
//           Reading bitsteam back to MEM[]
//=================================================================
  while (1)
  {
    STATE=1;
    wait(event_start); 
    m_rst_n();
    flag_interrupt=0;
    done=1;

    unsigned int length = 358624;// Bitstream length
    unsigned int src_addr = frame_in_addr;
    unsigned int width = 12;
    unsigned int count = length/width;
    unsigned int offset = 0;

    // printf("count:%x\n",count+1);
    
    for (unsigned int i = 0; i<count+1; i++)
      {
	//     printf("i:%d of %d, src %x , mem %x, w %x\n",i,count,src_addr,MEM+offset,width);
	master_burst_read( src_addr, MEM+offset, width);
	src_addr+=width;
	offset+=width;
      } 
    
    //printf("Decode %x,%x\n",MEM,length);
    STATE=0;
    done = ldecod(MEM, length);
    STATE=1;

    printf("Done");
    if(done==0){ 
      flag_interrupt=1;
    }
  } 
}

//=============================================================================
//                Transaction Requestion: R/W data from/to memory
//(author:cylin)===============================================================
void sc_h264d_top::transaction_req()
{
  //haddr = 0x2f000000;
  while(1){
    wait(event_transaction_req);
    //printf("Write output %x %x\n", FrameOutAddr, tlmDataLenByte);
    //   for (int i=0;i<12;i++){
    //	       printf("------------Data %x \n", tlmDataBuffer[i]);
    //	 }
    //master_burst_write( haddr, tlmDataBuffer, tlmDataLenByte);   
    unsigned int addr=FrameOutAddr;
    master_burst_write( addr, tlmDataBuffer, tlmDataLenByte);  
    FrameOutAddr=addr;

    //haddr += tlmDataLenByte;
    FrameOutAddr +=tlmDataLenByte;
    event_transaction_end.notify(SC_ZERO_TIME);
  }
}

//-----test bus/memory transaction protocal-----
void sc_h264d_top::test_burst()
{
    unsigned int length = 53;    
    //unsigned int length = 1513;    
    unsigned int src_addr = 0x10000000;
    unsigned int dest_addr = 0x20000000;
    master_burst_read( src_addr, MEM, length);// reading bitsteam back
    cout<<"TEMP: MEM="<<endl;
    for (unsigned int i = 0; i<length+1; ++i) 
    {cout<<":"<<MEM[i];}
    master_burst_write( dest_addr, MEM, length);// reading bitsteam back
}
