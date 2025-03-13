#ifndef BOARD_H
#define BOARD_H

//------------------------------------------------------------------------------
#if HSI == 1

#define HSI_CLK_HZ  (16000000UL)
      
#define SYSCLK_HZ   (HSI_CLK_HZ)
#define HCLK        (SYSCLK_HZ)
#define APB1_CLK_HZ (HCLK)
#define APB2_CLK_HZ (HCLK)

#endif

#if HSE == 1

#define HSE_CLK_HZ  (8000000UL)

#define PLLM        (8U)
#define PLLN        (336U)
#define PLLP        (0U) // 00-> /2
//00: PLLP = 2
//01: PLLP = 4
//10: PLLP = 6
//11: PLLP = 8
      
#define SYSCLK_HZ   (HSE_CLK_HZ / PLLM * PLLN / ((PLLP + 1) * 2))

//#define AHBPRE (0U) not divided SYSCLK_HZ -> HCLK
#define HCLK        (SYSCLK_HZ)

#define APB1_CLK_HZ (HCLK / 4)
#define APB2_CLK_HZ (HCLK / 2)

#endif

#if HSI==0 && HSE==0
#error "Define: HSI=1 or HSE=1"
#endif

// MCO1 EXT CLK out for test - 168 MHz
#define MCO1_PORT           (GPIOA)
#define MCO1_PIN            (1 << 8)


//------------------------------------------------------------------------------

#define UART2_CONSOLE_SPEED  (115200UL)

// USART2 - Console
#define USART2_TX_PORT     (GPIOA) 
#define USART2_TX          (1 << 2)
#define USART2_RX_PORT     (GPIOA) 
#define USART2_RX          (1 << 3)

// SPI2 - SDCARD
#define SPI2_CLK_PORT      (GPIOB) 
#define SPI2_CLK           (1 << 13)
#define SPI2_MOSI_PORT     (GPIOB) 
#define SPI2_MOSI          (1 << 15)
#define SPI2_MISO_PORT     (GPIOB) 
#define SPI2_MISO          (1 << 14)
#define SPI2_CS_PORT       (GPIOB) 
#define SPI2_CS_L          (1 << 12)


/*
// LED STATUS R
#define LED_R_PORT  (GPIOC)
#define LED_R       (1 << 3)

// LED STATUS G
#define LED_G_PORT  (GPIOC)
#define LED_G       (1 << 2)
*/






#endif
