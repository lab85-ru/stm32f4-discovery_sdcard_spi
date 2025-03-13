#ifndef SYS_TIMER_H_
#define SYS_TIMER_H_

#include <stdint.h>

void sys_timer_init(void);
void time_set_sec_counter( uint32_t t );
uint32_t time_get_ms_counter( void );
uint32_t time_get_sec_counter( void );
void sys_tick_handler( void );

#endif