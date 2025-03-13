#ifndef GLOBAL_VAR_H_
#define GLOBAL_VAR_H_

#include <stdint.h>
#include "queue_buf.h"

extern volatile uint8_t flag_consol_tx;
extern volatile uint32_t consol_tx_size;
extern queue_buf_t q_consol_tx;
extern queue_buf_t q_consol_rx;




#endif
