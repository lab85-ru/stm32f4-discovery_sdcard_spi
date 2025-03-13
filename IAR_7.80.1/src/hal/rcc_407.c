#include <stdint.h>
#include "main.h"
#include "stm32f407xx.h"
#include "rcc_407.h"
#include "gpio_407.h"

uint32_t SystemCoreClock;

//------------------------------------------------------------------------------
// Включение тактового сигнала переферии
//------------------------------------------------------------------------------
void rcc_clk_en(__IO uint32_t * reg, uint32_t data)
{
    volatile uint32_t tmp;
    
    *reg |= data;
    tmp = *reg & data;
}

//------------------------------------------------------------------------------
// Включение тактового сигнала для GPIO
//------------------------------------------------------------------------------
void rcc_clk_en_for_gpio(GPIO_TypeDef *gpio)
{
    uint32_t pio = (uint32_t)gpio;
        
    switch (pio) {
    case (uint32_t)GPIOA: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); return;
    case (uint32_t)GPIOB: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); return;
    case (uint32_t)GPIOC: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); return;
    case (uint32_t)GPIOD: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); return;
    case (uint32_t)GPIOE: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN); return;
    case (uint32_t)GPIOF: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN); return;
    case (uint32_t)GPIOG: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN); return;
    case (uint32_t)GPIOH: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN); return;
    case (uint32_t)GPIOI: rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN); return;
    }
}
