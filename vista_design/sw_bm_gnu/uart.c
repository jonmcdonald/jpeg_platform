/*
** Copyright (C) ARM Limited, 2006. All rights reserved.
*/

/*
** This file contains UART Control Registers and simple
** UART I/O functions in ISSM
*/

#include "uart.h"
#include "gic.h"
#include "stdio.h"
/*
 * UART Control Registers
 * in seperate section so it can be placed correctly using scatter file
 */

UART_t *uart = (UART_t*)0x40002000;

void UartInterruptHandler(void)
{
  unsigned mis_status = (uart->MIS & 0x0000FFFF);
  
  if (mis_status & (UARTMIS_RX | UARTMIS_RT)  )
  {
    //printf("Uart Rx Interrupt Received\n");
    //printf("mis_status is %x\n", mis_status);    
    uart->IRQClear = (UARTICR_RX | UARTICR_RT);  
  }

  else if (mis_status & UARTMIS_TX)
  {
    //printf("Uart Tx Interrupt Received\n");
    //printf("mis_status is %x\n", mis_status);
    uart->IRQClear = UARTICR_TX;
  } 
}

/* UART Init Function */
void UART_init(void)
{ 
   unsigned mis_status = 0;   
 
  /* Set Interrupt mask Register */
  uart->MSC = (UARTMSC_RX | UARTMSC_RT);
  
    /* Enable UART for Transmit and Receive */
  uart->LCR_H = UARTLCR_H_FIFOENABLE;
  uart->Ctrl = UARTCR_ENABLE | UARTCR_TXENABLE | UARTCR_RXENABLE;   

  gic_register_handler(32, &UartInterruptHandler);
  gic_set_target_2_interrupt(32, 0);
}

/* UART Write Function */
void uart_putc_polled(char c)
{
    while (uart->Flags & UARTFR_TXFIFOFULL) {     
    }
    uart->Data = c;
}

/* UART Read Function */
char uart_getchar(void)
{
  char ch;

  while (uart->Flags & UARTFR_RXFIFOEMPTY) {
    asm("wfi");
  }
  ch = uart->Data;
  uart_putc_polled(ch);

  return ch;
}

