
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <unistd.h>

#include "uart.h"

unsigned long __wrap__write(int fd, const char *buf, size_t count)
{
  if (fd > 2) {
    return __real__write(fd, buf, count);
  }

  size_t u = 0;
  for (u = 0; u < count; u++) {
    uart_putc_polled(buf[u]);
  }
  return count;
}

unsigned long __wrap__read(int fd, char *buf, size_t count)
{
  if (fd > 2) {
    return __real__read(fd, buf, count);
  }

  if (count == 0)
    return 0;
  buf[0] = uart_getchar();
  return 1;
}

#if 0
void _close(int fd)
{
}

int _isatty(int fd)
{
  return 0;
}

off_t _lseek(int fd, off_t offset, int whence)
{
  return offset;
}

int _fstat(int fd, struct stat *buf)
{
  return 1;
}
#endif

unsigned long mgc_semihosting(unsigned opcode, unsigned long arg0, unsigned long arg1)
{
  unsigned long retv;
  asm volatile("mov r0, %[opcode] \n"
               "mov r1, %[arg0] \n"
               "mov r2, %[arg1] \n"
               "svc 0x123458 \n"
               "mov %[result], r0\n"
               : [result]"=r" (retv)
               : [opcode]"r" (opcode), [arg0]"r" (arg0), [arg1]"r" (arg1)
               : "r0", "r1", "r2"
               );
  return retv;
}

#define MB_STOP_OFFSET         0x84
#define MGC_SEMIHOSTING_WRITE  1

void _exit(int value)
{
  mgc_semihosting(MGC_SEMIHOSTING_WRITE, MB_STOP_OFFSET, value);
  while (1);
}

clock_t times(struct tms *buf)
{
  buf->tms_utime = 0;
  buf->tms_stime = 0;
  buf->tms_cutime = 0;
  buf->tms_cstime = 0;
  
  return 10;
}
