
#include <stdlib.h>

#include "dhry.h"
#include "uart.h"
#include "jpeg_program.h"
#include "board.h"
#include "mb/sw/control.h"

extern void dhry_main();
extern void board_init();
extern int cjpeg_main(int argc, char** argv);

static void uart_puts(const char* str)
{
  while(*str) {
    uart_putc_polled(*str);
    str++;
  }
}

void run_cjpeg()
{
  static char* argv[] = {
    "prog",
    "-verbose",
    "-progressive",
    "-opt",
    "-outfile",
    "result.jpg",
    "fractal_800x600-png.ppm"
  };
  cjpeg_main(sizeof(argv)/sizeof(*argv), argv);
}

static void print_menu(void)
{
  uart_puts("\n\nType the Number of the required test \n\n");
  uart_puts("1) Run Dhrystone Benchmark \n");
  uart_puts("2) Run HW BMP-JPEG translation (FAST Clock)\n");
  uart_puts("3) Run HW BMP-JPEG translation (SLOW Clock)\n");
  uart_puts("4) Exit the simulation \n");
  uart_puts("\n\n");
}

static void main_primary(void)
{
  char buf[16];
  while(1) {
    char selection = (char)0;
    print_menu();
    selection = uart_getchar();
    uart_puts("\n");

    switch (selection) {
    case '1':
      uart_puts("Selection - 1\n");
      dhry_main();
      break;
    case '2':
      uart_puts("Selection - 2\n");
      jpeg_main(1);
      break;
    case '3':
      uart_puts("Selection - 3\n");
      jpeg_main(2);
      break;
    case '4':
      uart_puts("Selection - 4\n");
      exit(0);
      break;
    default:
      uart_puts("Invalid Selection - Try again\n");
      break;
    }
  }
}

static void main_secondary()
{
  while (1) {
    mb_core_message("Run SW BMP-JPEG translation");
    run_cjpeg();
    mb_core_message("Completed SW BMP-JPEG translation");
    asm volatile ("wfi");
  }
}

int main(void)
{
  int core_id = get_core_id();
  if (core_id == 0) {
    mb_core_message("Primary core in main.");
  } else {
    mb_core_message("Secondary core in main.");
  }

  board_init(); /* also enables ICache */
  cpuif_init();

  if (core_id == 0) {
    gic_init();
    UART_init();
    jpeg_init();
    pagetable_init();
    uart_puts("Board initialized.\n");
    mb_core_message("Board initialized.");
  }

  enable_mmu();
  enable_dcache();

  if (core_id == 0) {
    mb_core_message("Sending trigger.");
    asm volatile("sev");
  }

  if (core_id == 0) {
    main_primary();
  } else {
//  main_secondary();
  }

  return 0;
}

