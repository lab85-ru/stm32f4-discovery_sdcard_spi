// SPI
#include "stm32f407xx.h"
#include "spi_407.h"

void spi_init(SPI_TypeDef * spi, uint16_t cr1_reg, uint16_t cr2_reg)
{
     spi->CR1 = cr1_reg;
     spi->CR2 = cr2_reg;
  
}

//------------------------------------------------------------------------------
// SPI Запись + чтение одного байта
//------------------------------------------------------------------------------
uint8_t spi_io(SPI_TypeDef * spi, uint8_t data)
{
    uint8_t b;
        
    while( (spi->SR & SPI_SR_TXE) == 0);
    *(volatile uint8_t *)&spi->DR = data;

    while( (spi->SR & SPI_SR_RXNE) == 0);
    *(volatile uint8_t *)&b = spi->DR;
        
    return b;
}



