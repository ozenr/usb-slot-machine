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
  // Set CS to LOW
  dev->cs_port->BSRR = (1 << (dev->cs_pin + 16));

  // Set DC to HIGH (shifts to Data mode)
  dev->dc_port->BSRR = (1 << dev->dc_pin);

  // Write data to display
  spi_write(dev->spi_regs, data);

  // Shifts CS Back to HIGH
  dev->cs_port->BSRR = (1 << dev->cs_pin);
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
  delay(500);

  // Configure COLMOD & MADCTL
  st7735s_write_cmd(dev, ST7735S_COLMOD);
  st7735s_write_data(dev, COLMOD_16_BIT);

  st7735s_write_cmd(dev, ST7735S_MADCTL);
  st7735s_write_data(dev, MADCTL_CFG);

  // Turn Display ON
  st7735s_write_cmd(dev, ST7735S_DISPON);
  delay_ms(100);
}