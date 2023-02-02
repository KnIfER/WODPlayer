#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "sqlite3.h"
#include <Duilib/Core/InsituDebug.h>
#include "database_helper.h"

WODBase::WODBase() 
{
    const char *sql;

    sqlite3 *db = NULL;
    int res = sqlite3_open("D:\\wodbase.db", &db);
    if (res != SQLITE_OK) {
        LogIs(L"打开失败!\n ERR: %s\n", sqlite3_errmsg(db));
        // todo throw???
    }

    sql = "create table if not exists timemarks(\
id INTEGER PRIMARY KEY AUTOINCREMENT\
, name TEXT\
, fname TEXT\
, path TEXT\
, vid INTEGER\
, pos INTEGER\
, duration INTEGER\
, opt INTEGER\
, layer INTEGER\
, color INTEGER\
, creation_time INTEGER NOT NULL\
, thumbnail BLOB\
        )";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
}



void sqlite3_test_get_table(sqlite3 *db)
{
    char **rows = NULL;
    int nrow = 0, ncolumn = 0;
    int res = sqlite3_get_table(db, "select id, lex from history order by creation_time", &rows, &nrow, &ncolumn, NULL);
    if (res != SQLITE_OK) {
        char err[1024] = { 0 };
        snprintf(err, sizeof err, sqlite3_errmsg(db));
        LogIs("sqlite3_prepare_v2 error: %s\n", err);
        return;
    }
    LogIs("获取 到%d条数据，列数量为: %d\n", nrow, ncolumn);

    for (int i = 0; i <= nrow; i++) {
        for (int j = 0; j < ncolumn; j++) {
            LogIs("%s\t", rows[i * ncolumn + j]);
        }
        LogIs("\n");
    }
    sqlite3_free_table(rows);
}

void sqlite3_test_update(sqlite3 *db)
{
    sqlite3_exec(db, "begin", NULL, NULL, NULL);
    sqlite3_stmt *stmt = NULL;
    const char *sql = "select id,name,age from stu order by id asc";
    const char *err = NULL;
    int res = sqlite3_prepare_v2(db, sql, -1, &stmt, &err);
    assert(res == SQLITE_OK);
    LogIs("%s\n", sql);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2) + 1;
        LogIs("%d\t%s\t%d\n", id, name, age);

        char sql[1024];
        snprintf(sql, sizeof sql, "update stu set age = %d where id = %d;", age, id);
        res = sqlite3_exec(db, sql, NULL, NULL, NULL);
        assert(res == SQLITE_OK);
    }
    sqlite3_exec(db, "commit", NULL, NULL, NULL);

    sqlite3_finalize(stmt);
}

void sqlite3_test_insert(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "insert into stu (name, age) values (?, ?)";
    const char *names[] = {"凡蕾", "幻灵", "紫安"};
    int ages[] = {15, 18, 23};
    int res = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (res != SQLITE_OK) {
        LogIs("sql prepare error: %s\n", sql);
        return;
    }
    sqlite3_exec(db, "begin", NULL, NULL, NULL);
    for (int i = 0; i < 3; i++) {
        sqlite3_bind_text(stmt, 1, names[i], -1, NULL);
        sqlite3_bind_int(stmt, 2, ages[i]);

        res = sqlite3_step(stmt);
        if (res != SQLITE_DONE) {
            LogIs("insert error. res = %d. name: %s age: %d\n", res, names[i], ages[i]);
            break;
        }
        LogIs("insert success. name: %s age: %d\n", names[i], ages[i]);

        sqlite3_reset(stmt);
    }
    sqlite3_exec(db, "commit", NULL, NULL, NULL);
    sqlite3_finalize(stmt);
}

int testSqlite()
{
    //LogIs("testSqlited\n");
    //sqlite3 *db = NULL;
    //int res = sqlite3_open("D:\\databaseV2.sql", &db);
    //if (res != SQLITE_OK) {
    //    LogIs("open database sqlite3test.db fail!\n ERR: %s\n", sqlite3_errmsg(db));
    //    return -1;
    //}
    //LogIs("open database sqlite3test.db ok!\n");

    //sqlite3_test_get_table(db);

    //sqlite3_close(db);
    return 0;
}
