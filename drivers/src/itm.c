#include "itm.h"
#include "stm32f4xx.h"

void itm_init(void) {
  DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;

  // enable TPIU TRCENA BIT (i lowkey dont know what dat mean)
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  // Unlock ITM Registers
  ITM->LAR |= ITM_REGISTER_LOCK_KEY;
}

void itm_sendchar(uint8_t ch) {
  // Check If ITM and Port 0 Are Enabled
  if ((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0U && (ITM->TER & 1UL) != 0UL) {
    // Wait Until Port Is Ready
    while (ITM->PORT[0U].u32 == 0UL) {
      __NOP();
    }

    // Send Over Char
    ITM->PORT[0U].u8 = ch;
  }
}