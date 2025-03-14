// Подпрограммы работы с железом

#include "main.h"
#include "board.h"
#include "hardware.h"
#include "gpio_407.h"
#include "usart_407.h"
#include "rcc_407.h"
#include "gpio_407.h"
#include "dma_407.h"
#include "i2c_407.h"
#include "time_hal.h"
#include "spi_407.h"

#include "global_var.h"

#ifndef DEBUG_HARD
#define DEBUG_HARD 0
#endif


//------------------------------------------------------------------------------
// spi2( APB1 - MHz ) - SDCARD
//------------------------------------------------------------------------------
void spi2_init(void)
{
    spi_cr1_reg_ut spi_cr1;
    spi_cr2_reg_ut spi_cr2;

    rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);  // GPIOB CLK EN
    rcc_clk_en(&RCC->APB1ENR, RCC_APB1ENR_SPI2EN);   // SPI2 CLK EN

    gpio_init(SPI2_CLK_PORT,  SPI2_CLK,   GPIO_MODE_AF, GPIO_OTYPE_PUSHPULL, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_DOWN, GPIO_AF5_SPI2);
    gpio_init(SPI2_MOSI_PORT, SPI2_MOSI,  GPIO_MODE_AF, GPIO_OTYPE_PUSHPULL, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_UP, GPIO_AF5_SPI2);
    gpio_init(SPI2_MISO_PORT, SPI2_MISO,  GPIO_MODE_AF, GPIO_OTYPE_PUSHPULL, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_UP, GPIO_AF5_SPI2);

    gpio_init(SPI2_CS_PORT,   SPI2_CS_L,  GPIO_MODE_OUT, GPIO_OTYPE_PUSHPULL, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_UP, 0);
    gpio_set_1(SPI2_CS_PORT,  SPI2_CS_L);
    
// SPI Init --------------------------------------------------------------------    
    //spi_cr1.data = 0;
    spi_cr1.reg.cpha     = 0; // 0: The first clock transition is the first data capture edge
    spi_cr1.reg.cpol     = 0; // 0: CK to 0 when idle
    spi_cr1.reg.mstr     = 1; // 1: Master configuration 
    spi_cr1.reg.br       = 6; // APB2(108MHz)/128 (110: fPCLK/128) = 0.843 MHz
    spi_cr1.reg.spe      = 0; // 0: Peripheral disable 
    spi_cr1.reg.lsbfirst = 0; // 0: data is transmitted / received with the MSB first
    spi_cr1.reg.ssi      = 0; // ???Internal slave select
    spi_cr1.reg.ssm      = 1; // Software NSS management (SSM = 1),  0: Software slave management disabled
    spi_cr1.reg.rxonly   = 0; // 0: Full-duplex (Transmit and receive)
    spi_cr1.reg.dff      = 0; // 0: Data frame format. 0: 8-bit data, 1: 16-bit data frame format is selected for transmission/reception
    spi_cr1.reg.crcnext  = 0; // 0: Next transmit value is from Tx buffer.
    spi_cr1.reg.crcen    = 0; // 0: CRC calculation disabled
    spi_cr1.reg.bidioe   = 0; // 0: Output disabled (receive-only mode)  
    spi_cr1.reg.bidimode = 0; // 0: 2-line unidirectional data mode selected 
    
    SPI2->CR1 = spi_cr1.data;
   
    
    //spi_cr2.data = 0;
    spi_cr2.reg.rxdmaen = 0; // 0: Rx buffer DMA disabled
    spi_cr2.reg.txdmaen = 0; // 0: Tx buffer DMA disabled
    spi_cr2.reg.ssoe    = 1; // 1: SS output is enabled in master mode and when the SPI interface is enabled.
    spi_cr2.reg.frf     = 0; // 0: SPI Motorola mode
    spi_cr2.reg.errie   = 0; // 0: Error interrupt is masked
    spi_cr2.reg.rxneie  = 0; // 0: RXNE interrupt masked
    spi_cr2.reg.txeie   = 0; // 0: TXE interrupt masked  

    SPI2->CR2 = spi_cr2.data;
    SPI2->CR1 |= SPI_CR1_SPE;     // enable SPI
}

//------------------------------------------------------------------------------
// Set SPI2 - CS.
//------------------------------------------------------------------------------
void spi2_cs(uint8_t cs)
{
    if (cs) {
        gpio_set_1(SPI2_CS_PORT,  SPI2_CS_L);
    } else {
        gpio_set_0(SPI2_CS_PORT,  SPI2_CS_L);
    }
}

