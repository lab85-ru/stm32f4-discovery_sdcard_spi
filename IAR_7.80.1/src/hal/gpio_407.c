#include "stm32f407xx.h"
#include "gpio_407.h"

//=============================================================================
// GPIO init
//=============================================================================
void gpio_init(GPIO_TypeDef *port, uint16_t pin_bit, gpio_mode_t mode, gpio_otype_t otype, gpio_speed_t speed, gpio_pull_t pull, uint8_t af)
{
    uint8_t pin = pin_bit & 0x0f;
    
    switch(pin_bit){
    case 0x0001: pin = 0; break;
    case 0x0002: pin = 1; break;
    case 0x0004: pin = 2; break;
    case 0x0008: pin = 3; break;

    case 0x0010: pin = 4; break;
    case 0x0020: pin = 5; break;
    case 0x0040: pin = 6; break;
    case 0x0080: pin = 7; break;

    case 0x0100: pin = 8; break;
    case 0x0200: pin = 9; break;
    case 0x0400: pin = 10; break;
    case 0x0800: pin = 11; break;

    case 0x1000: pin = 12; break;
    case 0x2000: pin = 13; break;
    case 0x4000: pin = 14; break;
    case 0x8000: pin = 15; break;
    case 0: return;  // если 0 значит ОШИБКА выходим
    }

    port->MODER   &= ~(3 << (pin *2));
    port->MODER   |= mode  << (pin * 2);
    
    port->OTYPER  &= ~(1 << pin);
    port->OTYPER  |= otype << pin;
    
    port->OSPEEDR &= ~(3 << (pin * 2));
    port->OSPEEDR |= speed << (pin * 2);
    
    port->PUPDR   &= ~(3  << (pin * 2));
    port->PUPDR   |= pull  << (pin * 2);

    if (pin <= 7){
        port->AFR[0] &= ~(0xf << (pin * 4));
        port->AFR[0] |= (af & 0xf) << (pin * 4);
    } else {
        port->AFR[1] &= ~(0xf << ((pin - 8) * 4));
        port->AFR[1] |= (af & 0xf) << ((pin - 8) * 4);
    }
}

//=============================================================================
// GPIO set 0
//=============================================================================
void gpio_set_0(GPIO_TypeDef *port, uint16_t pin_bit)
{
    port->BSRR |= pin_bit << 16;
}

//=============================================================================
// GPIO set 1
//=============================================================================
void gpio_set_1(GPIO_TypeDef *port, uint16_t pin_bit)
{
    port->BSRR |= pin_bit;
}

//=============================================================================
// GPIO get
//=============================================================================
uint32_t gpio_get(GPIO_TypeDef *port, uint16_t pin_bit)
{
    if (port->IDR & pin_bit)
        return 1;
    else
        return 0;
}