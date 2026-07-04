#include "st7735s.h"

void st7735s_write_cmd(suint8_t cmd) { 
  spi_write();
}

void st7735s_init(st7735s_config_t *config) {
  // Initialize SPI
  spi_init(config->spi_regs, &(config->spi_settings));

  // Set Colour Mode
}