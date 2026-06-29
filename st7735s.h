#include "stm32f4xx.h"
/*
 * @brief Command Registers
 */
// SYSTEM ADDRESSES
#define ST7735S_NOP 0x00
#define ST7735S_SWRESET 0x01
#define ST7735S_SLPOUT 0x11
#define ST7735S_NORON 0x13
#define ST7735S_DISPOFF 0x28
#define ST7735S_DISPON 0x29
#define ST7735S_MADCTL 0x36
#define ST7735S_COLMOD 0x3A

// READ DISPLAY SETTING ADDRESSES - MIGHT not Need
#define ST7735S_RDDST 0x09
#define ST7735S_RDDPM 0x0A
#define ST7735S_RDDMADCTL 0x0B
#define ST7735S_RAMRD 0x2E

// WRITE DISPLAY SETTING ADDRESSES
#define ST7735S_CASET 0x2A // COLUMN PAGE SETUP
#define ST7735S_RASET 0x2B // ROW PAGE SETUP
#define ST7735S_RAMWR 0x2C

/*
 * @brief Physical Pins Mapped to STM32F401
 * TODO: DETERMINE CORRECT PINS
 */
#define RESET_PIN 0
#define A0_PIN 0
#define SCK_PIN 0
#define SDA_PIN 0
#define CS_PIN 0

/*
 * @brief Initializes ST7735S
 */
void st7735s_init(void);