#ifndef SPI_H_
#define SPI_H_

typedef struct {
    uint16_t cpha:1;
    uint16_t cpol:1;
    uint16_t mstr:1;
    uint16_t br:3;
    uint16_t spe:1;
    uint16_t lsbfirst:1;
    uint16_t ssi:1;
    uint16_t ssm:1;
    uint16_t rxonly:1;
    uint16_t dff:1;
    uint16_t crcnext:1;
    uint16_t crcen:1;
    uint16_t bidioe:1;
    uint16_t bidimode:1;
} spi_cr1_reg_t;


typedef union {
    uint16_t data;
    spi_cr1_reg_t reg;
} spi_cr1_reg_ut;

typedef struct {
    uint16_t rxdmaen:1;
    uint16_t txdmaen:1;
    uint16_t ssoe:1;
    uint16_t res0:1;
    uint16_t frf:1;
    uint16_t errie:1;
    uint16_t rxneie:1;
    uint16_t txeie:1;
    uint16_t res1:8;
} spi_cr2_reg_t;

typedef union {
    uint16_t data;
    spi_cr2_reg_t reg;
} spi_cr2_reg_ut;

// spi_sr - не заносил, работаем по флагам и маскам !
// spi_i2s, spi_i2spr - не заносил,т.к. не используем данный режим

uint8_t spi_io(SPI_TypeDef * spi, uint8_t data);

#endif