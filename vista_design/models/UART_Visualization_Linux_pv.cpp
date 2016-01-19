
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
//* This file contains the PV class for UART_Visualization_Linux.
//* This is a template file: You may modify this file to implement the 
//* behavior of your component. 
//* 
//* Model Builder version: 3.0.2.beta1
//* Generated on: Oct. 24, 2010 12:15:47 PM, (user: monas)
//* Automatically merged on: Nov. 21, 2010 11:30:59 AM, (user: monas)
//* Automatically merged on: Feb. 04, 2011 10:36:55 AM, (user: zamir)
//* Automatically merged on: Feb. 22, 2012 07:00:44 PM, (user: zamir)
//* Automatically merged on: Aug. 26, 2013 11:45:50 AM, (user: ahanga)
//* Automatically merged on: Oct. 11, 2013 04:28:23 PM, (user: johns)
//* Automatically merged on: Jan. 09, 2014 07:31:19 PM, (user: johns)
//*>


#include "UART_Visualization_Linux_pv.h"
#include <iostream>

template <class Character>  //To handle const char* - unsigned char *
static void to_printable(Character *buf, int len, char *out_buf)
{
  while (len--) {
    int c = *buf++;
    if (isprint(c) || isspace(c) || iscntrl(c))
      *out_buf++ = c;
    else {
      sprintf(out_buf, "\\x%02x", c);
      out_buf += strlen(out_buf);
    }
  }
  *out_buf = '\0';
}

class con_io_buffered_write : public mb::physical_io::buffered_write
{
public:
  con_io_buffered_write(mb::physical_io::console *con_io) :
    m_con_io(con_io) {
  }
  virtual int vflush(const unsigned char* buf, unsigned int buf_len) {
    return m_con_io->write((const char*) buf, buf_len);
  }
private:
  mb::physical_io::console* m_con_io;
};


//constructor
UART_Visualization_Linux_pv::UART_Visualization_Linux_pv(sc_module_name module_name) 
  : UART_Visualization_Linux_pv_base(module_name) {


  con_io = new mb::physical_io::console("Console", "rw", "-fn 10x20");
  m_buffered_write = new con_io_buffered_write(con_io);

  con_io->set_line_mode(line_mode);
  con_io->set_echo_mode(echo_mode);
  con_io->set_xon_mode(xon_mode);

  //Initialize nUARTRTS
  nUARTRTS.initialize(false);

  //Set wait period for receive from console thread
  set_receive_from_console_polling_time();
  SC_THREAD(receive_from_console);

  //Enable receive
    m_console_receive_enable = true;
}    

// Read callback for UARTRXD port.
// Returns true when successful.
bool UART_Visualization_Linux_pv::UARTRXD_callback_read(mb_address_type address, unsigned char* data, unsigned size) {
  #ifdef MB_DEBUG
  std::cout << name() << "::UARTRXD_callback_read, address = 0x" << std::hex << address << ", size = " << std::dec << size << std::endl;
  #endif
  return true;
}


// Write callback for UARTRXD port.
// Returns true when successful.
bool UART_Visualization_Linux_pv::UARTRXD_callback_write(mb_address_type address, unsigned char* data, unsigned size) {
  #ifdef MB_DEBUG
  std::cout << name() << "::UARTRXD_callback_write, address = 0x" << std::hex << address << ", data = {";
  for (unsigned u=0; u<size; u++) {
    std::cout << std::hex << "0x" << (unsigned int)(data[u]);
    if (u+1 != size) {
      std::cout << ", ";
    }
  }
  std::cout << "}, size = " << std::dec << size << std::endl;
  #endif

//  char* buf = new char[size * 2];
//  char buf[size * 2];
//  to_printable(data, size, buf);


//  fprintf(stderr, "write to console1: \n");
//  cout << buf << endl;

  #if defined MB_DEBUG_VisualizationObject & (MB_DEBUG_VisualizationObject >= 1)
    cout << this-> name() << " @ " << sc_time_stamp() << " Writing to RX console data: " << data << " ; 0x" << hex << int(data[0]) << endl;
  # endif

  if (con_io)
    //    con_io->write(buf);
    m_buffered_write->write(data, size);

  return true;

}


unsigned UART_Visualization_Linux_pv::UARTRXD_callback_read_dbg(mb_address_type address, unsigned char* data, unsigned size) {
  return 0;
}

