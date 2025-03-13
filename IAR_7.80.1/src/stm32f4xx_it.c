/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include "dma_407.h"      
#include "hardware.h"
#include "sys_timer.h"
#include "global_var.h"
      
/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
	while (1);
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

extern void disk_timerproc (void);

void SysTick_Handler(void)
{
    sys_tick_handler();
    disk_timerproc();
}

//------------------------------------------------------------------------------
// USART2 console int
//------------------------------------------------------------------------------
void USART2_IRQHandler(void)
{
    uart2_console_rx_int();
}

//------------------------------------------------------------------------------
// DMA - USART2 передача - в consol
// Конец передачи
//------------------------------------------------------------------------------
extern void consol_tx_data( void );
void DMA1_Stream6_IRQHandler(void)
{
    if (DMA1->HISR & DMA_HISR_TCIF6) {
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6;
        pop_data_dma_queue( &q_consol_tx, consol_tx_size);
        flag_consol_tx = 0;
        if (q_consol_tx.in == q_consol_tx.out) return;
        consol_tx_data();
    }
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
}

/**
  * @brief This function handles Ethernet global interrupt.
  */
void ETH_IRQHandler(void)
{
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
