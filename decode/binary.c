#include <stdio.h>

// 二进制转换。将number转换为binaryNum进制数，结果存储在value中。
// number: 待转换的数字
// binaryNum: 目标进制数，范围为2-36
// value: 用于存储转换结果的数组，由调用者提供足够的内存空间
// return: 转换后的数字的位个数
int getBinary(int number,int binaryNum,int *value)
{
    if(binaryNum < 2 || binaryNum > 36){
        printf("Error: Binary number must be between 2 and 36.\n");
        return -1;
    }
    if(value == NULL) {
        printf("Error: values is NULL.\n");
        return -2;
    }

    int cnt = 0;
    int remainder = number%binaryNum;
    int val = number/binaryNum; 
    value[cnt++] = remainder;
    while(val!=0) {
        remainder = val%binaryNum;
        val = val/binaryNum;
        value[cnt++] = remainder;
    }
    
    return cnt;
}

void printBinary(int *numbers,int len,int binaryNum) {
    if(binaryNum < 2 || binaryNum > 36){
        printf("Error: Binary number must be between 2 and 36.\n");
        return;
    }
    int cnt;
    int value[100];

    for(int i=0;i<len;i++) {
        cnt = getBinary(numbers[i],binaryNum,value);
        if(cnt < 0){
            printf("Error: getBinary %d failed.\n",numbers[i]);
            continue;
        }
        printf("%d binary:",binaryNum); //print numbe
        for(int j=cnt-1;j>=0;j--) {
            if(value[j]>=10) {
                printf("%c", (char)(value[j]-10 +'A')); //print A~Z
            } else {
                printf("%d", value[j]); //print 0~9
            }
        }
        printf("\n");
    }

    return;
}

int testbinary() {
    int inputvalue;
    scanf("%d", &inputvalue);

    for(int i=2;i<=36;i++) {
        printBinary(&inputvalue,1,i);
    }
    return 0;
}
