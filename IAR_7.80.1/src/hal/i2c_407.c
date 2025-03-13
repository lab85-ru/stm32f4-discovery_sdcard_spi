#include <stdint.h>
#include "stm32f407xx.h"
#include "rcc_407.h"
#include "gpio_407.h"
#include "i2c_407.h"

//------------------------------------------------------------------------------
// Init I2C - GPIO init
//------------------------------------------------------------------------------
void i2c_gpio_init(GPIO_TypeDef *clk_port, uint16_t clk_pin_bit, GPIO_TypeDef *sda_port, uint16_t sda_pin_bit, uint8_t af)
{
    rcc_clk_en_for_gpio(clk_port);
    rcc_clk_en_for_gpio(sda_port);
    gpio_init(clk_port, clk_pin_bit, GPIO_MODE_AF, GPIO_OTYPE_OPENDRAIN, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_NO, af);
    gpio_init(sda_port, sda_pin_bit, GPIO_MODE_AF, GPIO_OTYPE_OPENDRAIN, GPIO_SPEEDF_VERYHIGH, GPIO_PULL_NO, af);
}


//------------------------------------------------------------------------------
// Init I2C (master)
//------------------------------------------------------------------------------
void i2c_init(I2C_TypeDef * i2c_dev, GPIO_TypeDef *clk_port, uint16_t clk_pin_bit, GPIO_TypeDef *sda_port, uint16_t sda_pin_bit, uint32_t speed, uint8_t af)
{

    i2c_gpio_init(clk_port, clk_pin_bit, sda_port, sda_pin_bit, af);
    
    rcc_clk_en(&RCC->APB1ENR, RCC_APB1ENR_I2C1EN);

    i2c_dev->CCR = speed & I2C_CCR_CCR_Msk;
    i2c_dev->CR2 = I2C_CR2_FREQ_1; // 2 Mhz
    i2c_dev->CR1 = I2C_CR1_PE;

}

//------------------------------------------------------------------------------
// I2C read (len) bytes to (*buf_read)
// return:
// 0 - OK
// 1 - ERROR
//------------------------------------------------------------------------------
uint8_t i2c_read_array(I2C_TypeDef *i2c_dev, const uint8_t i2c_dev_adr7bit, uint8_t *buf_read, const uint16_t len)          // data read length
{
    volatile uint16_t sr1, sr2;

    if (len == 1) {
        i2c_dev->CR1 |= I2C_CR1_ACK; // set ACK
        // Start
        i2c_dev->CR1 |= I2C_CR1_START;

        do {
            sr1 = i2c_dev->SR1;

            if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) return 1; // error
        } while ((sr1 & I2C_SR1_SB) == 0);

        // i2c ADR(7bit) + R=1
        i2c_dev->DR = (i2c_dev_adr7bit << 1) | 1;

        do {
            sr1 = i2c_dev->SR1;
        
            if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF)) return 1; // error
        } while ((sr1 & I2C_SR1_ADDR) == 0);
        sr1 = i2c_dev->SR1;
        sr2 = i2c_dev->SR2;

        i2c_dev->CR1 &= ~I2C_CR1_ACK;
        // stop
        i2c_dev->CR1 |= I2C_CR1_STOP;

        do {
            sr1 = i2c_dev->SR1;
            //sr2 = i2c_dev->SR2;

            if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) return 1; // error
        } while ((sr1 & I2C_SR1_RXNE) == 0);

        buf_read[ 0 ] = i2c_dev->DR;
        
        return 0;
    }

    // for len > 1 -------------------------------------------------------------
    // Start
    i2c_dev->CR1 |= I2C_CR1_ACK; // set ACK
    i2c_dev->CR1 |= I2C_CR1_START;

    do {
        sr1 = i2c_dev->SR1;
        sr2 = i2c_dev->SR2;

        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) return 1; // error
    } while ((sr1 & I2C_SR1_SB) == 0);

    // i2c ADR(7bit) + R=1
    i2c_dev->DR = (i2c_dev_adr7bit << 1) | 1;

    do {
        sr1 = i2c_dev->SR1;
        
        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF)) return 1; // error
    } while ((sr1 & I2C_SR1_ADDR) == 0);
    sr1 = i2c_dev->SR1;
    sr2 = i2c_dev->SR2;

    // read 
    i2c_dev->CR1 |= I2C_CR1_ACK; // set ACK
    
    uint16_t i = 0;
    while (len - i > 2) {
        do {
            sr1 = i2c_dev->SR1;

            if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) return 1; // error
        } while ((sr1 & I2C_SR1_RXNE) == 0);

        buf_read[ i ] = i2c_dev->DR;
        i++;

        i2c_dev->CR1 |= I2C_CR1_ACK; // set ACK
    }
    
    do {
        sr1 = i2c_dev->SR1;

        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) return 1; // error
    } while ((sr1 & I2C_SR1_RXNE) == 0);

    buf_read[ i ] = i2c_dev->DR;
    i++;
        
    i2c_dev->CR1 &= ~I2C_CR1_ACK; // clear ACK

    // stop
    i2c_dev->CR1 |= I2C_CR1_STOP;

    do {
        sr1 = i2c_dev->SR1;

        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) return 1; // error
    } while ((sr1 & I2C_SR1_RXNE) == 0);

    buf_read[ i ] = i2c_dev->DR;
    
    return 0; // OK
}

//------------------------------------------------------------------------------
// I2C write (len), (*buf_in) bytes to DEV
// return:
// 0 - OK
// 1 - ERROR
//------------------------------------------------------------------------------
uint8_t i2c_write_array(I2C_TypeDef * i2c_dev, const uint8_t i2c_dev_adr7bit, const uint8_t *buf_in, const uint16_t len)
{
    volatile uint16_t sr1, sr2;

    // Start
    i2c_dev->CR1 |= I2C_CR1_START;

    do {
        sr1 = i2c_dev->SR1;

        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) 
            return 1; // error
        
    } while((sr1 & I2C_SR1_SB) == 0);

    // i2c ADR(7bit) + W=0
    i2c_dev->DR = (i2c_dev_adr7bit << 1) | 0;

    do {
        sr1 = i2c_dev->SR1;

        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF)) 
            return 1; // error
        
    } while((sr1 & I2C_SR1_ADDR) == 0);
    sr1 = i2c_dev->SR1;
    sr2 = i2c_dev->SR2;

    // write
    for (uint16_t i=0; i<len; i++) {
        while(1) {
            sr1 = i2c_dev->SR1;

            if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF)) 
                return 1; // error

            if (sr1 & I2C_SR1_TXE) {
                i2c_dev->DR = buf_in[ i ];
                break;
            }
        }
    }

    // wait data end TX
    do {
        sr1 = i2c_dev->SR1;

        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF)) 
            return 1; // error
        
    } while((sr1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == 0);
    sr2 = i2c_dev->SR2;
    
    
    // stop
    i2c_dev->CR1 |= I2C_CR1_STOP;

    while(i2c_dev->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF));

    return 0; // OK
}






