#ifndef USART_H_
#define USART_H_

typedef struct {
    uint32_t sbk:1;
    uint32_t rwu:1;
    uint32_t re:1;
    uint32_t te:1;
    uint32_t idleie:1;
    uint32_t rxneie:1;
    uint32_t tcie:1;
    uint32_t txeie:1;
    uint32_t peie:1;
    uint32_t ps:1;
    uint32_t pce:1;
    uint32_t wake:1;
    uint32_t m:1;
    uint32_t ue:1;
    uint32_t RES0:1;
    uint32_t over8:1;
    uint32_t RES1:16;
} usart_cr1_reg_t;

typedef union {
    uint32_t data;
    usart_cr1_reg_t reg;
} usart_cr1_reg_ut;


typedef struct {
    uint32_t add:4;
    uint32_t RES0:1;
    uint32_t lbdl:1;
    uint32_t lbdie:1;
    uint32_t RES1:1;
    uint32_t lbcl:1;
    uint32_t cpha:1;
    uint32_t cpol:1;
    uint32_t clken:1;
    uint32_t stop:2;
    uint32_t linen:1;
    uint32_t RES2:17;
} usart_cr2_reg_t;

typedef union {
    uint32_t data;
    usart_cr2_reg_t reg;
} usart_cr2_reg_ut;


typedef struct {
    uint32_t eie:1;
    uint32_t iern:1;
    uint32_t irlp:1;
    uint32_t hdsel:1;
    uint32_t nack:1;
    uint32_t scen:1;
    uint32_t dmar:1;
    uint32_t dmat:1;
    uint32_t rtse:1;
    uint32_t ctse:1;
    uint32_t ctsie:1;
    uint32_t onebit:1;
    uint32_t RES0:20;
} usart_cr3_reg_t;

typedef union {
    uint32_t data;
    usart_cr3_reg_t reg;
} usart_cr3_reg_ut;


void usart_reset(USART_TypeDef *u);

#endif