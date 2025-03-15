#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include "spi_407.h"

#define FCLK_SLOW() { \
    SPI2->CR1 &= ~SPI_CR1_SPE; \
    SPI2->CR1 &= ~(SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2); \
    SPI2->CR1 |=  (SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2); \
    SPI2->CR1 |=  SPI_CR1_SPE; } /* Set SCLK = PCLK / 256 */

#define FCLK_FAST() { \
    SPI2->CR1 &= ~SPI_CR1_SPE; \
    SPI2->CR1 &= ~(SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2); \
    SPI2->CR1 |=  SPI_CR1_SPE; }  /* Set SCLK = PCLK / 2 */

#define CS_HIGH()	spi2_cs(1)
#define CS_LOW()	spi2_cs(0)
#define	MMC_CD		1 /* Card detect (yes:true, no:false, default:true) */
#define	MMC_WP		0 /* Write protected (yes:true, no:false, default:false) */


#define WAIT_TX_READY()     while( (SPI2->SR & SPI_SR_TXE) == 0 )
#define WAIT_TX_END()       while( (SPI2->SR & (SPI_SR_BSY | SPI_SR_TXE | SPI_SR_RXNE)) != (SPI_SR_TXE | SPI_SR_RXNE) )

#define SPI_DR_8BIT         *(volatile uint8_t*)&SPI2->DR
#define SPI_DR_16BIT        *(volatile uint16_t*)&SPI2->DR

#define SPI_SET_16BIT() { \
    SPI2->CR1 &= ~SPI_CR1_SPE; \
    SPI2->CR1 |= (SPI_CR1_SPE | SPI_CR1_DFF); }

    
#define SPI_SET_8BIT() { \
    SPI2->CR1 &= ~(SPI_CR1_SPE | SPI_CR1_DFF); \
    SPI2->CR1 |= SPI_CR1_SPE; }

#endif
