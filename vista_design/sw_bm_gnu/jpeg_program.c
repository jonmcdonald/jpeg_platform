
#include "jpeg_program.h"
#include "gic.h"
#include <stdio.h>
#include <math.h>

#include "mb/sw/control.h"

static volatile int flag;

void JpegInterruptHandler(void)
{
  // Acknowledge interrupt
  volatile unsigned int * reg_address;
  reg_address = (unsigned int *) (JPEG_BASE_ADDRESS + STATUS);
  *reg_address = 0;

  flag = 1;

  mb_core_print(100);
}

void jpeg_init()
{
  gic_register_handler(34, &JpegInterruptHandler);
  gic_set_target_2_interrupt(34, 0);
}

#if 0
unsigned int create_checksum1(unsigned char* ptr, unsigned int length)
{
  int i, j, sum = 1;
  printf("Started checksum.\n");
  unsigned char* end_ptr = ptr + length;
  for (; ptr < end_ptr; ptr += 10000) {
    unsigned long diff = (end_ptr - ptr);
    sum += sqrt(*ptr);
    if (diff % 1000000 == 0) 
      printf("diff=%d sum=%d\n", (int)diff, sum);
  }
  return sum;
}
#endif

unsigned int create_checksum(unsigned char* ptr, unsigned int length)
{
  int sum = 1;
  int i, j;
  //  printf("Started checksum.\n");
  mb_core_print(0xAAAA);
//for (i = 0; i < length/10; i++) {
//  sum += i * i;
//}
  mb_core_print(0xBBBB);
  //  printf("Finished checksum.\n");
  return sum;
}

void jpeg_main (unsigned int clk)
{
  unsigned int input_length = 4 * 3 * MAX_ROWS * MAX_COLUMNS;
  volatile unsigned int * reg_address;
  unsigned int checksum;

  // Program JPEG inputaddress
  reg_address = (unsigned int *) (JPEG_BASE_ADDRESS + INPUTADDR);
  *reg_address = SRAM0_BASE_ADDRESS;

  // Program IO inputaddress
  reg_address = (unsigned int *) (IO_BASE_ADDRESS + INPUTADDR);
  *reg_address = SRAM0_BASE_ADDRESS;

  // Program JPEG inputalength
  reg_address = (unsigned int *) (JPEG_BASE_ADDRESS + INPUTLENGTH);
  *reg_address = input_length;

  // Program IO inputalength
  reg_address = (unsigned int *) (IO_BASE_ADDRESS + INPUTLENGTH);
  *reg_address = input_length;

  // Program JPEG outputaddress
  reg_address = (unsigned int *) (JPEG_BASE_ADDRESS + OUTPUTADDR);
  *reg_address = SRAM0_BASE_ADDRESS + input_length;

  // Program IO outputaddress
  reg_address = (unsigned int *) (IO_BASE_ADDRESS + OUTPUTADDR);
  *reg_address = SRAM0_BASE_ADDRESS + input_length;
  
  // Upload BMP via IO
  reg_address = (unsigned int *) (IO_BASE_ADDRESS + UPLOAD);
  *reg_address = 1;

  mb_core_print(0);

#ifdef VARIANT_SEQ
  puts("Sequential\n");
  checksum =
    create_checksum((unsigned char *)SRAM0_BASE_ADDRESS, input_length);
  //  printf("Value: %u\n", checksum);
#endif

  mb_core_print(1);

  // Start Computation
  reg_address = (unsigned int *) (JPEG_BASE_ADDRESS + START);
  *reg_address = clk;
  flag = 0;

  mb_core_print(2);

#ifdef VARIANT_PAR
  puts("Parallel\n");
  checksum =
    create_checksum((unsigned char *)SRAM0_BASE_ADDRESS, input_length);
  //  printf("Value: %u\n", checksum);
#endif

  mb_core_print(3);

  // Waiting for JPEG Interrupt
  while (flag == 0) {
    asm("wfi");
  }

  mb_core_print(4);

  // Download JPG via IO
  reg_address = (unsigned int *) (IO_BASE_ADDRESS + DOWNLOAD);
  *reg_address = 1;
}

