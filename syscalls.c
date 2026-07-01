#include "itm.h"
#include <sys/types.h>

register char *stack_ptr asm("sp"); // tell gcc to keep this in a CPU register
// asm("sp") stores stack_ptr into register R13 -> physical stack pointer
// register

// syscall performed from malloc when heap is exhausted
caddr_t _sbrk(int incr) {
  extern char __bss_end__;
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0) {
    heap_end = &__bss_end__;
  }
  prev_heap_end = heap_end;

  // check if stack and heap collide (in SRAM, stack grows down, heap grows up)
  if (heap_end + incr > stack_ptr) {
    while (1) {
    }
  }

  heap_end += incr;
  return (caddr_t)prev_heap_end;
}

// write syscall
int _write(int file, char *ptr, int len) {
  (void)file;
  for (int i = 0; i < len; ++i) {
    itm_sendchar(*ptr++);
  }

  return len;
}