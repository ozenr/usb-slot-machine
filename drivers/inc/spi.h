#ifndef SPI_H
#define SPI_H

// External Libraries
#include <stdint.h>
#include <stm32f4xx.h>

/*
 * @brief Enum to store different SPI Modes
 */
typedef enum {
  SPI_SLAVE_MODE = 0x00000000U,
  SPI_MASTER_MODE = SPI_CR1_MSTR
} spi_mode_t;

/*
 * @brief Enum to store different directions (i.e. FULL-DUPLEX, HALF-DUPLEX,
 * RX-ONLY)
 */
typedef enum {
  SPI_DIR_FULL_DUPLEX = 0x00000000U,
  SPI_DIR_HALF_DUPLEX = SPI_CR1_BIDIMODE,
  SPI_DIR_RXONLY = SPI_CR1_RXONLY
} spi_direction_t;

/*
 * @brief Struct to store SPI clock configuration
 */
typedef struct {
  uint32_t cpol;
  uint32_t cpha;
  uint32_t baud_rate;
} spi_clock_config_t;

/*
 * SPI Configuration Settings Struct
 */
typedef struct {
  spi_mode_t mode;
  uint32_t data_size;
  spi_direction_t direction;
  spi_clock_config_t clock_config;
} spi_config_t;

/*
 * @brief Initializes SPI communication with given registers and settings
 */
void spi_init(SPI_TypeDef *spi_regs, spi_config_t *spi_settings);

/*
 * @brief Sends over data into SPI TX buffer
 */
void spi_write(SPI_TypeDef *spi_regs, uint16_t data);

/*
 * @brief Returns data read from SPI RX buffer
 */
uint16_t spi_read(SPI_TypeDef *spi_regs);
#endif
