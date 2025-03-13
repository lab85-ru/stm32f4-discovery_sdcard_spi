#ifndef I2C_407_H
#define I2C_407_H
#include <stdint.h>

void i2c_init(I2C_TypeDef * i2c_num, GPIO_TypeDef *clk_port, uint16_t clk_pin_bit, GPIO_TypeDef *sda_port, uint16_t sda_pin_bit, uint32_t speed, uint8_t af);
uint8_t i2c_read_array(I2C_TypeDef * i2c_dev, const uint8_t i2c_dev_adr7bit, uint8_t *buf_read, const uint16_t len);
uint8_t i2c_write_array(I2C_TypeDef * i2c_dev, const uint8_t i2c_dev_adr7bit, const uint8_t *buf_in, const uint16_t len);

#endif
