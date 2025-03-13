#ifndef DEBUG_LIB_H_
#define DEBUG_LIB_H_

void hex_out(const char *buf_in, const size_t len);
void hex_out_p(const char *buf_in, const size_t len);
void print_hex(const char *in, const size_t len);
void dump_queue(const char * buf, const size_t in, const size_t out, const size_t size );

#endif /*DEBUG_LIB_H_*/
