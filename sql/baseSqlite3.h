#ifndef BASESQLITE3_H
#define BASESQLITE3_H

#include "sqlite3.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// 打开数据库，成功返回数据库指针，失败返回NULL
sqlite3 *open_sqlite3_db(const char *db_name);
// 关闭数据库，成功返回0
int close_sqlite3_db(sqlite3 *db);

// 执行SQL语句，成功返回0，失败返回-1
int exec_sqlite3_sql(sqlite3 *db, const char *sql);
// 执行SQL语句，成功返回0，失败返回-1，回调函数用于处理查询结果
int exec_sqlite3_sql_callback(sqlite3 *db, const char *sql, int (*callback)(void *, int, char **, char **), void *data);

// 执行多条SQL语句，成功返回0，失败返回-1
int exec_sqlite3_sqls(sqlite3 *db, const char **sqls,int size);

// 开始事务，成功返回0，失败返回-1。
int begin_sqlite3_affair(sqlite3 *db);
// 结束事务，成功返回0，失败返回-1。
int end_sqlite3_affair(sqlite3 *db);

// 获取所有表名，成功返回0，失败返回-1。
int get_table_names(sqlite3 *db);
// 获取表信息，成功返回0，失败返回-1。
int get_table_info(sqlite3 *db, const char *table_name);
// 获取行数，成功返回0，失败返回-1。
int get_table_rowcount(sqlite3 *db, const char *table_name);

// 创建表，成功返回0，失败返回-1
int create_table(sqlite3 *db, const char *table_name, const char *columns);
// 删除表，成功返回0，失败返回-1
int drop_table(sqlite3 *db, const char *table_name);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BASESQLITE3_H