#include "itm.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

// Pin Declarations
#define LED_PIN (10U)
#define BEAM_PIN (5U)

// Serial Clock
void clock_init();
void delay_ms(uint32_t ms);
volatile uint32_t ticks;

void main(void) {
  // Initialize Clock
  clock_init();
  SystemCoreClockUpdate();

  // Enable ITM
  itm_init();

  // Enable GPIO A and GPIO B
  RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);
  RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOBEN_Pos);

  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile uint32_t dummy;
  dummy = RCC->AHB1ENR;
  dummy = RCC->AHB1ENR;

  SysTick_Config(84000000U / 1000U);
  __enable_irq();

  // Configure LED GPIO to Output
  GPIOA->MODER &= ~(GPIO_MODER_MODER10_Msk); // clear bits first
  GPIOA->MODER |= (1 << GPIO_MODER_MODER10_Pos);

  // Configure Sensor GPIO to Input (clear bits)
  GPIOB->MODER &= ~(GPIO_MODER_MODER5_Msk);

  // Superloop
  while (1) {
    uint8_t port_state = (GPIOB->IDR >> BEAM_PIN) & 1;
    if (port_state) {
      // Turn LED ON
      GPIOA->ODR &= ~(1 << LED_PIN);
    } else {
      // Turn LED OFF
      GPIOA->ODR |= (1 << LED_PIN);
    }
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