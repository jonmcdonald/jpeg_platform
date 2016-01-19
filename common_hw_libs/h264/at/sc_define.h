#ifndef SC_DEFINES_H
#define SC_DEFINES_H

#define H264D_BASE_ADDR         0x14003000

typedef enum{
  H264D_ADR_START               = 0x00,
  H264D_ADR_STOP                = 0x04,
  H264D_ADR_RESUME              = 0x08, 
  H264D_ADR_SW_RST              = 0x0c,
  H264D_ADR_INTERRUPT           = 0x10,
  H264D_ADR_FRAME_DONE          = 0x14,
  H264D_ADR_FRAME_TYPE          = 0x18,
  H264D_ADR_BITSTREAM_BUFF_LOW  = 0x1C,
  H264D_ADR_BASE_ADDR_Y         = 0x20,
  H264D_ADR_BASE_ADDR_U         = 0x24,
  H264D_ADR_BASE_ADDR_V         = 0x28,
  H264D_ADR_BASE_ADDR_BITSTREAM = 0x2c,
  H264D_ADR_BASE_BITSTREAM_SIZE = 0x30
}h264_status;

#endif
