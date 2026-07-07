#ifndef ST7735S_H
#define ST7735S_H

// Internal Libraries
#include "itm.h"
#include "spi.h"

// External Libraries
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

// Delay_Ms Function from main
extern volatile uint32_t ticks;
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
#define ST7735S_CASET 0x2AU // colummn address set
#define ST7735S_RASET 0x2BU // row addresset set
#define ST7735S_RAMWR 0x2CU

// MAX BOUNDS FOR WINDOW RESOLUTION
#define ROW_Y_MAX 0x7F
#define COL_X_MAX 0x7F

/*
 * @brief Enum for MADCTL Configuration
 */
typedef enum { MADCTL_PORTRAIT = 0x00, MADCTL_LANDSCAPE = 0x60 } madctl_cfg_t;

/*
 * @brief Enum for COLMOD Configuration
 */
typedef enum {
  COLMOD_12 = 0x03,
  COLMOD_16 = 0x05,
  COLMOD_18 = 0x06
} colmod_cfg_t;

/*
 * @brief struct for storing st7735s device settings
 */
typedef struct {
  // SPI Config
  SPI_TypeDef *spi_regs;
  const spi_config_t spi_settings;

  // GPIO Config
  GPIO_TypeDef *dc_port;
  uint8_t dc_pin;
  GPIO_TypeDef *reset_port;
  uint8_t reset_pin;
  GPIO_TypeDef *cs_port;
  uint8_t cs_pin;

  // Display Config
  colmod_cfg_t colmod;
  madctl_cfg_t madctl;
} st7735s_dev_t;

/*
 * @brief Write function to send commands to lcd controller
 */
void st7735s_write_cmd(st7735s_dev_t *dev, uint8_t cmd);

/*
 * @brief Write function to send data to lcd controller
 */
void st7735s_write_data(st7735s_dev_t *dev, uint8_t data);

/*
 * @brief Function to for mapping display window on the LCD
 * Requires: x1 <= x2, y1 <= y2, all of x and y < ROW_Y_MAX and COL_X_MAX
 *           respectively
 *
 */
void st7735s_set_window(st7735s_dev_t *dev, uin16_t x1, uint16_t x2,
                        uint16_t y1, uint16_t y2);

/*
 * @brief Write function to write pixel data onto physical display
 * Requires: st7735s_set_window() is called first
 */
void st7735s_write_pixel_data(st7735s_dev_t *dev, uint8_t px);

/*
 * @brief Initializes communication and display
 */
void st7735s_init(st7735s_dev_t *dev);

/*
 * @brief Read function to receive display data (may not need)
 */
// uint8_t st7735s_read(st7735s_dev_t *dev);
#endif