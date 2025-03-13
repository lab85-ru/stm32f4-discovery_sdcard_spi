//------------------------------------------------------------------------------
// Print array to HEX format Library.
//
// V 1.0.23 Sviridov Georgy sgot@inbox.ru aka info@lab85.ru
//------------------------------------------------------------------------------
#include "printf_hal.h" // Define alternative printf function or stdio.h
#include <string.h>
#include <stdlib.h>

#define DUMP_STRING_LEN (16)
#define HEX_OUT_BUF_SIZE (128)
static char hex_out_buf[ HEX_OUT_BUF_SIZE ];
static char s[DUMP_STRING_LEN];

//------------------------------------------------------------------------------
// Optimization HEX out for MCPU.
// Out 1 string HEX to buf, print(buf)
//------------------------------------------------------------------------------
void hex_out(const char *buf_in, const size_t len)
{
    size_t n, i, j;
    size_t index = 0;
    char c;

    sprintf(&hex_out_buf[ index ], "\n");
    index = strlen(hex_out_buf);

    n = 0;

    for (i=0; i<len; i++) {
        c = *(buf_in + i);

        sprintf(&hex_out_buf[ index ], " %02X", (unsigned char)c);
        index = strlen(hex_out_buf);

        if (c < 0x20)
            s[ n ] = '.';
        else
            s[ n ] = c;

        n++;
        if (n == DUMP_STRING_LEN) {
            sprintf(&hex_out_buf[ index ], " | ");
            index = strlen(hex_out_buf);

            for (j=0; j<DUMP_STRING_LEN; j++) {
                sprintf(&hex_out_buf[ index ], "%c", s[ j ]);
                index = strlen(hex_out_buf);
            }

            n = 0;

            sprintf(&hex_out_buf[ index ], "\n");
            index = strlen(hex_out_buf);
        }
        printf("%s", hex_out_buf);
        index = 0;
    } // for ----------------------------------------------------

    index = 0;

    if (n > 0 && n < DUMP_STRING_LEN) {
        for (j=0; j<DUMP_STRING_LEN-n; j++) { 
            sprintf(&hex_out_buf[ index ], "   ");
            index = strlen(hex_out_buf);
        }
        sprintf(&hex_out_buf[ index ], " | ");
        index = strlen(hex_out_buf);

        for (j=0; j<n; j++) {
            sprintf(&hex_out_buf[ index ], "%c", s[ j ]);
            index = strlen(hex_out_buf);
        }

        sprintf(&hex_out_buf[ index ], "\n");
        printf("%s", hex_out_buf);
    }
}

//------------------------------------------------------------------------------
// hex out
//------------------------------------------------------------------------------
void hex_out_p(const char *buf_in, const size_t len)
{
    size_t n, i, j;
    char c;
    char s[DUMP_STRING_LEN];

    printf("\n");
    n = 0;

    for (i=0; i<len; i++) {
        c = *(buf_in + i);
        printf(" %02X",(unsigned char)c);

        if (c < 0x20)
            s[n] = '.';
        else
            s[n] = c;

        n++;
        if (n == DUMP_STRING_LEN) {
            printf(" | ");
            for (j=0; j<DUMP_STRING_LEN; j++) 
                printf("%c", s[j]);
                n = 0;
                printf("\n");
                //printf("addres= %04lx: ",i+1);
        }
    }

    if (n > 0 && n < DUMP_STRING_LEN) {
        for (j=0; j<DUMP_STRING_LEN-n; j++) printf("   ");
        printf(" | ");
        for (j=0; j<n; j++) printf("%c", s[j]);
    }

    printf("\n");
}

//------------------------------------------------------------------------------
// print to console hex buffer po 16 colonok
//------------------------------------------------------------------------------
void print_hex(const char *in, const size_t len)
{

    size_t i = 0;

    for (i=0; i<len; i++) {
        if (i == 16) printf("\n");
        printf(" %02x", *(in + i));
    }
    printf("\n");
}

//------------------------------------------------------------------------------
// dump - hex out queue (No Pop data !)
//------------------------------------------------------------------------------
#define DUMP_STRING_LEN (16)
void dump_queue(const char * buf, const size_t in, const size_t out, const size_t size )
{
    size_t i = 0;
    size_t j = 0;
    size_t ini  = in;
    size_t outi = out;
    char s[DUMP_STRING_LEN];
    char c;

    printf("\n=================DUMP QUEUE START==================\n");
    printf("in = %d out = %d\n", ini, outi);
    if (ini == outi) goto _end;

    do {
        c = *(buf + outi);
        printf(" %02X", (unsigned char)c);
        if (c < 0x20)
            s[i] = '.';
        else
            s[i] = c;
        i++;
        if (i == DUMP_STRING_LEN) {
            printf(" | ");
            for (j=0; j<DUMP_STRING_LEN; j++) printf("%c", s[j]);
            printf("\n");
            i = 0;
        }
        outi++;
        if (outi == size) outi = 0;
    } while ( ini != outi );

_end:
    if (i > 0 && i < DUMP_STRING_LEN) {
        for (j=0; j<DUMP_STRING_LEN-i; j++) printf("   ");

        printf(" | ");
        for (j=0; j<i; j++) printf("%c", s[j]);
    }

    printf("\n=================DUMP QUEUE END====================\n");
}
