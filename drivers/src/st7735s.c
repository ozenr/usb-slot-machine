#include "st7735s.h"

void st7735s_write_cmd(st7735s_dev_t *dev, uint8_t cmd) {
  // Set CS to Low
  dev->cs_port->BSRR = (1 << (dev->cs_pin + 16));

  // Set DC to Low (shifts to Command mode)
  dev->dc_port->BSRR = (1 << (dev->dc_pin + 16));

  // Write command to display
  spi_write(dev->spi_regs, cmd);

  // Shift CS Back to High
  dev->cs_port->BSRR = (1 << dev->cs_pin);
}

void st7735s_write_data(st7735s_dev_t *dev, uint8_t data) {
  // Set CS to LOW
  dev->cs_port->BSRR = (1 << (dev->cs_pin + 16));

  // Set DC to HIGH (shifts to Data mode)
  dev->dc_port->BSRR = (1 << dev->dc_pin);

  // Write data to display
  spi_write(dev->spi_regs, data);

  // Shifts CS Back to high
  dev->cs_port->BSRR = (1 << dev->cs_pin);
}

void st7735s_init(st7735s_dev_t *dev) {
  // Initialize SPI
  spi_init(config->spi_regs, &(config->spi_settings));

  // Pull reset pin LOW

  // Set Colour Mode
}