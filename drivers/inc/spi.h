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
 * @brief Enum to store data frame format of SPI (8 bit vs 16 bit)
 */
typedef enum {
  SPI_8_BIT = 0x00000000U,
  SPI_16_BIT = SPI_CR1_DFF
} spi_data_size_t;

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
 * @brief Enum to store different CPOL/CPHA configurations
 */
typedef enum {
  SPI_CLOCK_MODE_0 = (0 << 0) | (0 << 1),
  SPI_CLOCK_MODE_1 = (0 << 0) | SPI_CR1_CPHA,
  SPI_CLOCK_MODE_2 = SPI_CR1_CPOL | (0 << 1),
  SPI_CLOCK_MODE_3 = SPI_CR1_CPOL | SPI_CR1_CPHA
} spi_clock_mode_t;

/*
 * @brief Enum to store different baud_rate configurations
 */
typedef enum {
  SPI_BAUD_RATE_DIV2 = 0x00000000U,
  SPI_BAUD_RATE_DIV4 = SPI_CR1_BR_0,
  SPI_BAUD_RATE_DIV8 = SPI_CR1_BR_1,
  SPI_BAUD_RATE_DIV16 = SPI_CR1_BR_1 | SPI_CR1_BR_0,
  SPI_BAUD_RATE_DIV32 = SPI_CR1_BR_2,
  SPI_BAUD_RATE_DIV64 = SPI_CR1_BR_2 | SPI_CR1_BR_0,
  SPI_BAUD_RATE_DIV128 = SPI_CR1_BR_2 | SPI_CR1_BR_1,
  SPI_BAUD_RATE_DIV256 = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0
} spi_baud_rate_t;

/*
 * @brief SPI Configuration Settings Struct
 * @details mode holds the different SPI mode (MASTER vs SLAVE Mode)
 *          data_size refers to size of data being sent (8 vs 16-bit)
 *          direction refers to how data is transferred (FULL vs HALF-Duplex)
 *          clock_config holds the settings for clock configuration
 */
typedef struct {
  spi_mode_t mode;
  spi_data_size_t data_size;
  spi_direction_t direction;
  spi_clock_mode_t clock_mode;
  spi_baud_rate_t baud_rate;
} spi_config_t;

/*
 * @brief Initializes SPI communication with given registers and settings
 * @param spi_regs holds the registers for specific spi version
 */
void spi_init(SPI_TypeDef *spi_regs, spi_config_t *spi_settings);

/*
 * @brief Sends over data into SPI TX buffer
 */
void spi_write(SPI_TypeDef *spi_regs, uint8_t data);

/*
 * @brief Returns data read from SPI RX buffer
 */
uint8_t spi_read(SPI_TypeDef *spi_regs);
#endif
