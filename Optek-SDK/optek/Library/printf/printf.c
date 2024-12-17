#include <stdio.h>
// #include <stdarg.h>
#include <string.h>
#include "printf.h"
#include "hw_uart.h"
#include "fifo.h"
#include "sdk_com.h"
#include "debug.h"





#define	PRINTF_MAX_STRING  256*4

#define SEND_BUFFER_SIZE 128

#define TX_FIFO_SIZE    1024*4

static U8 txFifoBuffer[TX_FIFO_SIZE] __PRINTF_BSS;

enum{
    enumPRINTF_STATUS_IDLE,
    enumPRINTF_STATUS_TRANSMITING
};

#define MAGIC_START 0X5555AAAA
#define MAGIC_END   0X12345678

struct __attribute__((aligned(4))) {
    U32 magic_start;
    /// pointer to External interface api
    const extif_api_stru* ext_if;
    /// store string to ber send
    U8 stringBuffer[PRINTF_MAX_STRING];
    /// holds the string being send
    U8 sendBuffer[SEND_BUFFER_SIZE];

    /// queue for printf stream
    FIFO printf_fifo;
    U16 tx_len;
    /// status of printf module
    U8 status;
    U8 overflow_flag;// 1 means overflow

#ifdef RTOS_ENABLE
    SemaphoreHandle_t mutex_put_data;
#endif
    U32 magic_end;
} printf_env __PRINTF_BSS;


/// States values
enum
{
    S_COPY,         // Initial state; copy chars of the format str
    S_PERCENT,      // just read '%'
    S_FLAGS,        // just read flag character
    S_WIDTH,        // just read width specifier
    S_DOT,          // just read '.'
    S_PRECIS,       // just read field_precision specifier
    S_SIZE,         // just read size specifier
    S_TYPE,         // just read type specifier
    S_MAX
};

/// character type values
enum
{
    C_OTHER,        // character with no special meaning
    C_PERCENT,      // '%'
    C_DOT,          // '.'
    C_ZERO,         // '0'
    C_DIGIT,        // '1'..'9'
    C_FLAG,         // ' ', '+', '-',
    C_SIZE,         // 'h', 'l', 'L'
    C_TYPE,         // type specifying character
    C_MAX
};


/// field_flags used to store the format information
enum
{
    FLAG_SHORT      = (1 << 0),   // short value
    FLAG_LONG       = (1 << 1),   // long value
    FLAG_SIGNED     = (1 << 2),   // signed value
    FLAG_SIGN       = (1 << 3),   // Add a - or + in the front of the field
    FLAG_SIGN_SPACE = (1 << 4),   // Add a space or - in the front of the field
    FLAG_LEFT       = (1 << 5),   // left justify
    FLAG_LEAD_ZERO  = (1 << 6),   // padding with 0
    FLAG_NEGATIVE   = (1 << 7),   // the value is negative
};


/// Transition table
static const uint8_t transition_table[S_MAX][C_MAX] =
{   //              OTHER       PERCENT     DOT         ZERO        DIGIT       FLAG        SIZE        TYPE
/*  S_COPY    */  { S_COPY,     S_PERCENT,  S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_COPY  },
/*  S_PERCENT */  { S_COPY,     S_COPY,     S_DOT,      S_FLAGS ,   S_WIDTH,    S_FLAGS ,   S_SIZE,     S_TYPE  },
/*  S_FLAGS   */  { S_COPY,     S_COPY,     S_DOT,      S_FLAGS,    S_WIDTH,    S_FLAGS,    S_SIZE,     S_TYPE  },
/*  S_WIDTH   */  { S_COPY,     S_COPY,     S_DOT,      S_WIDTH,    S_WIDTH,    S_COPY,     S_SIZE,     S_TYPE  },
/*  S_DOT     */  { S_COPY,     S_COPY,     S_COPY,     S_PRECIS,   S_PRECIS,   S_COPY,     S_SIZE,     S_TYPE  },
/*  S_PRECIS  */  { S_COPY,     S_COPY,     S_COPY,     S_PRECIS,   S_PRECIS,   S_COPY,     S_SIZE,     S_TYPE  },
/*  S_SIZE    */  { S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_TYPE  },
/*  S_TYPE    */  { S_COPY,     S_PERCENT,  S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_COPY,     S_COPY  },
};

/* Upper case hexadecimal table */
static const char hex_upper_table[] = "0123456789ABCDEF";
#ifdef SUPPORT_LOWER_CASE_HEX
/* Lower case hexadecimal table */
static const char hex_lower_table[] = "0123456789abcdef";
#endif


