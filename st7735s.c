#include "st7735s.h"

void st7735s_init() {
  // Force Hardware Reset
  GPIO->BSRR = (1 << RESET_PIN);
}