#ifndef DMA_407_H_
#define DMA_407_H_

#include "stm32f407xx.h"

typedef struct {
    uint32_t en:1;
    uint32_t dmeie:1;
    uint32_t teie:1;
    uint32_t htie:1;
    uint32_t tcie:1;
    uint32_t pfctrl:1;

    uint32_t dir:2;
    uint32_t circ:1;
    uint32_t pinc:1;
    uint32_t minc:1;
    uint32_t psize:2;
    uint32_t msize:2;
    uint32_t pincos:1;

    uint32_t pl:2;
    uint32_t dbm:1;
    uint32_t ct:1;
    uint32_t RES0:1;
    uint32_t pburst:2;
    uint32_t mburst:2;
    uint32_t chsel:3;
    uint32_t RES1:4;
} cr_reg_t;

typedef union
{
    cr_reg_t reg;
    uint32_t data;
} dma_cr_reg_ut;

void dma_reset(DMA_Stream_TypeDef * st);
void dma_init(DMA_Stream_TypeDef * st, uint32_t adr_hard, uint32_t adr_ram0, uint16_t size, uint32_t cfg);
void dma_set_ram(DMA_Stream_TypeDef * st, uint32_t adr_ram0, uint16_t size);
void dma_set_size(DMA_Stream_TypeDef * st, uint32_t size);
void dma_on(DMA_Stream_TypeDef * st);
void dma_off(DMA_Stream_TypeDef * st);


#endif