static uint32_t co_min(uint32_t a,uint32_t b)
{
    return a < b ? a:b;
}


/**
 ****************************************************************************************
 * @brief Function to read a particular character and map its type
 *
 * This function is called to read a particular character and fetch its type
 *
 * @param[in] c Input character
 *
 * @return Type of the character
 ****************************************************************************************
 */
static uint32_t type_get(char c)
{
    uint32_t res;

    switch(c)
    {
    case '%':
        res =  C_PERCENT;
        break;

    case '.':
        res =  C_DOT;
        break;

    case '0':
        res =  C_ZERO;
        break;

    case ' ':
    case '+':
    case '-':
        res =  C_FLAG;
        break;

    case 'h':
    case 'l':
        res =  C_SIZE;
        break;

    case 'x':
    case 'X':
    case 'd':
    case 'b':
    case 'i':
    case 'c':
    case 'u':
    case 's':
    case 'm':
    case 'M':
    case 'a':
    case 'A':
        res =  C_TYPE;
        break;

    case '*':
        res =  C_DIGIT;
        break;

    default:
        if(('1' <= c) && (c <= '9'))
        {
            res =  C_DIGIT;
        }
        else
        {
            res =  C_OTHER;
        }
        break;
    }

    return res;
}


int	mysnprintf(char* const Buffer, unsigned int buf_size, const char *format, ... )
{
	int len;
	va_list vargs;

	va_start( vargs, format );
	len = mysprintf( (char *)Buffer, PRINTF_MAX_STRING, format, vargs );
	va_end( vargs );
	
	return len;
}

static void printf_callback(uint32_t param, uint8_t status) __INTERNAL_RAM_TEXT;
static void printf_callback(uint32_t param, uint8_t status)
{
    U16 dlen;
    dlen = fifo_get_data_len(&printf_env.printf_fifo);

    if (dlen)
    {
        if (dlen > SEND_BUFFER_SIZE)
            dlen = SEND_BUFFER_SIZE;

        fifo_get_data(&printf_env.printf_fifo, printf_env.sendBuffer, dlen);
        printf_env.tx_len = dlen;
        printf_env.ext_if->write(printf_env.sendBuffer, dlen, printf_callback, 0);
    }
    else
    {
        printf_env.status = enumPRINTF_STATUS_IDLE;        
    }    
}

void printf_init(const extif_api_stru* ext_if)
{
    int len;

    printf_env.ext_if = ext_if;
    printf_env.overflow_flag = 0;
    
#ifdef RTOS_ENABLE
    printf_env.mutex_put_data = xSemaphoreCreateMutex();
#endif

    if (printf_env.magic_start != MAGIC_START || printf_env.magic_end != MAGIC_END || 
    printf_env.printf_fifo.fifo_start_addr != txFifoBuffer || 
    printf_env.printf_fifo.fifo_end_addr != txFifoBuffer + sizeof(txFifoBuffer) || 
    printf_env.printf_fifo.pRead < txFifoBuffer ||
    printf_env.printf_fifo.pRead > txFifoBuffer + sizeof(txFifoBuffer) ||
    printf_env.printf_fifo.pWrite < txFifoBuffer ||
    printf_env.printf_fifo.pWrite > txFifoBuffer + sizeof(txFifoBuffer) ||
    (printf_env.tx_len > sizeof(printf_env.sendBuffer))
    )
    {// hard reset
        printf_env.magic_start = MAGIC_START;
        printf_env.magic_end = MAGIC_END;
        fifo_init (&printf_env.printf_fifo, txFifoBuffer, TX_FIFO_SIZE,TRUE);
        printf_env.status = enumPRINTF_STATUS_IDLE;
        printf_env.tx_len = 0;

        printf_env.sendBuffer[0] = 0xff;//clear the peer status to idle
        len = mysnprintf(   &printf_env.sendBuffer[1], 
                            sizeof(printf_env.sendBuffer - 1), 
                            "\n\n Hard reset!!!\r\n\n");
        printf_env.status = enumPRINTF_STATUS_TRANSMITING;
        printf_env.ext_if->write(printf_env.sendBuffer, len + 1, printf_callback, 0);
    }
    else
    {//software reset, maybe watdog reset
        if (fifo_get_free_space (&printf_env.printf_fifo) >= printf_env.tx_len)
        {
            fifo_put_data (&printf_env.printf_fifo, printf_env.sendBuffer, printf_env.tx_len);
        }


        printf_env.sendBuffer[0] = 0xff;//clear the peer status to idle
        len = mysnprintf(   &printf_env.sendBuffer[1], sizeof(printf_env.sendBuffer - 1), 
                            "\n\n Soft reset!!! %d chars left, \
                            debug info:%d, %d, %d, %d, %d, 0x%x, 0x%x\r\n\n", 
                            fifo_get_data_len(&printf_env.printf_fifo), 
                            debug_info.debug_info1, debug_info.debug_info2, 
                            debug_info.debug_info3, 
                            debug_info.debug_info4, 
                            debug_info.debug_info5, 
                            debug_info.last_printf_tick, 
                            debug_info.last_watchdag_reset_tick );
        printf_env.status = enumPRINTF_STATUS_TRANSMITING;
        printf_env.ext_if->write(printf_env.sendBuffer, len + 1, printf_callback, 0);
    }

    memset(&debug_info, 0, sizeof(debug_info));
}




