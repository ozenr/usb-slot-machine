// Internal Libraries
#include "spi.h"

void spi_init(SPI_TypeDef *spi_regs, spi_config_t *spi_settings) {
  // Enable SPI on Peripheral Bus
  if (spi_regs == SPI1) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  } else if (spi_regs == SPI2) {
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  } else if (spi_regs == SPI3) {
    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
  } else if (spi_regs == SPI4) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
  }

  // Clear Control Register 1
  spi_regs->CR1 = 0;

  // Configure Based on Spi Settings
  spi_regs->CR1 |= spi_settings->mode;
  spi_regs->CR1 |= spi_settings->direction;
  spi_regs->CR1 |= spi_settings->data_size;
  spi_regs->CR1 |= spi_settings->clock_mode;
  spi_regs->CR1 |= spi_settings->baud_rate;

  // Start SPI Communication
  spi_regs->CR1 |= SPI_CR1_SPE;
}

void spi_write(SPI_TypeDef *spi_regs, uint8_t data) {
  // Check For Bidirectional Mode
  if (spi_regs->CR1 & SPI_DIR_HALF_DUPLEX) {
    spi_regs->CR1 |= SPI_CR1_BIDIOE;
  }

  // Wait for Transmit Buffer to Be Empty
  while (!(spi_regs->SR & SPI_SR_TXE)) {
  }

  // Load Data into TX Buffer (Data Register)
  spi_regs->DR = data;

  // Wait For Data to Finish Transmitting Before Finishing
  while (spi_regs->SR & SPI_SR_BSY) {
  }
}

uint16_t spi_read(SPI_TypeDef *spi_regs) {
  // Send Out Dummy Data for Spi Exchange
  spi_write(spi_regs, 0x00);

  uint8_t data;
  // Check For Bidirectional Mode
  if (spi_regs->CR1 & SPI_DIR_HALF_DUPLEX) {
    spi_regs->CR1 &= ~(1 << SPI_CR1_BIDIOE);
  }

  // Wait For RX Buffer to be Not Empty
  while (!(spi_regs->SR & SPI_SR_RXNE)) {
  }

  // Read Data from RX Buffer
  data = spi_regs->DR;

  return data;
}