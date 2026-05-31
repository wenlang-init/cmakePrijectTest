#ifndef MENDIAN_H
#define MENDIAN_H

class MEndian
{
public:
    MEndian(){};

    static inline bool isBigEndian()
    {
        const static int m = 0x12345678;
        if(*(unsigned char*)&m == 0x12){
            return true;
        } else {
            return false;
        }

    }
    /*
     * 转为大端序数据
     * value:一般为16,32,64位的数据
     * */

    template <typename T> static inline T ToBigEndian(T value)
    {
        const static int m = 0x12345678;
        if(*(unsigned char*)&m == 0x12){
            return value;
        } else {
            int value_len = sizeof(T);
            T value_t;
            char *p = (char *)&value_t;
            char *q = (char *)&value;
            for(int i=0;i<value_len;i++){
                p[i] = q[value_len-1-i];
            }
            return value_t;
        }
    }
    /*
     * 转为小端序数据
     * value:一般为16,32,64位的数据
     * */
    template <typename T> static inline T ToLittleEndian(T value)
    {
        const static int m = 0x12345678;
        if(*(unsigned char*)&m == 0x12){
            int value_len = sizeof(T);
            T value_t;
            char *p = (char *)&value_t;
            char *q = (char *)&value;
            for(int i=0;i<value_len;i++){
                p[i] = q[value_len-1-i];
            }
            return value_t;
        } else {
            return value;
        }
    }
    /*
     * 将来自小端序的数据转为本机字节序
     * value:一般为16,32,64位的数据
     * */
    template <typename T> static inline T FromLittleEndian(T value)
    {
        const static int m = 0x12345678;
        if(*(unsigned char*)&m == 0x12){
            int value_len = sizeof(T);
            T value_t;
            char *p = (char *)&value_t;
            char *q = (char *)&value;
            for(int i=0;i<value_len;i++){
                p[i] = q[value_len-1-i];
            }
            return value_t;
        } else {
            return value;
        }
    }
    /*
     * 将来自大端序的数据转为本机字节序
     * value:一般为16,32,64位的数据
     * */
    template <typename T> static inline T FromBigEndian(T value)
    {
        const static int m = 0x12345678;
        if(*(unsigned char*)&m == 0x12){
            return value;
        } else {
            int value_len = sizeof(T);
            T value_t;
            char *p = (char *)&value_t;
            char *q = (char *)&value;
            for(int i=0;i<value_len;i++){
                p[i] = q[value_len-1-i];
            }
            return value_t;
        }
    }
};

#endif // MENDIAN_H