unsigned UART_Visualization_Linux_pv::UARTRXD_callback_write_dbg(mb_address_type address, unsigned char* data, unsigned size) {
  return 0;
}

bool UART_Visualization_Linux_pv::UARTRXD_get_direct_memory_ptr(mb_address_type address, tlm::tlm_dmi& dmiData) {
  return false;
}

 

// callback for any change in signal: nUARTCTS of type: sc_in<bool>
void UART_Visualization_Linux_pv::nUARTCTS_callback() {

 # if defined(MB_DEBUG_VisualizationObject) & MB_DEBUG_VisualizationObject > 1
    if(nUARTCTS == 1) {
      cout << this-> name() << " @ " << sc_time_stamp() << " nUARTCTS is deasserted (HIGH) " << endl;
    }
    else {
      cout << this-> name() << " @ " << sc_time_stamp() << " nUARTCTS is asserted (LOW) " << endl;
    }
  # endif 
 
    if (con_io) {
      m_buffered_write->flush();
      if(nUARTCTS == 0) {//If assered
        //Check if transmission should start
        con_io->write("Kindly resume transmission.\n");
      }
      else {
        con_io->write("Please stop transmission. Wait until data are read.\n");
      }
    }
}



void UART_Visualization_Linux_pv::receive_from_console() {
//SC_THREAD that reads from the console 

  while(1) {
    if (m_console_receive_enable) {
    char buf[read_from_console_buffer_size];
    int r = con_io->b_read(buf, sizeof(buf), m_console_read_wait_period);
    //    int r = con_io->read(buf, sizeof(buf));
    if (r > 0) {
      m_buffered_write->flush();
      #if defined MB_DEBUG_VisualizationObject & (MB_DEBUG_VisualizationObject >= 1)            
      char out_buf[read_from_console_buffer_size*2];  
      to_printable(buf, r, out_buf);
      cout << this-> name() << " @ " << sc_time_stamp() << " Reading from TX console: " << out_buf;   //No endl is needed
      # endif
  
      to_UART(buf,r);
    }
    else if (r == 0) {
      //fprintf(stderr, "NO_DATA");
    }
    else
      break;
    }

    //    wait(m_console_read_wait_period); 
  }

  if ( (errno != EPIPE) && (m_console_receive_enable == true) )
    perror("Failed");  

  delete con_io;
  con_io = 0;
}

void UART_Visualization_Linux_pv::to_UART(char *buf, int len) {
  //send received data to UART
  while (len--) {
    unsigned char c = *buf++;
    //    if(c == '\n') break;
    #if defined MB_DEBUG_VisualizationObject & (MB_DEBUG_VisualizationObject >= 1)
    cout << this-> name() << " @ " << sc_time_stamp() << " Sending to UART Data: " << c << " ; 0x" << hex << int(c) << endl;
    # endif
    UARTTXD_write(0, &c ,1);

  }
}



void UART_Visualization_Linux_pv::set_receive_from_console_polling_time() {
// Set wait period of receive_from_console() SC_THREAD according to parameters
sc_time_unit SCTimeUnit;
switch (read_from_console_latency_time_unit[0]) {
  case 'f':
  case 'F':
    SCTimeUnit = SC_FS;
    break;
  case 'p':
  case 'P':
    SCTimeUnit = SC_PS;
    break;
  case 'n':
  case 'N':
    SCTimeUnit = SC_NS;
    break;
  case 'u':
  case 'U':
    SCTimeUnit = SC_US;
    break;
  case 'm':
  case 'M':
    SCTimeUnit = SC_MS;
    break;
  case 's':
  case 'S':
    SCTimeUnit = SC_SEC;
    break;
  default:
    SCTimeUnit = SC_NS;
    cout << this-> name() <<" Warning! Read from console latency time unit parameter, set to " << read_from_console_latency_time_unit << ", is invalid. Default time unit of NS will be used instead." << endl;    
 }

 sc_time t(read_from_console_latency, SCTimeUnit);
 m_console_read_wait_period = t;

}

// callback for any change in signal: UARTEN of type: sc_in<bool>
void UART_Visualization_Linux_pv::UARTEN_callback() {
//Setting variable to control Rx flow
  if(UARTEN == 0) {
    m_console_receive_enable = false;
  }
  else {
    m_console_receive_enable = true;
  }


}
