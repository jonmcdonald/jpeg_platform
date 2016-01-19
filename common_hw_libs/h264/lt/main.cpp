////////////////////////////////////////////////////////////////////////////
// main.cpp

#include "h264_top.h"

int sc_main (int argc, char* argv []) {

  // Instantiate modules
  h264_top * top = new h264_top ("top", sc_time (10, SC_NS), sc_time (10, SC_NS), sc_time (10, SC_NS));

  // sc_signal <bool> rst_n ("rst_n");
  // top->rst_n (rst_n);

  // Simulate

  // rst_n.write (true);
  // sc_start (100, SC_NS);
  // rst_n.write (false);
  // sc_start (100, SC_NS);
  // rst_n.write (true);
  // sc_start (100, SC_NS);

#ifndef MTI_SYSTEMC
  sc_start ();
#else
  sc_start (-1);
#endif
  sc_stop ();

  delete top;

  return 0;

}

