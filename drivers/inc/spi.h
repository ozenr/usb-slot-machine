// External Libraries
#include <stdint.h>
#include <stm32f4xx.h>

/*
 * SPI Configuration Settings Struct
 */
typedef struct {
  uint32_t mode;
  uint32_t direction;
  uint32_t data_size;
  uint32_t CPOL;
  uint32_t CPHA;
  uint32_t baud_rate;
} spi_config_t;

/*
 * @brief Initializes SPI communication with given registers and settings
 */
void spi_init(SPI_TypeDef *spi_registers, spi_config_t *spi_settings);

/*
 * @brief Sends over data one byte at a time into SPI queue
 */
void spi_write(SPI_TypeDef *spi_registers, uint8_t data);

/*
 * @brief Returns data read from SPI queue
 */
uint8_t spi_read(SPI_TypeDef *spi_registers);