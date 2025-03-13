#include <stdint.h>
#include "time_hal.h"
#include "sys_timer.h"
#include "printf_hal.h"


#ifndef DEBUG_TIME
#define DEBUG_TIME 0
#endif


//******************************************************************************
// Delay ms
//******************************************************************************
void delay_ms( const uint32_t delay )
{
    uint32_t c;
  
    //if (get_os_status()) {
    //    vTaskDelay(delay);
    //} else {
        c = time_get_ms_counter();
        c = c + delay;
        while(time_get_ms_counter() < c);
    //}
}

//******************************************************************************
// Delay sec
//******************************************************************************
void delay_s( const uint32_t delay )
{
    uint32_t c;
    
    //if (get_os_status()) {
    //    vTaskDelay(delay * 1000);
    //} else {
        c = time_get_ms_counter();
        c = c + (delay * 1000);
        while(time_get_ms_counter() < c);
    //}
}

//******************************************************************************
// Poluchenie Statusa - skolko vremeni prohlo s poslednego sobitiya.
// Status vichislaetcha kak raznost dT = (T tekushee)-(T sobitiya(prohloe))
// dalee if dT >= INTERVAL then return TIR_OUT else return TIR_WAIT
// Time = sec,ms & etc
//
// t_start - vrema nachala otchuta
// t_delay - interval 
// t_cur   - tekushee vrema
//
//******************************************************************************
tir_e calcul_time_out( const uint32_t t_cur, const uint32_t t_start, const uint32_t t_delay )
{
	uint32_t t_delta;
	
	if (DEBUG_TIME) printf("Time start = %lu\n", t_start);
	if (DEBUG_TIME) printf("time cur   = %lu\n", t_cur);
	if (t_cur >= t_start){
		t_delta = t_cur - t_start;
	}else{
	        t_delta = (uint32_t)0xffffffffUL - t_start + t_cur;
	}
	if (DEBUG_TIME) printf("\nTIME DELTA=%lu   time interval =%lu\n", t_delta, t_delay);
	if (t_delta >= t_delay)
		return TIR_OUT;
	else
		return TIR_WAIT;
}
