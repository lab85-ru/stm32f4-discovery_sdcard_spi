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

//------------------------------------------------------------------------------
// Запуск внешнего кварцевого резонатора
// Возврат:
// = 0 -> Ок
// < 0 -> Ошибка
//------------------------------------------------------------------------------
int rcc_hse_init(void)
{
#if HSI == 1
    return 0;
#endif    
    
#if HSE == 1    
    uint32_t res;
    uint32_t cnt = 0;

    if (RCC->CR & RCC_CR_HSERDY) {
        // Генератор запущен значит Загрузчик его запустил и все настроил-выходим
        return 0;
    }
    
    // Включаем HSE
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);
    
    // Ждем когда запуститься генератор
    do {
        res = RCC->CR & RCC_CR_HSERDY;
        cnt ++;
    } while (res == 0 && cnt != HSE_STARTUP_TIME_OUT);
    
    if (res == 0) return -1; // Ошибка, генератор не запустился !

    //--------------------------------------------------    
    // HSE  запустился, дальше настраиваем Раздачу часов

    /*!< Prefetch Buffer Enable */
    /*!< LATENCY bit (Latency) */
    //FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_4WS; // 4WS -> for 150MHz
    FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS; // 5 WS (6 CPU cycles) 150 < HCLK ? 168

    //--------------------------------------------------    
    // Настраиваем и запускаем PLL
    
    /* (6) Set the PLL multiplier to  */
    // MUX: HSE(8 MHz) -> PLL
    // HSE(8Mhz)  / 8 * 336 / 2 = 168 MHz
    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE \
                | (PLLM << RCC_PLLCFGR_PLLM_Pos) \
                | (PLLN << RCC_PLLCFGR_PLLN_Pos) \
                | (PLLP << RCC_PLLCFGR_PLLP_Pos);
   
    
    /* (7) Enable the PLL */
    RCC->CR |= RCC_CR_PLLON; /* (7) */
    
    cnt = 0;
    while((RCC->CR & RCC_CR_PLLRDY) == 0) { /* (8) */
        cnt++;
        if (cnt == PLL_STARTUP_TIME_OUT) return -1; // Ошибка, PLL не запустился
    }
    
    // APB1PRE = /4, APB2PRE = /2
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
        
    /* (9) Select PLL as system clock */
    RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); /* (9) */
    
    /* (10) Wait until the PLL is switched on */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {  /* (10) */
        /* For robust implementation, add here time-out management */
    }
    
    //--------------------------------------------------    
    // MCO1 for test
    
    rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);  // GPIO CLK EN
  
    // gpio_init
    gpio_init(MCO1_PORT, MCO1_PIN, GPIO_MODE_AF, GPIO_OTYPE_PUSHPULL, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_UP, GPIO_AF0_MCO);
    
    // MUX PLL->MCO1 168 / 3 = 56 MHz
    RCC->CFGR |= RCC_CFGR_MCO1_0 | RCC_CFGR_MCO1_1 | RCC_CFGR_MCO1PRE_2 | RCC_CFGR_MCO1PRE_0;
    
    SystemCoreClock = HCLK;

    return 0;
#endif
}
