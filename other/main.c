
#include "myPrintf.h"
#include <stdio.h>

int main()
{
    char aa[10];char bb[10];
    aa[10] = 10;
    char buffer[4096];
    printFormatBuffer(buffer, sizeof(buffer), GREEN "%s:%d %s" RED "---" RESET,__FUNCTION__,__LINE__,__FILE__);
    printf("%s",buffer);
    printFormatBuffer(buffer, sizeof(buffer), "- hello world %d %lf\n", 123, 3.1415926);
    printf("%s",buffer);
    
    MY_PRINTF_BUFFER_INFO(buffer, sizeof(buffer));
    printf("%s",buffer);
    MY_PRINTF_BUFFER(buffer, sizeof(buffer),"0 hello world %d %lf\n", 123, 3.1415926);
    printf("%s",buffer);

    MY_PRINTF_BUFFER_DEBUG(buffer, sizeof(buffer), "1 hello world %d %lf\n", 123, 3.1415926);
    printf("%s",buffer);


    printFormat(GREEN "%s:%d %s" RED "---" RESET,__FUNCTION__,__LINE__,__FILE__);
    printf("%s",printf_info);
    printFormat("- hello world %d %lf\n", 123, 3.1415926);
    printf("%s",printf_info);

    MY_PRINTF_INFO();
    printf("%s",printf_info);
    MY_PRINTF("20 hello world %d\n", 123);
    printf("%s",printf_info);

    MY_PRINTF_DEBUG("21 hello world %d %lf\n", 123, 3.1415926);
    printf("%s",printf_info);

    return 0;
}
