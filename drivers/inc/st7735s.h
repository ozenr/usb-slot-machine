#ifndef ST7735S_H
#define ST7735S_H

// Internal Libraries
#include "spi.h"

// External Libraries
#include "stm32f4xx.h"
#include <stdint.h>

// Delay_Ms Function from main
extern uint32_t ticks;
extern void delay_ms(uint32_t ms);

// SYSTEM/COMMAND ADDRESSES
#define ST7735S_NOP 0x00U
#define ST7735S_SWRESET 0x01U
#define ST7735S_SLPOUT 0x11U
#define ST7735S_NORON 0x13U
#define ST7735S_DISPOFF 0x28U
#define ST7735S_DISPON 0x29U
#define ST7735S_MADCTL 0x36U
#define ST7735S_COLMOD 0x3AU

// MADCTL & COLMOD Parameters
#define MADCTL_CFG 0x00
#define COLMOD_16_BIT 0x05

// READ DISPLAY SETTING ADDRESSES
#define ST7735S_RDDST 0x09U
#define ST7735S_RDDPM 0x0AU
#define ST7735S_RDDMADCTL 0x0BU
#define ST7735S_RAMRD 0x2EU

// WRITE DISPLAY SETTING ADDRESSES
#define ST7735S_CASET 0x2AU
#define ST7735S_RASET 0x2BU
#define ST7735S_RAMWR 0x2CU

/*
 * @brief struct for storing st7735s device settings
 */
typedef struct {
  // SPI Config
  SPI_TypeDef *spi_regs;
  spi_config_t spi_settings;

  // GPIO Config
  GPIO_TypeDef *dc_port;
  uint8_t dc_pin;
  GPIO_TypeDef *reset_port;
  uint8_t reset_pin;
  GPIO_TypeDef *cs_port;
  uint8_t cs_pin;

  // Display Config
  uint8_t colour_mode;
  uint8_t madctl_val;
} st7735s_dev_t;

/*
 * @brief Write function to send commands to display
 */
void st7735s_write_cmd(st7735s_dev_t *dev, uint16_t cmd);

/*
 * @brief Write function to send frame data to display
 */
void st7735s_write_data(st7735s_dev_t *dev, uint16_t data);

/*
 * @brief Initializes communication and display devuration
 */
void st7735s_init(st7735s_dev_t *dev);

/*
 * @brief Read function to receive display data (may not need)
 */
uint8_t st7735s_read();
#endif