#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "baseSqlite3.h"
#include "lognone.h"

sqlite3* open_sqlite3_db(const char *db_name)
{
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        DEBUG_LOG("Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

int close_sqlite3_db(sqlite3 *db)
{
    if (db != NULL) {
        sqlite3_close(db);
        return 0;
    }
    return -1;
}

int exec_sqlite3_sql(sqlite3 *db, const char *sql)
{
    char *zErrMsg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n%s\n", zErrMsg, sql);
        sqlite3_free(zErrMsg);
        return -1;
    }
    return 0;
}

int exec_sqlite3_sql_callback(sqlite3 *db, const char *sql, int (*callback)(void *, int, char **, char **), void *data)
{
    char *zErrMsg = NULL;
    int rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    return 0;
}

int exec_sqlite3_sqls(sqlite3 *db, const char **sqls,int size)
{
    char *zErrMsg = NULL;
#if 0
    char cmds[102400];
    int len = sprintf(cmds,"BEGIN;");
    for(int i=0;i<size;i++ ) {
        len += sprintf(cmds+len,"%s;",sqls[i]);
    }
    len += sprintf(cmds+len,"COMMIT;");
    DEBUG_LOG("cmds:%s\n",cmds);
    int rc = sqlite3_exec(db,cmds,0,0,&zErrMsg);//执行sql语句
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    return 0;
#else
    int rc = sqlite3_exec(db,"BEGIN;",0,0,&zErrMsg);//开始一个事务
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    for(int i=0;i<size;i++ ) {
        rc = sqlite3_exec(db,sqls[i],0,0,&zErrMsg);//执行sql语句
        if (rc!= SQLITE_OK) {
            DEBUG_LOG("SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            // rc = sqlite3_exec(db,"rollback;",0,0,&zErrMsg);//回滚（撤销）
            // if (rc!= SQLITE_OK) {
            //     DEBUG_LOG("SQL error: %s\n", zErrMsg);
            //     sqlite3_free(zErrMsg);
            // }
        }
    }
    rc = sqlite3_exec(db,"COMMIT;",0,0,&zErrMsg);//提交
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    return 0;
    #endif
}

int begin_sqlite3_affair(sqlite3 *db)
{
    char *zErrMsg = NULL;
    int rc = sqlite3_exec(db,"BEGIN;",0,0,&zErrMsg);//开始一个事务
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    return 0;
}
int end_sqlite3_affair(sqlite3 *db)
{
    char *zErrMsg = NULL;
    int rc = sqlite3_exec(db,"COMMIT;",0,0,&zErrMsg);//提交
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    return 0;
}

int get_table_names(sqlite3 *db)
{
    char *zErrMsg = NULL;
    char **pazResult;
    int table_count;
    int column_count;
    int rc = sqlite3_get_table(db, "SELECT name FROM sqlite_master WHERE type='table';", &pazResult, &table_count, &column_count,&zErrMsg);
    //rc = sqlite3_exec(db, "SELECT name FROM sqlite_master WHERE type='table';",[](void *, int, char **, char **){}, &pazResult, &zErrMsg);
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    DEBUG_LOG("table_count:%d,column_count:%d\n",table_count,column_count);
    for (int i =0; i <= table_count; i++) { // 第0行为列名
        for (int j = 0; j < column_count; j++) {
            printf("%-5s\t", pazResult[i * column_count + j]);
        }
        printf("\n");
    }
    sqlite3_free_table(pazResult);
    return 0;
}

int get_table_info(sqlite3 *db, const char *table_name)
{
    char *zErrMsg = NULL;
    char **pazResult;
    int column_count;
    int row_count;
    char sql[100];
    snprintf(sql, sizeof(sql), "PRAGMA table_info('%s');", table_name);
    int rc = sqlite3_get_table(db, sql, &pazResult,&row_count, &column_count, &zErrMsg);    
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    DEBUG_LOG("row_count:%d,column_count:%d\n",row_count,column_count);
    for (int i =0; i <= row_count; i++) { // 第0行为列名
        for (int j = 0; j < column_count; j++) {
            printf("%-20s\t", pazResult[i * column_count + j]);
        }
        printf("\n");
    }
    sqlite3_free_table(pazResult);
    return 0;
}

int get_table_rowcount(sqlite3 *db, const char *table_name)
{
    char *zErrMsg = NULL;
    char **pazResult;
    int column_count;
    int row_count;
    char sql[100];
    snprintf(sql, sizeof(sql), "select count(*) from %s;", table_name);
    int rc = sqlite3_get_table(db, sql, &pazResult,&row_count, &column_count, &zErrMsg);    
    if (rc!= SQLITE_OK) {
        DEBUG_LOG("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    DEBUG_LOG("row_count:%d,column_count:%d\n",row_count,column_count);
    for (int i =0; i <= row_count; i++) { // 第0行为列名
        for (int j = 0; j < column_count; j++) {
            printf("%-20s\t", pazResult[i * column_count + j]);
        }
        printf("\n");
    }
    sqlite3_free_table(pazResult);
    return 0;
}

int create_table(sqlite3 *db, const char *table_name, const char *columns)
{
    int len = strlen(columns) + 100;
    char *sql = (char *)malloc(len);
    //snprintf(sql, len, "CREATE TABLE IF NOT EXISTS %s (id integer primary key,%s);", table_name, columns);
    snprintf(sql, len, "CREATE TABLE IF NOT EXISTS %s (%s);", table_name, columns);
    INFO_LOG("sql=%s\n",sql);
    int rc = exec_sqlite3_sql(db, sql);
    free(sql);
    if (rc!= 0) {
        return -1;
    }
    return 0;
}

int drop_table(sqlite3 *db, const char *table_name)
{
    char sql[4096];
    snprintf(sql, sizeof(sql), "DROP TABLE IF EXISTS %s;", table_name);
    int rc = exec_sqlite3_sql(db, sql);
    if (rc!= 0) {
        return -1;
    }
    return 0;
}
