#include "itm.h"
#include "core_cm4.h"

void itm_init(void) {
  // enable TPIU TRCENA BIT (i lowkey dont know what dat mean)
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  // Unlock ITM Registers
  ITM->LAR |= ITM_REGISTER_LOCK_KEY;
}

void itm_sendchar(uint8_t ch) {}