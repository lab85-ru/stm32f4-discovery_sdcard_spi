#ifndef GPIO_H_
#define GPIO_H_

/*---------------------------------- STM32F407xx/STM32F417xx------------------*/
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping                       */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping            */
#define GPIO_AF0_TAMPER        ((uint8_t)0x00)  /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping             */
#define GPIO_AF0_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping                          */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */

/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF2_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping  */
#define GPIO_AF3_TIM9          ((uint8_t)0x03)  /* TIM9 Alternate Function mapping  */
#define GPIO_AF3_TIM10         ((uint8_t)0x03)  /* TIM10 Alternate Function mapping */
#define GPIO_AF3_TIM11         ((uint8_t)0x03)  /* TIM11 Alternate Function mapping */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
#define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping */
#define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping        */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2/I2S2 Alternate Function mapping   */
#define GPIO_AF5_I2S3ext       ((uint8_t)0x05)  /* I2S3ext_SD Alternate Function mapping  */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_SPI3          ((uint8_t)0x06)  /* SPI3/I2S3 Alternate Function mapping  */
#define GPIO_AF6_I2S2ext       ((uint8_t)0x06)  /* I2S2ext_SD Alternate Function mapping */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping     */
#define GPIO_AF7_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping     */
#define GPIO_AF7_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping     */
#define GPIO_AF7_I2S3ext       ((uint8_t)0x07)  /* I2S3ext_SD Alternate Function mapping */

/** 
  * @brief   AF 8 selection  
  */ 
#define GPIO_AF8_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping  */
#define GPIO_AF8_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping  */
#define GPIO_AF8_USART6        ((uint8_t)0x08)  /* USART6 Alternate Function mapping */

/** 
  * @brief   AF 9 selection 
  */ 
#define GPIO_AF9_CAN1          ((uint8_t)0x09)  /* CAN1 Alternate Function mapping  */
#define GPIO_AF9_CAN2          ((uint8_t)0x09)  /* CAN2 Alternate Function mapping  */
#define GPIO_AF9_TIM12         ((uint8_t)0x09)  /* TIM12 Alternate Function mapping */
#define GPIO_AF9_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping */
#define GPIO_AF9_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping */

/** 
  * @brief   AF 10 selection  
  */ 
#define GPIO_AF10_OTG_FS        ((uint8_t)0x0A)  /* OTG_FS Alternate Function mapping */
#define GPIO_AF10_OTG_HS        ((uint8_t)0x0A)  /* OTG_HS Alternate Function mapping */

/** 
  * @brief   AF 11 selection  
  */ 
#define GPIO_AF11_ETH           ((uint8_t)0x0B)  /* ETHERNET Alternate Function mapping */

/** 
  * @brief   AF 12 selection  
  */ 
#define GPIO_AF12_FSMC          ((uint8_t)0x0C)  /* FSMC Alternate Function mapping                     */
#define GPIO_AF12_OTG_HS_FS     ((uint8_t)0x0C)  /* OTG HS configured in FS, Alternate Function mapping */
#define GPIO_AF12_SDIO          ((uint8_t)0x0C)  /* SDIO Alternate Function mapping                     */

/** 
  * @brief   AF 13 selection  
  */ 
#define GPIO_AF13_DCMI          ((uint8_t)0x0D)  /* DCMI Alternate Function mapping */

/** 
  * @brief   AF 15 selection  
  */ 
#define GPIO_AF15_EVENTOUT      ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */
/*----------------------------------------------------------------------------*/




typedef enum {
    GPIO_MODE_IN  = 0,        // 00:Input mode (reset state)
    GPIO_MODE_OUT = 1,        // 01: General purpose output mode
    GPIO_MODE_AF  = 2,        // 10: Alternate function mode
    GPIO_MODE_AN  = 3,        // 11: Analog mode
} gpio_mode_t;

typedef enum {
    GPIO_OTYPE_PUSHPULL  = 0, // 0: Output push-pull (reset state)
    GPIO_OTYPE_OPENDRAIN = 1, // 1: Output open-drain
} gpio_otype_t;

typedef enum {
    GPIO_SPEEDF_LOW      = 0, // 00: Low speed
    GPIO_SPEEDF_MED      = 1, // 01: Medium speed
    GPIO_SPEEDF_HIGH     = 2, // 10: High speed
    GPIO_SPEEDF_VERYHIGH = 3, // 11: Very high speed
} gpio_speed_t;

typedef enum {
    GPIO_PULL_NO   = 0, // 00: No pull-up, pull-down
    GPIO_PULL_UP   = 1, // 01: Pull-up
    GPIO_PULL_DOWN = 2, // 10: Pull-down
                        // 11: Reserved
} gpio_pull_t;

void gpio_init(GPIO_TypeDef *port, uint16_t pin_bit, gpio_mode_t mode, gpio_otype_t otype, gpio_speed_t speed, gpio_pull_t pull, uint8_t af);
void gpio_set_0(GPIO_TypeDef *port, uint16_t pin_bit);
void gpio_set_1(GPIO_TypeDef *port, uint16_t pin_bit);
uint32_t gpio_get(GPIO_TypeDef *port, uint16_t pin_bit);

#endif