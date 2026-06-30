#ifndef itm_h
#define itm_h

#include <stdint.h>

// @brief ITM REGISTER LOCK KEY
#define ITM_REGISTER_LOCK_KEY (0xC5ACCE55)

/*
 * @brief Initializes Trace port and unlocks hardware
 */
void init_itm(void);

/*
 * @brief Transmits single character through SWO pin
 */
void itm_sendchar(uint8_t ch);

#endif