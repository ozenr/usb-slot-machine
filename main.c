// Internal Libraries
#include "itm.h"
#include "spi.h"
#include "st7735s.h"

// External Libraries
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

// GPIO Port A Declarations
#define LED_PIN 6U
#define RESET_PIN 9U
#define SCK_PIN 5U
#define SDA_PIN 7U

// GPIO Port B
#define A0_PIN 8U
#define CS_PIN 10U

// #define BEAM_PIN (5U)

// Serial Clock
void clock_init();
void delay_ms(uint32_t ms);
volatile uint32_t ticks;

// SPI Config
const spi_config_t spi_settings = {SPI_MASTER_MODE, SPI_8_BIT,
                                   SPI_DIR_FULL_DUPLEX, SPI_CLOCK_MODE_0,
                                   SPI_BAUD_RATE_DIV16};

// Display Config
st7735s_dev_t st7735s_settings = {
    .spi_regs = SPI1, .spi_settings = spi_settings,   
    .dc_port = GPIOB, .dc_pin = A0_PIN, .reset_port = GPIOA,
    .reset_pin = RESET_PIN, .cs_port = GPIOB, .cs_pin = CS_PIN, 
    .colmod = COLMOD_16, .madctl = MADCTL_LANDSCAPE};

void main(void) {
  // Initialize Clock
  clock_init();
  SystemCoreClockUpdate();

  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile uint32_t dummy;
  dummy = RCC->AHB1ENR;
  dummy = RCC->AHB1ENR;

  // Initiate ITM for Debugging
  itm_init();

  // Enable GPIO A and B Clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  // Enable SPI Clock
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  // Configure GPIO for SPI
  GPIOA->MODER &= ~(GPIO_MODER_MODER5_Msk); // clear mode bits first
  GPIOA->MODER &= ~(GPIO_MODER_MODER7_Msk);
  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5 | GPIO_AFRL_AFSEL7); // clear AFRL bits

  // Set to AF Mode
  GPIOA->MODER |= GPIO_MODER_MODER5_1;
  GPIOA->MODER |= GPIO_MODER_MODER7_1;

  // Shift AF5
  GPIOA->AFR[0] |= (5U << GPIO_AFRL_AFSEL5_Pos);
  GPIOA->AFR[0] |= (5U << GPIO_AFRL_AFSEL7_Pos);

  // Configure Other GPIO Pins for Display
  // a0
  GPIOB->MODER &= ~(GPIO_MODER_MODER8_Msk);
  GPIOB->MODER |= GPIO_MODER_MODER8_0;

  // reset
  GPIOA->MODER &= ~(GPIO_MODER_MODER9_Msk);
  GPIOA->MODER |= GPIO_MODER_MODER9_0;

  // cs
  GPIOB->MODER &= ~(GPIO_MODER_MODER10_Msk);
  GPIOB->MODER |= GPIO_MODER_MODER10_0;

  // Initialize SPI & Driver
  spi_init(SPI1, &spi_settings);

  // Start Timer
  SysTick_Config(84000000U / 1000U);
  __enable_irq();

  // st7735s_settings.spi_settings = spi_settings;
  st7735s_init(&st7735s_settings);

  // Configure LED GPIO to Output
  GPIOA->MODER &= ~(GPIO_MODER_MODER6_Msk); // clear bits first
  GPIOA->MODER |= (1 << GPIO_MODER_MODER6_Pos);

  // Configure Sensor GPIO to Input (clear bits)
  GPIOB->MODER &= ~(GPIO_MODER_MODER5_Msk);
  // Superloop
  while (1) {
    // Turn on LED BACKLIGHT
    GPIOA->ODR |= (1 << LED_PIN);
    // uint8_t port_state = (GPIOB->IDR >> BEAM_PIN) & 1;
    // if (port_state) {
    //   // Turn LED ON
    //   GPIOA->ODR &= ~(1 << LED_PIN);
    // } else {
    //   // Turn LED OFF
    //   GPIOA->ODR |= (1 << LED_PIN);
    // }
    delay_ms(500U);
  }
}

void clock_init() {
  // Enable Power Controller and Set Voltage Scale 2 Mode
  // NOTE: By default, the voltage scale is set to 2
  // This is really just for practice
  RCC->APB1ENR |= RCC_APB1ENR_PWREN_Msk;
  volatile uint32_t dummy;
  dummy = RCC->APB1ENR;
  dummy = RCC->APB1ENR;

  // Clear Bits First
  PWR->CR &= ~PWR_CR_VOS_Msk;
  PWR->CR |= (0b10 << PWR_CR_VOS_Pos);

  // Configure Flash Controller
  FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

  // Enable HSE Bypass and Turn it On
  RCC->CR |= RCC_CR_HSEBYP_Msk | RCC_CR_HSEON_Msk;

  // Block the Internal Clock's Control Register (CR) until The External Clock
  // Is Ready
  while (!(RCC->CR & RCC_CR_HSERDY_Msk)) {
  }

  // Clear PLLM, N, AND P
  RCC->PLLCFGR &=
      ~(RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk | RCC_PLLCFGR_PLLP_Msk);

  // Configure PPLM, N, and P for 84 MHz Clock Cycle
  RCC->PLLCFGR |= ((4 << RCC_PLLCFGR_PLLM_Pos) | (168 << RCC_PLLCFGR_PLLN_Pos) |
                   (1 << RCC_PLLCFGR_PLLP_Pos) | (1 << RCC_PLLCFGR_PLLSRC_Pos));

  // Set APB1 Prescaler to 2
  RCC->CFGR |= (0b100 << RCC_CFGR_PPRE1_Pos);

  // Enable PLL
  RCC->CR |= (RCC_CR_PLLON_Msk);
  while (!(RCC->CR & RCC_CR_PLLRDY_Msk)) {
  }

  // Take PLL Output to Use as System Clock
  RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
  while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {
  }
}

// Handler We Initialize Here
void systick_handler() { ticks++; }

/*
 * @brief delays code execution
 */
void delay_ms(uint32_t ms) {
  uint32_t start = ticks;
  uint32_t end = start + ms;

  if (end < start) {
    while (ticks > start) {
    }
  }

  while (ticks < end) {
  }
}