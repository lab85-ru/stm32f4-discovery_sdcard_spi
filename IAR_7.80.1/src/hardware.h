#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "stm32f407xx.h"

void periphery_disable(void);
void hardware_init (void);
void uart2_console_init(void);
void uart2_console_rx_int(void);
void run_reboot(void);
void spi2_cs(uint8_t d);

#endif