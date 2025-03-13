#include <stdint.h>
#include "printf_hal.h"

extern void tx_byte(uint8_t ch);

//------------------------------------------------------------------------------
// Захватить семафор печати - консоль 
//------------------------------------------------------------------------------
void ConPrintSemTake( void )
{
//    while(xSemaphoreTake(xUartConsolOSsem, portMAX_DELAY) == pdFALSE);
}

//------------------------------------------------------------------------------
// Освободить семафор печати - консоль 
//------------------------------------------------------------------------------
void ConPrintSemGive( void )
{
//    xSemaphoreGive(xUartConsolOSsem);
}

//==============================================================================
// Print while(1) STRING to debug console
//==============================================================================
void PRINT_FATAL_ERROR(const char *ch)
{
	if (ch == 0) {
		while(1){
			printf_d("PRINT_FATAL_ERROR: ERROR string is NULL\n");
		}
	}

	while(1) {
		printf_d("%s\n", ch);
	}
}
