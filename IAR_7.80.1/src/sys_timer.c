#include "main.h"
#include <stdint.h>
#include "sys_timer.h"
#include "rcc_407.h"

static volatile uint32_t sys_tick_counter    = 0;  // ������� ���������� �������
static volatile uint32_t tick_sec_counter    = 0;  // ������� ������
static volatile uint32_t tick_1000ms_counter = 0;  // ������� 1000�� ����������


//#define F_CPU 		(8000000UL)	    // �������� ���
#define TimerTick  	(SYSCLK_HZ / 1000 - 1)	// ��� ����� ��������

//------------------------------------------------------------------------------
// ��������� �������
// T period = 1 ms
//------------------------------------------------------------------------------
void sys_timer_init(void)
{
    SysTick_Config( TimerTick );
    /*Configure the SysTick IRQ priority */
    NVIC_SetPriority(SysTick_IRQn, 0U);
    /*Enable SysTick IRQ */
    NVIC_EnableIRQ(SysTick_IRQn);
}

//------------------------------------------------------------------------------
// ��������������� �������� ������� ������
//------------------------------------------------------------------------------
void time_set_sec_counter( uint32_t t )
{
	tick_sec_counter = t;
}

//------------------------------------------------------------------------------
// ���������� ������� �������� ������� ������
//------------------------------------------------------------------------------
uint32_t time_get_sec_counter( void )
{
	return tick_sec_counter;
}


//------------------------------------------------------------------------------
// ���������� ������� �������� ���������� �������, ������� ms
//------------------------------------------------------------------------------
uint32_t time_get_ms_counter( void )
{
	return sys_tick_counter;
}

//------------------------------------------------------------------------------
// ���������� ���������� �� ���������� �������
//------------------------------------------------------------------------------
void sys_tick_handler( void )
{
    sys_tick_counter++;
    tick_1000ms_counter++;
    if (tick_1000ms_counter == 1000){
        tick_1000ms_counter = 0;
        tick_sec_counter++;       // sec + 1
    }
}
