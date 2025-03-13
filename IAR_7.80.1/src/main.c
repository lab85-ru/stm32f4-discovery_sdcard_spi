//------------------------------------------------------------------------------
// Stm32f4-Discovery - STM32F407
// SPI 8 bit - SDCARD FatFs test.
//
// HSI = 16MHz (all hard modules -> 16 MHz)
// fask spi clk = 8 MHz
// write file size = 15Mb t_start = 383, t_stop = 133839 Speed write = 118260 b/s
// read file size = 15801700, t_start = 487, t_stop = 77619 Speed read = 205216 b/s
//
// HSE = 8MHz (CORE=168 MHz APB1=42MHz)
// fask spi clk = 42/2=21 MHz
// write file size = 10Mb, t_start = 200, t_stop = 64949 Speed write = 163840 b/s 
// read file size = 10Mb, t_start = 64974, t_stop = 90487 Speed read = 419430 b/s
//------------------------------------------------------------------------------
#include "main.h"
#include "stm32f407xx.h"
#include "board.h"
#include <stdint.h>
#include "gpio_407.h"
#include "rcc_407.h"
#include "dma_407.h"
#include "i2c_407.h"
#include "sys_timer.h"
#include "queue_buf.h"
#include "hardware.h"
#include "time_hal.h"
#include "git_commit.h"
#include "ff.h"

#include <time.h>
#include <string.h>

#include "printf_hal.h"

#ifndef DEBUG
#define DEBUG 0
#endif

const char txt_device_ver_soft[] = {"SV:"};
const char txt_device_ver_hard[] = {"HV:stm32f4-discovery"};
const char txt_device_name[]     = {"SPI sdcard test"};

uint8_t console_tx_char(uint8_t ch);

FATFS FatFs;				/* File system object for each logical drive */
#define BLOCK_SIZE (512)
uint8_t file_io_buf[ BLOCK_SIZE ];

volatile UINT Timer;

/*---------------------------------------------------------*/
/* User provided RTC function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called back     */
/* from FatFs module.                                      */

#if !FF_FS_NORTC && !FF_FS_READONLY
DWORD get_fattime (void)
{
	RTCTIME rtc;

	/* Get local time */
	if (!rtc_gettime(&rtc)) return 0;

	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
}
#endif

//------------------------------------------------------------------------------
// Console TX buffer
#define QUEUE_CONSOL_TX_BUF_SIZE (1024)

// flag console transfer: tx=1 transfer data, tx=0 not transfer data
volatile uint8_t flag_consol_tx = 0;

// –азмер передаваемых данных по DMA (нужно дл€ перемещени€ указател€ в 
// прерывании после оконцани€ передачи)
volatile uint32_t consol_tx_size = 0; 

// buffer console tx
uint8_t consol_tx_buf[ QUEUE_CONSOL_TX_BUF_SIZE ];

queue_buf_t q_consol_tx = {
    .queue = consol_tx_buf,
    .len   = QUEUE_CONSOL_TX_BUF_SIZE,
};

// queue console RX
uint8_t consol_rx_buf[ QUEUE_CONSOL_RX_SIZE ];
queue_buf_t q_consol_rx = {
    .queue = consol_rx_buf,
    .len   = QUEUE_CONSOL_RX_SIZE,
};

//******************************************************************************
// Consol передача массива по DMA
//******************************************************************************
void consol_tx_data( void )
{
    uint8_t *adr_start = 0;

    if (DMA1_Stream6->CR & 1) return; 
    if (flag_consol_tx) return;

    if (q_consol_tx.in == q_consol_tx.out) return;

    adr_start = &consol_tx_buf[ q_consol_tx.out ];
    
    if ( q_consol_tx.in > q_consol_tx.out) 
        consol_tx_size = q_consol_tx.in - q_consol_tx.out;
    else 
        consol_tx_size = q_consol_tx.len - q_consol_tx.out;

    flag_consol_tx = 1;  // flag - tx data enable
    dma_set_ram(DMA1_Stream6, (uint32_t)adr_start, consol_tx_size);
    dma_on(DMA1_Stream6);
}
      
//******************************************************************************
// Tx byte to Console uart
//******************************************************************************
void tx_byte(uint8_t ch)
{
    // Get free space to TX buffer
    while( get_free_size_queue( &q_consol_tx ) <= 1) {
        consol_tx_data();
    }
    
    push_data_queue_b( &q_consol_tx, ch);
    
    if (ch == '\n' || ch == '\r') consol_tx_data(); // end string -> start TX
}


/** ----------------------------------------------------------------------------
  * @brief  Setup the microcontroller system.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* NOTE :SystemInit(): This function is called at startup just after reset and 
                         before branch to main program. This call is made inside
                         the "startup_stm32f0xx.s" file.
                         User can setups the default system clock (System clock source, PLL Multiplier
                         and Divider factors, AHB/APBx prescalers and Flash settings).
   */
}

