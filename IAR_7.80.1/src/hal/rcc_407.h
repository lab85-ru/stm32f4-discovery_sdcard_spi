#ifndef RCC_H_
#define RCC_H_

#include <stdint.h>
#include "stm32f407xx.h"


#define HSE_STARTUP_TIME_OUT (1000000)
#define PLL_STARTUP_TIME_OUT (1000000)

//#define RCC_CLK_EN(x,y)   do { \
//                                        __IO uint32_t tmpreg; \
//                                        (x) |= (y);\
//                                        /* Delay after an RCC peripheral clock enabling */ \
//                                        tmpreg = (x) & (y);\
//                                        UNUSED(tmpreg); \
 //                                     } while(0)

void rcc_clk_en(__IO uint32_t * reg, uint32_t data);
int rcc_hse_init(void);
void rcc_clk_en_for_gpio(GPIO_TypeDef * gpio);



#endif