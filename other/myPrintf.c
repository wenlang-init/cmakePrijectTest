#include "myPrintf.h"

#include <stdarg.h>
// int __vsnprintf(out_fct_type out, char* buffer, const unsigned int maxlen, const char* format, va_list va);
signed int rt_vsnprintf(char *buf, unsigned int size, const char *fmt, va_list args);

int printFormat(const char *format,...)
{
    va_list arg;
    va_start(arg, format);
    va_start (arg, format);
    int length = rt_vsnprintf(printf_info,PRINT_BUFFER_SIZE,format,arg);
    va_end (arg);
    if(length > 0){
        //printf("%s",printf_info);
        //HAL_UART_Transmit(&huart1, (uint8_t *) printf_string, len, HAL_MAX_DELAY);
    }
    if(length > PRINT_BUFFER_SIZE){
        length = PRINT_BUFFER_SIZE-1;
    }
    return length;
}

int printFormatBuffer(char *buffer, unsigned int bufferSize, const char *format,...)
{
    va_list arg;
    va_start(arg, format);
    va_start (arg, format);
    int length = rt_vsnprintf(buffer, bufferSize, format, arg);
    va_end (arg);
    if(length > 0){
        //printf("%s",printf_info);
        //HAL_UART_Transmit(&huart1, (uint8_t *) printf_string, len, HAL_MAX_DELAY);
    }
    if(length > bufferSize){
        length = bufferSize-1;
    }
    return length;
}