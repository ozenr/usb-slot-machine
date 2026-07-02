// Internal Libraries
#include "spi.h"

void spi_init(SPI_TypeDef *spi_registers, spi_config_t *spi_settings);

void spi_write(SPI_TypeDef *spi_registers, uint8_t data);

uint8_t spi_read(SPI_TypeDef *spi_registers);