//------------------------------------------------------------------------------
// usart2( APB1 - MHz ) - Console
//------------------------------------------------------------------------------
void uart2_console_init(void)
{
    usart_cr1_reg_ut usart_cr1;
    usart_cr2_reg_ut usart_cr2;
    usart_cr3_reg_ut usart_cr3;
    dma_cr_reg_ut dma_cfg;
    
    rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);  // GPIO CLK EN
    rcc_clk_en(&RCC->APB1ENR, RCC_APB1ENR_USART2EN); // USART CLK EN
    rcc_clk_en(&RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN);   // DMA1 CLK EN
    
    // gpio_init
    gpio_init(USART2_TX_PORT, USART2_TX, GPIO_MODE_AF, GPIO_OTYPE_PUSHPULL, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_UP, GPIO_AF7_USART2);
    gpio_init(USART2_RX_PORT, USART2_RX, GPIO_MODE_AF, GPIO_OTYPE_PUSHPULL, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_UP, GPIO_AF7_USART2);
    
    // DMA TX ------------------------------------------------------------------
    dma_cfg.reg.en = 0;    // 0: stream disabled
    dma_cfg.reg.teie=1;    // 1: TE interrupt enabled
    dma_cfg.reg.htie=0;    // 0: HT interrupt disabled
    dma_cfg.reg.tcie=1;    // 1: TC interrupt enabled
    dma_cfg.reg.dir=1;     // 0: Read from peripheral 1: Read from memory
    dma_cfg.reg.circ=0;    // 0: circular mode disabled
    dma_cfg.reg.pinc=0;    // peripheral increment mode(0 - peripheral address pointer is fixed)
    dma_cfg.reg.minc=1;    // memory increment mode(memory address pointer is incremented after each data transfer)
    dma_cfg.reg.psize=0;   // peripheral data size(byte (8-bit))
    dma_cfg.reg.msize=0;   // memory data size (byte (8-bit))
    dma_cfg.reg.chsel=4;   // Channel 4

    dma_init(DMA1_Stream6,             // DMA stream
             (uint32_t)&(USART2->DR),  // adr reg
             0,                        // adr ram 0
             0,                        // size
             dma_cfg.data              // dma config
             );
    
    NVIC_SetPriority(DMA1_Stream6_IRQn, 5);
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);

    // USART INIT --------------------------------------------------------------
    USART2->CR1 = 0; // clear
    USART2->CR2 = 0; // clear
    USART2->CR3 = 0; // clear
    USART2->BRR = 0; // clear
    
    // Tx/Rx baud = fCK / USARTDIV - for 16 oversampling
    // USARTDIV = fCK / Tx/Rx baud
    USART2->BRR = (APB1_CLK_HZ / UART2_CONSOLE_SPEED) & 0xffff;
//#warning "disable for 16 MHz"
//    USART2->BRR = (16000000 / UART2_CONSOLE_SPEED) & 0xffff;
    
    usart_cr2.data = 0;    
    usart_cr2.reg.lbcl    = 0; // LBCL: Last bit clock pulse
    usart_cr2.reg.cpha    = 0; // CPHA: Clock phase
    usart_cr2.reg.cpol    = 0; // CPOL: Clock polarity
    usart_cr2.reg.clken   = 0; // CLKEN: Clock enable
    usart_cr2.reg.stop    = 0; // STOP[1:0]: STOP bits, 00: 1 stop bit
    USART2->CR2 = usart_cr2.data;
    
    usart_cr3.data = 0;    
    usart_cr3.reg.eie      = 0; // EIE: Error interrupt enable
    usart_cr3.reg.hdsel    = 0; // HDSEL: Half-duplex selection
    usart_cr3.reg.dmar     = 0; // DMAR: DMA enable receiver
    usart_cr3.reg.dmat     = 1; // DMAT: DMA enable transmitter
    usart_cr3.reg.rtse     = 0; // RTSE: RTS enable
    usart_cr3.reg.ctse     = 0; // CTSE: CTS enable
    usart_cr3.reg.ctsie    = 0; // CTSIE: CTS interrupt enable
    usart_cr3.reg.onebit   = 1; // ONEBIT: One sample bit method enable, 1: One sample bit method NF-disable
    USART2->CR3 = usart_cr3.data;
    
    usart_cr1.data      = 0;
    usart_cr1.reg.ue    = 1; // UE: USART enable, 
    usart_cr1.reg.re    = 1; // RE: Receiver enable, 1: Receiver is enabled
    usart_cr1.reg.te    = 1; // TE: Transmitter enable, 1: Transmitter is enabled 
    usart_cr1.reg.idleie = 0;// IDLEIE: IDLE interrupt enable, 0: Interrupt is inhibited
    usart_cr1.reg.rxneie = 1;// RXNEIE: RXNE interrupt enable, 1: A USART interrupt is generated whenever ORE=1 or RXNE=1
    usart_cr1.reg.tcie  = 0; // TCIE: Transmission complete interrupt enable, 0: Interrupt is inhibited
    usart_cr1.reg.txeie = 0; // TXEIE: interrupt enable, 0: Interrupt is inhibited
    usart_cr1.reg.peie  = 0; // PEIE: PE interrupt enable, 0: Interrupt is inhibited
    usart_cr1.reg.ps    = 0; // PS: Parity selection, 0: Even parity
    usart_cr1.reg.pce   = 0; // PCE: Parity control enable, 0: Parity control disabled
    usart_cr1.reg.wake  = 0; // WAKE: Receiver wakeup method, 0: Idle line
    usart_cr1.reg.over8 = 0; // OVER8: Oversampling mode, 0: Oversampling by 16; 1: Oversampling by 8
    USART2->CR1 = usart_cr1.data;
    
    NVIC_SetPriority(USART2_IRQn, 5);
    NVIC_EnableIRQ(USART2_IRQn);
}

//-----------------------------------------------------------------------------
// usart2 console - RX INT
//-----------------------------------------------------------------------------
void uart2_console_rx_int(void)
{
    if (USART2->SR & USART_SR_RXNE) {
        uint8_t d = USART2->DR;
        
        uint32_t dl = get_free_size_queue( &q_consol_rx );
        if ( dl > 1 ) {
            push_data_queue_b( &q_consol_rx, d );
        }
    }
}

//-----------------------------------------------------------------------------
// Перезагрузка
//-----------------------------------------------------------------------------
void run_reboot(void)
{
    //periphery_disable();
        
    __disable_irq();
    for (uint32_t i=0; i<8; i++) {    
        NVIC->ICER[i] = 0xffffffff; // Сбрасываем все прирывания
        NVIC->ICPR[i] = 0xffffffff; // Сбрасываем все прирывания в ожидании
    }
    
    NVIC_SystemReset();
}

//-----------------------------------------------------------------------------
// Init hardware
//-----------------------------------------------------------------------------
void hardware_init (void)
{
    spi2_init();
    uart2_console_init();
}
