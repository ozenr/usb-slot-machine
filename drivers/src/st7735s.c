// Internal Libraries
#include "st7735s.h"

void st7735s_write_cmd(st7735s_dev_t *dev, uint8_t cmd) {
  // Set CS to LOW
  dev->cs_port->BSRR = (1 << (dev->cs_pin + 16));

  // Set DC to LOW (shifts to Command mode)
  dev->dc_port->BSRR = (1 << (dev->dc_pin + 16));

  // Write command to display
  spi_write(dev->spi_regs, cmd);

  // Shift CS Back to HIGH
  dev->cs_port->BSRR = (1 << dev->cs_pin);
}

void st7735s_write_data(st7735s_dev_t *dev, uint8_t data) {
  if (!dev || !dev->reset_port) {
    return;
  }

  // Set CS to LOW
  dev->cs_port->BSRR = (1 << (dev->cs_pin + 16));

  // Set DC to HIGH (shifts to Data mode)
  dev->dc_port->BSRR = (1 << dev->dc_pin);

  // Write data to display
  spi_write(dev->spi_regs, data);

  // Shifts CS Back to HIGH
  dev->cs_port->BSRR = (1 << dev->cs_pin);
}

void st7735s_set_window(st7735s_dev_t *dev, uint16_t x1, uint16_t x2,
                        uint16_t y1, uint16_t y2) {
  // Check For Valid Display Coordinates
  if (x1 > x2 || y1 > y2) {
    printf("START > END\n");
    return;
  }
  if (x1 >= 128 || x2 >= 128 || y1 >= 128 || y2 >= 128) {
    printf("COORDINATE OUT OF BOUNDS\n");
  }

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
}

void st7735s_write_pixel_data(st7735s_dev_t *dev, uint8_t px) {
  // Set Memory Write
  st7735s_write_cmd(dev, ST7735S_RAMWR);
  st7735s_write_data(dev, )
}

void st7735s_init(st7735s_dev_t *dev) {
  // Pull reset pin LOW then HIGH (HW Reset)
  dev->reset_port->BSRR = (1 << (dev->reset_pin + 16));
  delay_ms(100); // have to pause 100 ms each
  dev->reset_port->BSRR = (1 << dev->reset_pin);
  delay_ms(100);

  // Send SW-RESET Command
  st7735s_write_cmd(dev, ST7735S_SWRESET);
  delay_ms(150);

  // Send SLPOUT Command (display starts off in sleep mode after turn on)
  st7735s_write_cmd(dev, ST7735S_SLPOUT);
  delay_ms(500);

  // Configure COLMOD & MADCTL
  st7735s_write_cmd(dev, ST7735S_COLMOD);
  st7735s_write_data(dev, dev->colmod);

  st7735s_write_cmd(dev, ST7735S_MADCTL);
  st7735s_write_data(dev, dev->madctl);

  // Turn Display ON
  st7735s_write_cmd(dev, ST7735S_DISPON);
  delay_ms(100);
}