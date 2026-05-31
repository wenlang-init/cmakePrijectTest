#ifndef ZHTOPY_H
#define ZHTOPY_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 汉字转拼音
 *
 * @note 得到的字符串需要手动释放(free)
 * 
 * @param utf8zh: UTF-8 字符串
 * @return 拼音字符串，失败返回 NULL
 */
char *zh_to_py(const char *utf8zh);

/*
 * 汉字转拼音首字母
 *
 * @note 得到的字符串需要手动释放(free)
 *
 * @param utf8zh: UTF-8 字符串
 * @return 拼音首字母字符串，失败返回 NULL
 */
char *zh_to_jp(const char *utf8zh);

#ifdef __cplusplus
}
#endif

#endif // ZHTOPY_H
