// DMA for STM32F407
#include "stm32f407xx.h"


//------------------------------------------------------------------------------
// RESET DMA
//------------------------------------------------------------------------------
void dma_reset(DMA_Stream_TypeDef * st)
{
    st->CR   = 0;
    st->NDTR = 0;
    st->PAR  = 0;
    st->M0AR = 0;
    st->M1AR = 0;

    if (st == DMA1_Stream0){
        DMA1->LIFCR = DMA_LIFCR_CFEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTCIF0;
        return;
    }
    if (st == DMA1_Stream1){
        DMA1->LIFCR = DMA_LIFCR_CFEIF1 | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CHTIF1 | DMA_LIFCR_CTCIF1;
        return;
    }
    if (st == DMA1_Stream2){
        DMA1->LIFCR = DMA_LIFCR_CFEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CHTIF2 | DMA_LIFCR_CTCIF2;
        return;
    }
    if (st == DMA1_Stream3){
        DMA1->LIFCR = DMA_LIFCR_CFEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTCIF3;
        return;
    }
    if (st == DMA1_Stream4){
        DMA1->HIFCR = DMA_HIFCR_CFEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTCIF4;
        return;
    }
    if (st == DMA1_Stream5){
        DMA1->HIFCR = DMA_HIFCR_CFEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTCIF5;
        return;
    }
    if (st == DMA1_Stream6){
        DMA1->HIFCR = DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6;
        return;
    }
    if (st == DMA1_Stream7){
        DMA1->HIFCR = DMA_HIFCR_CFEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTCIF7;
        return;
    }
    
    // -------------------------------------------------------------------------
    if (st == DMA2_Stream0){
        DMA2->LIFCR = DMA_LIFCR_CFEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTCIF0;
        return;
    }
    if (st == DMA2_Stream1){
        DMA2->LIFCR = DMA_LIFCR_CFEIF1 | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CHTIF1 | DMA_LIFCR_CTCIF1;
        return;
    }
    if (st == DMA2_Stream2){
        DMA2->LIFCR = DMA_LIFCR_CFEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CHTIF2 | DMA_LIFCR_CTCIF2;
        return;
    }
    if (st == DMA2_Stream3){
        DMA2->LIFCR = DMA_LIFCR_CFEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTCIF3;
        return;
    }
    if (st == DMA2_Stream4){
        DMA2->HIFCR = DMA_HIFCR_CFEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTCIF4;
        return;
    }
    if (st == DMA2_Stream5){
        DMA2->HIFCR = DMA_HIFCR_CFEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTCIF5;
        return;
    }
    if (st == DMA2_Stream6){
        DMA2->HIFCR = DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6;
        return;
    }
    if (st == DMA2_Stream7){
        DMA2->HIFCR = DMA_HIFCR_CFEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTCIF7;
        return;
    }
    
}

//------------------------------------------------------------------------------
// Init DMA
//------------------------------------------------------------------------------
void dma_init(DMA_Stream_TypeDef * st, uint32_t adr_hard, uint32_t adr_ram0, uint32_t size, uint32_t cfg)
{
    st->CR   = 0;
    st->PAR  = adr_hard;
    st->M0AR = adr_ram0;
    st->NDTR = size;
    st->CR   = cfg;
}

//------------------------------------------------------------------------------
// DMA set adr ram + size DMA
//------------------------------------------------------------------------------
void dma_set_ram(DMA_Stream_TypeDef * st, uint32_t adr_ram0, uint32_t size)
{
    st->NDTR = size;
    st->M0AR = adr_ram0;
}

//------------------------------------------------------------------------------
// DMA set NEW size NDTR
//------------------------------------------------------------------------------
void dma_set_size(DMA_Stream_TypeDef * st, uint32_t size)
{
    st->NDTR = size;
}

//------------------------------------------------------------------------------
//dma on
//------------------------------------------------------------------------------
void dma_on(DMA_Stream_TypeDef * st)
{
    st->CR |= (uint32_t) 1;
}

//------------------------------------------------------------------------------
// dma off
//------------------------------------------------------------------------------
void dma_off(DMA_Stream_TypeDef * st)
{
    st->CR &= ~((uint32_t)1);
}