//******************************************************************************
// Print table files in working directory
//******************************************************************************
FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    static FILINFO fno;

    res = f_opendir(&dir, path);                           /* Open the directory */
    printf_d("PATH = %s\n", path);
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                printf_d(" DIR  %s\n", fno.fname);
            } else {                                       /* It is a file. */
                printf_d(" FILE %s  %ld\n", fno.fname, fno.fsize);
            }
        }
        f_closedir(&dir);
    }

    return res;
}

//------------------------------------------------------------------------------
// MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
//------------------------------------------------------------------------------
int main(void)
{
    rcc_clk_en(&RCC->APB1ENR, RCC_APB1ENR_PWREN);
    rcc_clk_en(&RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

#if HSE == 1
    rcc_clk_en(&PWR->CR, PWR_CR_VOS); // VOS=1 for clk 144-168 MHZ
    int res = rcc_hse_init();
#endif
    
    sys_timer_init();
    hardware_init();
    xdev_out(tx_byte);           // настройка указател€ на функцию вывода (дл€ xprintf.c)

    printf_d("Device start.\n");    
    printf_d("================================================================================\n");
    printf_d(" Soft Version = %s\n", txt_device_ver_soft);
    printf_d(" Hard Version = %s\n", txt_device_ver_hard);
    printf_d(" Device Name  = %s\n", txt_device_name);
    printf_d(" GIT commit   = %s\n", git_commit_str);
    printf_d(" GIT branch   = %s\n", git_branch_str);

#if HSI == 1
    printf_d(" HSI          = %lu Hz\n", HSI_CLK_HZ);
    printf_d(" SYSCLK       = %lu Hz\n", SYSCLK_HZ);
#endif    

#if HSE == 1
    printf_d(" HSE          = %lu Hz\n", HSE_CLK_HZ);
    printf_d(" SYSCLK       = %lu Hz\n", SYSCLK_HZ);
#endif    
    
    printf_d("================================================================================\n");
   
    
    
    //--------------------------------------------------------------------------
    FRESULT fres;
    uint32_t t_start = 0, t_stop = 0;
    FIL fil;
    uint32_t speed = 0;
    uint32_t r_bytes = 0;
    uint32_t w_bytes = 0;
    uint64_t file_size = 0;
    const uint32_t WRITE_FILE_SIZE = 10 * 1024 * 1024;

    fres = f_mount(&FatFs, "", 1);
    if (fres != FR_OK){
        printf_d("ERROR: Mount SD, error num = %d\n", fres);
        goto exit;
    }
    printf_d("Mount OK.\n");

    printf_d("DIR/LS-------------------------------------------------------------\n");
    scan_files ("0:/");

    printf_d("Test write file -------------------------------------------------------------\n");
    fres = f_open( &fil, "test.bin", FA_CREATE_ALWAYS | FA_WRITE);
    if (fres != FR_OK) {
        printf_d("ERROR Create file.\n");
        goto exit;
    }
    
    t_start = time_get_ms_counter();
    
    printf_d("Write:\n");
    
    file_size = WRITE_FILE_SIZE;
    
    while (file_size) {
        //printf_d(".");
        
        fres = f_write( &fil, file_io_buf, BLOCK_SIZE, &w_bytes);
        
        file_size -= w_bytes;
        
        if (fres != FR_OK) {
            printf_d( "ERROR: write.\n" );
            break;
        }
        
        if (w_bytes < BLOCK_SIZE) {
            printf_d( "ERROR: write size.\n" );
            break;
        }
    }

    t_stop = time_get_ms_counter();
    
    printf_d("t_start = %u, t_stop = %u\n", t_start, t_stop);
    speed = ((WRITE_FILE_SIZE)/ ((t_stop - t_start)/1000));
    printf_d("Speed write = %u b/s\n", speed);
    
    f_close(&fil);
    
    printf_d("Test read file -------------------------------------------------------------\n");
    fres = f_open( &fil, "test.bin", FA_OPEN_EXISTING | FA_READ);
    if (fres != FR_OK) {
        printf_d("ERROR Open file.\n");
        goto exit;
    }
    
    file_size = f_size(&fil);
    
	printf_d("File size = %llu\n", file_size);
    
    t_start = time_get_ms_counter();
    
    printf_d("Read:\n");
    while(1) {
        //printf_d(".");
        fres = f_read( &fil, file_io_buf, BLOCK_SIZE, &r_bytes);
        
        if (fres != FR_OK) {
            printf_d( "ERROR: read.\n" );
            break;
        }
        
        if (r_bytes < BLOCK_SIZE) {
            break;
        }
    }

    t_stop = time_get_ms_counter();
    
    printf_d("t_start = %u, t_stop = %u\n", t_start, t_stop);
    speed = ((file_size)/ ((t_stop - t_start)/1000));
    printf_d("Speed read = %u b/s\n", speed);
    
    f_close(&fil);
    
exit:
    
    printf_d("\nStop.\n");
    
    while (1) {
        consol_tx_data();
        delay_ms(1000);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
