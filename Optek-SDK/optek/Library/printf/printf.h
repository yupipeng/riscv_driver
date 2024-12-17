#ifndef _PRINTF_H_
#define _PRINTF_H_
#include "c_def.h"
#include <stdarg.h>
// #include "fifo.h"

static uint32_t mysprintf(char *buffer, uint32_t size, const char *fmt, va_list args);
int	myprintf( const char *format, ... );
static int outputb( const char *buf, int size );

#endif