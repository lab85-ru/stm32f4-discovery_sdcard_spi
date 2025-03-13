/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include "board.h"
#include "queue_buf.h"

#define USART_BUF_RX_LEN 		(2048 * 2)// длинна кольцевого буфера модема
#define MODEM_BUF_READ_LEN 		(512) // длинна буфера для обработки sms и команд модему

#define SIGMA_UART_PORT_MAX 	(4)// количество внешних пользовательских портов устройства

#define QUEUE_CONSOL_RX_SIZE	(64)// длинна входного буфера консоли

#define EXT_UART_SPEED_DEFAULT  (9600) // Скорость по умолчанию для внешних портов
#define EXT_UART_SPEED_MAX      (115200 * 4) // Максимальная скорость работы портов

#define QUEUE_UART1_RX_LEN_MAX  (2048)
#define QUEUE_UART1_TX_LEN_MAX  (128)
#define QUEUE_UART4_RX_LEN_MAX  (2048)
#define QUEUE_UART4_TX_LEN_MAX  (128)
#define QUEUE_UART5_RX_LEN_MAX  (2048)
#define QUEUE_UART5_TX_LEN_MAX  (128)
#define QUEUE_UART6_RX_LEN_MAX  (2048)
#define QUEUE_UART6_TX_LEN_MAX  (128)




#define QUEUE_FROM_PC_LEN		(16)	//длинна очереди пакетов от ПК
#define QUEUE_TO_PC_LEN			(64)	//длинна очереди пакетов к ПК

#define TCP_TX_BUF_SIZE			(512)	// промежуточный буффер для накопления пакетов и передачи в стек
#define RX_FROM_TCP_BUF_LEN_MAX (PAK_LEN_MAX * 32)

#define TIME_OUT_RX_FROM_TCP	(5)		// таймаут на прием из стека
#define TIME_OUT_TX_TO_TCP		(20)	// таймаут на передачу в стек
#define TIME_OUT_PPPCLOSE		(30000)	// таймаут на закрытие pppClose
#define TIME_OUT_NO_DATA		(300000)// таймаут на закрытие ppp соединения в случаии отсутствия принимаемых-передаваемых данных

//#define QUEUE_TCP_FROM_PC_BUf_SIZE (2048) // дополнительный буфер для данных приходящих из стека(при перегрузке количество данных может отличаться )
#define BUF_ASSEMBLY_PAK_SIZE 	(PAK_LEN_MAX) // промежуточный буффер для сборки неполного пакета

#define RECIV_TIME_OUT 10 // таймаут для приема данных из TCP Changed LWIP_SO_RCVTIMEO  to '1';

typedef struct {
	queue_buf_t *qp_uart_tx     [ SIGMA_UART_PORT_MAX ];	// указатели на кольцевые буфера
	queue_buf_t *qp_uart_rx     [ SIGMA_UART_PORT_MAX ];
	uint8_t      uart_speed     [ SIGMA_UART_PORT_MAX ];	// скорость на которую настроен порт (значение из пакета(от ПК байт)
	uint32_t     uart_rx_time   [ SIGMA_UART_PORT_MAX ];	// системное время последнего принятого байта (ms)
    uint32_t     uart_rx_time_out [ SIGMA_UART_PORT_MAX ];	// значение таймаута для конкретной скорости, расчитывается при настройке порта (ms)
    uint32_t     uart_rx_data_size_prev [ SIGMA_UART_PORT_MAX ]; // Размер данных в буфере (предыдущее значение) для вычисления таймаута
	uint32_t     uart_baudrate  [ SIGMA_UART_PORT_MAX ];	// скорость порта биты в секунду
} ext_uart_cfg_t;

#define mainLED_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define mainPPPSTART_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainTestHard_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainWaitSMS_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainPaketHandling_TASK_PRIORITY	( tskIDLE_PRIORITY + 2 )
#define mainCONSOL_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

typedef enum{			// состояние в котором находится автомат соединения с инетом
	STATE_IDLE,
	STATE_DIAL,
	STATE_OPEN_PPP,
	STATE_CONNECTION_PPP,
	STATE_DATA_TRANSFER,
	STATE_CLOSE_PPP,
	STATE_WAIT_CLOSE_PPP
} pppstart_state_t;

typedef enum{
	PPP_TIME_OUT_MSG,			// time out na ppp start
	PPP_CONNECTION_MSG,
	PPP_CLOSE_MSG,
	PPPERR_PROTOCOL_MSG,
	PPPERR_CONNECT_MSG,
	PPPERR_PARAM_MSG,
	PPPERR_OPEN_MSG,
	PPPERR_DEVICE_MSG,
	PPPERR_ALLOC_MSG,
	PPPERR_AUTHFAIL_MSG,
	PPPERR_UNHANDLED_ERROR_MSG,
	NETCONN_ERR_NEW,
	NETCONN_ERR_CONNECT
} pppstatuscv_t;

typedef enum{
	STATE_WAIT_SMS,
	STATE_RUN_GPRS
} sms_state_t;

typedef enum{
	R_ON,
	G_ON,
	RG_ON,
	G_BLINK,
	R_BLINK,
	RG_BLINK
} led_blink_state;

#define DEVICE_READY			" Device Ready."

// Флаг запуск GPRS соединения (флаг устанавливается если IP адрес из SMS коректен)
// сбрасывается флаг в ручную !
typedef enum { NO_GPRS_SESSION=0, START_GPRS_SESSION=1 } start_gprs_session_t;

#define IP_C_SIZE				(16)	// xxx.xxx.xxx.xxx\0 dlinna adresa+\0 char

typedef struct {
	start_gprs_session_t flag;  // status GPRS session
	char ip_c[ IP_C_SIZE ];     // text ip addres for connect
	uint16_t port_u16;          // text tcp port
}inet_adr_pc;

#define TIME_OUT_PPP_START		(30000)	// vrema na podklucheniu ppp,systemnie tiki !

// Min value of 4s
#define WDT_MAX_VALUE_US   4000000

#define SMS_BUF_SIZE 			(256)	                // длинна буфер в который считывается SMS
#define SMS_TXT_OUT_SIZE   		(256)
#define STR_CMD_TXT_SIZE		(64)	                // здесь хранится текст команды из SMS

typedef enum {SC_NO=0, SC_OK=1, SC_PIN=2} simcard_status_t;
typedef enum {CREG_0=0,CREG_1=1,CREG_2=2,CREG_3=3,CREG_4=4,CREG_5=5} modem_creg_t;

#define STR_SIZE   (350)     // razmer stroki dla -> get_string
#define SMS_BUF_LEN (512)

typedef struct {
        simcard_status_t simcard_status;  // status sim carti
        unsigned char rssi;               // rssi - uroven signala modema, diapazon ot 0 --- do 31
        modem_creg_t creg;                // status registrachii v seti
        uint8_t mdm_reinit_count;         // DEBUG - skolko ciklov modem reset+init 
} device_status_t;


#define QBUF_MODEM_BUF_READ_LEN (2048) // Size queue buffer for RX data from modem

void simcard_error(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