/**
 ****************************************************************************************
 * @brief Execute a pseudo vsnprintf function
 *
 * @param[out] buffer  Output buffer
 * @param[in]  size    Size of the output buffer
 * @param[in]  fmt     Format string
 * @param[in]  args    Variable list of arguments
 *
 * @return The number of bytes copied into the buffer
 ****************************************************************************************
 */
static uint32_t mysprintf(char *buffer, uint32_t size, const char *fmt, va_list args)
{
    uint32_t    state_current = S_COPY;  // Initial state
    char        c;

    char        *fmt_field_ptr = NULL;
    int         fmt_field_size = 0;

    char        buffer_tmp[64];
    char        *buffer_init = buffer;     // Store the init buffer to compute the length

    int         field_width = 0;
    char        field_flags = 0;
    int         field_precision = 0;
    int         field_padding = 0;

    int32_t     value;
    char        *tmp_ptr = NULL;

    size--;                             // reserve space for the tailling '\0'

    // For each char in format string
    while (((c = *fmt++) != 0) && (size>0))
    {
        state_current =  transition_table[state_current][type_get(c)];

        switch (state_current)
        {
        case S_COPY  :
            // Copy character to the output
            *buffer++ = c;
            size--;
            break;

        case S_PERCENT:
            // Assign default value for the conversion parameters
            field_width = 0;
            field_flags = 0;
            field_precision = size;
            break;

        case S_FLAGS:
            // set flag based on which flag character
            switch(c)
            {
            case '-':
                // left justify
                field_flags |= FLAG_LEFT;
                break;
            case '+':
                // force sign indicator
                field_flags |= FLAG_SIGN;
                break;
            case ' ':
                // force sign or space
                field_flags |= FLAG_SIGN_SPACE;
                break;
            case '0':
                // pad with leading 0
                field_flags |= FLAG_LEAD_ZERO;
                break;
            default:
                DBG_Assert(0);
                break;
            }
            break;

        case S_WIDTH:
            if (c != '*')
            {
                // add digit to current field width
                field_width = field_width * 10 + (c - '0');
            }
            else
            {
                field_width = (int)va_arg(args, int);
            }
            break;

        case S_DOT:
            // Clear the field_precision variable (i.e. do not use default field_precision)
            field_precision = 0;
            break;

        case S_PRECIS:
            if (c != '*')
            {
                // Add digit to field_precision variable
                field_precision = field_precision * 10 + (c - '0');
            }
            else
            {
                field_precision = (int)va_arg(args, int);
            }
            break;

        case S_SIZE:
            // currently ignored
            switch (c)
            {
            case 'l':
                // 'l' => long int
                field_flags |= FLAG_LONG;
                break;

            #ifdef FLAG_SHORT_SUPPORTED
            case 'h':
                // 'h' => short int
                field_flags |= FLAG_SHORT;
                break;
            #endif // FLAG_SHORT_SUPPORTED

            default:
                DBG_Assert(0);
                break;
            }
            break;

        case S_TYPE:

            // Now the options have been decoded
            switch (c)
            {
                // c
                case 'c':
                    // Store byte in Tx buffer
                    buffer_tmp[0] = (char)va_arg(args, int);
                    fmt_field_ptr = buffer_tmp;
                    fmt_field_size = 1;
                    break;


                // String
                case 's':
                    // Read parameter (pointer on string)
                    fmt_field_ptr =  va_arg(args, char *);
                    fmt_field_size = 0;
                    if (fmt_field_ptr != NULL)
                    {
                        // Compute the length of the string
                        tmp_ptr =  fmt_field_ptr;
                        while ((field_precision > fmt_field_size) && (*tmp_ptr != '\0'))
                        {
                            tmp_ptr++;
                            fmt_field_size++;
                        }
                    }
                    break;

                // MAC address
                case 'm':
                case 'M':
                    {
                    int i;
                    fmt_field_ptr   = buffer_tmp;
                    tmp_ptr         = va_arg(args, char *);
                    fmt_field_size  = 17;
                    for(i = 5;;)
                    {
                        value = (unsigned char) *tmp_ptr++;
                        *fmt_field_ptr++ = hex_upper_table[value >> 4];
                        *fmt_field_ptr++ = hex_upper_table[value & 0xF];
                        if (i-- == 0) break;
                        *fmt_field_ptr++ = ':';
                    }
                    fmt_field_ptr = buffer_tmp;
                    break;
                    }

                case 'a':
                case 'A':
                    fmt_field_ptr = buffer_tmp;
                    tmp_ptr = va_arg(args, char *);
                    // prevent overflow
                    field_width = co_min(field_width, sizeof(buffer_tmp)/3);
                    // if no width given
                    if (!field_width) field_width = 16;
                    fmt_field_size = field_width * 3 - 1 ;
                    for(;;)
                    {
                        value = (unsigned char) *tmp_ptr++;
                        *fmt_field_ptr++ = hex_upper_table[value >> 4];
                        *fmt_field_ptr++ = hex_upper_table[value & 0xF];
                        if (--field_width == 0) break;
                        // sep . (or : on align)
                        if (3 & (uint32_t) tmp_ptr) *fmt_field_ptr++ = '.';
                        else *fmt_field_ptr++ = ':';
                    }
                    fmt_field_ptr = buffer_tmp;
                    break;

                case 'i':   // signed decimal
                    c = 'd';
                case 'd':   // signed decimal
                case 'u':   // unsigned
                case 'X':   // hexa
                case 'x':   // hexa
                case 'b':   // binary

                    // Point to the last byte of the buffer (go backward during conversion)
                    fmt_field_ptr = buffer_tmp + sizeof(buffer_tmp);
                    fmt_field_size = 0;

                    // Get the value
                    if (field_flags & FLAG_LONG)
                    {
                        // long
                        value = va_arg(args, uint32_t);
                    }
                    #ifdef FLAG_SHORT_SUPPORTED
                    else if(field_flags & FLAG_SHORT)
                    {
                        if (c == 'd')
                        {
                            // extend the sign
                            value = (int16_t) va_arg(args, int);
                        }
                        else
                        {
                            value = (uint16_t) va_arg(args, int);
                        }
                    }
                    #endif // FLAG_SHORT_SUPPORTED
                    else
                    {
                        // int
                        // extend the sign
                        value = va_arg(args, int);
                    }

                    switch (c)
                    {
                    case 'd':
                            // Separate the sign to display it before the number
                            if (value < 0)
                            {
                                value =  (uint32_t)(-value);
                                // remember negative sign
                                field_flags |= FLAG_NEGATIVE;
                            }

                    case 'u':
                            // field_precision is the minimun number of digit to be display
                            // if value is 0 we don't need to display more chars
                            do
                            {
                                // go backward
                                fmt_field_ptr--;
                                *fmt_field_ptr = ('0'+ ((char) (value % 10)));
                                value = value / 10;
                                fmt_field_size++;
                            }
                            while ((field_precision > 0) && (value != 0));

                            // Add the sign
                            if (field_flags & FLAG_NEGATIVE)
                            {   // prefix with a '-'
                                // go backward
                                fmt_field_ptr--;

                                *fmt_field_ptr = '-';
                                fmt_field_size++;
                            }
                            else if (field_flags & FLAG_SIGN)
                            {   // prefix with a '+' (sign is forced)
                                // go backward
                                fmt_field_ptr--;
                                *fmt_field_ptr = '+';
                                fmt_field_size++;
                            }
                            else if (field_flags & FLAG_SIGN_SPACE)
                            {   // prefix with a ' ' (used instead of '+')
                                // go backward
                                fmt_field_ptr--;
                                *fmt_field_ptr = ' ';
                                fmt_field_size++;
                            }
                            break;

                    case 'x':
                        #ifdef SUPPORT_LOWER_CASE_HEX
                            do
                            {
                                // go backward
                                fmt_field_ptr--;
                                *fmt_field_ptr = hex_lower_table[value & 0XFUL];
                                value = ((uint32_t) value) >> 4;
                                fmt_field_size++;
                                field_precision--;
                            }
                            while ((field_precision > 0) && (value != 0));
                            break;
                        #endif

                    case 'X':
                            do
                            {
                                // go backward
                                fmt_field_ptr--;
                                *fmt_field_ptr = hex_upper_table[value & 0XFUL];
                                value = ((uint32_t) value) >> 4;
                                fmt_field_size++;
                                field_precision--;
                            }
                            while ((field_precision > 0) && (value != 0));
                            break;

                    case 'b':
                            do
                            {
                                // go backward
                                fmt_field_ptr--;
                                *fmt_field_ptr = '0' + ((char) (value & 0x01UL));
                                value = ((uint32_t) value) >> 1;
                                fmt_field_size++;
                                field_precision--;
                            }
                            while ((field_precision > 0) && (value != 0));
                            break;
                    default:
                        break;
                    } // embedded switch type

                    default:
                        break;
            } // switch type


            // Add padding
            field_padding = field_width - fmt_field_size;

            // put out the padding, prefix, and text, in the correct order
            if (field_flags & FLAG_LEAD_ZERO)
            {
                // Add leading zeros
                while ((size > 0) && (field_padding > 0))
                {
                    *buffer++ = '0';
                    size--;
                    field_padding--;
                }
            }
            else if (field_flags & FLAG_LEFT)
            {
                // Add blanks at the left to pad the field of the rigth
                while ((size > 0) && (field_padding > 0))
                {
                    *buffer++ = ' ';
                    size--;
                    field_padding--;
                }
            }

            // Copy the formated field
            while (fmt_field_size > 0)
            {
                *buffer++ = *fmt_field_ptr++;
                fmt_field_size--;
                size--;
            }

            if (field_width)
            {
                // Add blanks at the rigth
                while ((size > 0) && (field_padding > 0))
                {
                    *buffer++ = ' ';
                    size--;
                    field_padding--;
                }
            }
            break;

        default:
            DBG_Assert(0);
        } // switch state
    } // while

    // if we reach end of buffer, indicate it
    if (!size)
    {
        *(buffer-2) = '*';
        *(buffer-1) = '\n';
    }

    *buffer = '\0';

    // return string length
    return buffer - buffer_init;
}



