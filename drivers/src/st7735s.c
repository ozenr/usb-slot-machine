// Internal Libraries
#include "st7735s.h"

// Functions to Select and Deselect CS
static inline void set_cs_high(st7735s_dev_t *dev) {
  dev->cs_port->BSRR = (1 << dev->cs_pin);
}

static inline void set_cs_low(st7735s_dev_t *dev) {
  dev->cs_port->BSRR = (1 << (dev->cs_pin + 16));
}

void st7735s_write_cmd(st7735s_dev_t *dev, uint8_t cmd) {
  // Set DC to LOW (shifts to Command mode)
  dev->dc_port->BSRR = (1 << (dev->dc_pin + 16));

  // Write command to display
  spi_write(dev->spi_regs, cmd);
}

void st7735s_write_data(st7735s_dev_t *dev, uint8_t data) {
  if (!dev || !dev->reset_port) {
    return;
  }

  // Set DC to HIGH (shifts to Data mode)
  dev->dc_port->BSRR = (1 << dev->dc_pin);

  // Write data to display
  spi_write(dev->spi_regs, data);
}

void st7735s_set_window(st7735s_dev_t *dev, uint16_t x1, uint16_t x2,
                        uint16_t y1, uint16_t y2) {
  // Check For Valid Display Coordinates
  if (x1 > x2 || y1 > y2) {
    return;
  }
  if (x1 >= 128 || x2 >= 128 || y1 >= 128 || y2 >= 128) {
    return;
  }

  // Pull CS Low
  set_cs_low(dev);

  // Set Column Size
  st7735s_write_cmd(dev, ST7735S_CASET);
  st7735s_write_data(dev, (uint8_t)(x1 >> 8)); // split into two different bytes
  st7735s_write_data(dev, (uint8_t)(x1 & 0x00FF));
  st7735s_write_data(dev, (uint8_t)(x2 >> 8));
  st7735s_write_data(dev, (uint8_t)(x2 & 0x00FF));

  // Set Row Size
  st7735s_write_cmd(dev, ST7735S_RASET);
  st7735s_write_data(dev, (uint8_t)(y1 >> 8)); // split into two different bytes
  st7735s_write_data(dev, (uint8_t)(y1 & 0x00FF));
  st7735s_write_data(dev, (uint8_t)(y2 >> 8));
  st7735s_write_data(dev, (uint8_t)(y2 & 0x00FF));

  // Pull cs High
  set_cs_high(dev);
}

void st7735s_clear_window(st7735s_dev_t *dev, const uint16_t *frame) {
  // const uint16_t COLOUR = 0xFFFF;

  // Set Window to Entire Screen
  st7735s_set_window(dev, 0, 126, 0, 126);

  // Set Memory Write
  set_cs_low(dev);
  st7735s_write_cmd(dev, ST7735S_RAMWR);

  for (uint32_t i = 0; i < 128 * 128; ++i) {
    // split colour data
    uint16_t colour = *(frame + i);
    st7735s_write_data(dev, (colour >> 8));
    st7735s_write_data(dev, (colour & 0x00FF));
  }

  set_cs_high(dev);
}

void st7735s_init(st7735s_dev_t *dev) {
  // Pull reset pin LOW then HIGH (HW Reset)
  dev->reset_port->BSRR = (1 << (dev->reset_pin + 16));
  delay_ms(100); // have to pause 100 ms each
  dev->reset_port->BSRR = (1 << dev->reset_pin);
  delay_ms(100);

  // Send SW-RESET Command
  set_cs_low(dev);
  st7735s_write_cmd(dev, ST7735S_SWRESET);
  set_cs_high(dev);
  delay_ms(150);

  // Send SLPOUT Command (display starts off in sleep mode after turn on)
  set_cs_low(dev);
  st7735s_write_cmd(dev, ST7735S_SLPOUT);
  set_cs_high(dev);
  delay_ms(500);

  // Configure COLMOD & MADCTL
  set_cs_low(dev);
  st7735s_write_cmd(dev, ST7735S_COLMOD);
  st7735s_write_data(dev, dev->colmod);

  // set_cs_low(dev);
  st7735s_write_cmd(dev, ST7735S_MADCTL);
  st7735s_write_data(dev, dev->madctl);
  set_cs_high(dev);
  delay_ms(500);

  // Turn Display ON
  set_cs_low(dev);
  st7735s_write_cmd(dev, ST7735S_DISPON);
  set_cs_high(dev);
  delay_ms(100);
}