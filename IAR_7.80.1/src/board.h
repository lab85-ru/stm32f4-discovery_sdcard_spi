#ifndef BOARD_H
#define BOARD_H

//------------------------------------------------------------------------------
#define HSI_CLK_HZ  (16000000UL)
      
      
#define SYSCLK_HZ   (HSI_CLK_HZ)
#define HCLK        (SYSCLK_HZ)
#define APB1_CLK_HZ (HCLK)
#define APB2_CLK_HZ (HCLK)
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