int	myprintf( const char *format, ... )
{
	int len;
	va_list vargs;

	va_start( vargs, format );
	
	#ifdef PRINTF_OUT_DOUBLE_FOLAT_ENABLE
	len = vsnprintf( (char *)printf_env.stringBuffer, PRINTF_MAX_STRING, format, vargs );
	#else
	len = mysprintf(     (char *)printf_env.stringBuffer, 
                        PRINTF_MAX_STRING, 
                        format, vargs );
	#endif
	
	DBG_Assert( len <= PRINTF_MAX_STRING );
	va_end( vargs );

	len = outputb( printf_env.stringBuffer, len);

	return len;
}

static int outputb( const char *buf, int size )
{
        U32 len;   

    #ifdef RTOS_ENABLE
        xSemaphoreTake( printf_env.mutex_put_data, portMAX_DELAY);//write print fifo lock to protect share buffer of multi tasks
    #endif

        if (printf_env.status == enumPRINTF_STATUS_IDLE)
        {
            U32 send_s;

            if (size > SEND_BUFFER_SIZE)
                send_s = SEND_BUFFER_SIZE;
            else
                send_s = size;

            printf_env.status = enumPRINTF_STATUS_TRANSMITING;

            memcpy(printf_env.sendBuffer, buf, send_s);
            printf_env.tx_len = send_s;
            // printf_env.ext_if->write(printf_env.sendBuffer, send_s, printf_callback, 0);

            size -= send_s;
            buf += send_s;
        }
        
        if (size)
        {
            len = fifo_get_free_space (&printf_env.printf_fifo);

            if (len < size)
            {
                //missing some char???
                size = len;
                if (size >= 2)
                    size -= 2;
                else
                    size = 0;

                printf_env.overflow_flag = 1;
                fifo_put_data (&printf_env.printf_fifo, buf, size);
                fifo_put_data (&printf_env.printf_fifo, "*\n", 2);
            }
            else
            {
                fifo_put_data (&printf_env.printf_fifo, buf, size);
            }  
        }

    #ifdef RTOS_ENABLE
        xSemaphoreGive( printf_env.mutex_put_data );
    #endif

        return size;
    }