#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "sqlite3.h"
#include <Duilib/Core/InsituDebug.h>
#include "database_helper.h"

#include "shlwapi.h"

WODBase::WODBase() 
{
}

bool WODBase::Init() 
{
    const char *sql;

    int res = sqlite3_open("D:\\wodbase.db", &db);
    if (res != SQLITE_OK) {
        LogIs(L"打开失败!\n ERR: %s\n", sqlite3_errmsg(db));
        // todo throw???
    }

    sql = "create table if not exists timemarks(\
id INTEGER PRIMARY KEY AUTOINCREMENT\
, name TEXT DEFAULT NULL\
, fname TEXT DEFAULT NULL\
, path TEXT DEFAULT NULL\
, vid INTEGER DEFAULT 0\
, pos INTEGER\
, duration INTEGER DEFAULT 0\
, thumbnail INTEGER DEFAULT 0\
, opt INTEGER DEFAULT 0\
, param TEXT DEFAULT NULL\
, layer INTEGER DEFAULT 0\
, color INTEGER DEFAULT 0\
, creation_time INTEGER NOT NULL\
        )";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    return 0;
}

int WODBase::AddBookmark(const char* fullpath, char* markName, long rowId, long pos, long duration, int flag)
{
    //LogIs(2, "AddBookmark %s %s", fullpath?fullpath:"", markName?markName:"");
    sqlite3_exec(db, "begin", NULL, NULL, NULL);
    const char *sql = "INSERT INTO timemarks(name, fname, path, pos, duration, creation_time) VALUES(?, ?, ? ,?, ?, ?)";

    sqlite3_stmt *stmt = NULL;

    int succ = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (succ != SQLITE_OK) {
        LogIs("2, sql prepare error:  %d %s ", succ, sql);
        return -1;
    }

    char buffer_path[MAX_PATH];

    while(TRUE) {
        sqlite3_bind_text(stmt, 1, markName, -1, NULL); // 书签名
        
        ::PathCanonicalizeA(buffer_path, fullpath);
        ::PathRemoveFileSpecA(buffer_path);

        sqlite3_bind_text(stmt, 2, buffer_path+strlen(buffer_path)+1, -1, NULL); // 文件名
        sqlite3_bind_text(stmt, 3, buffer_path, -1, NULL); // 父目录
        //LogIs(2, "%s \n %s", buffer_path, buffer_path+strlen(buffer_path)+1);

        sqlite3_bind_int(stmt,  4, pos);
        sqlite3_bind_int(stmt,  5, duration);
        sqlite3_bind_int(stmt,  6, 0);

        succ = sqlite3_step(stmt);
        if (succ != SQLITE_DONE) {
            LogIs(2, "insert error. res = %d\n", succ);
            break;
        }

        sqlite3_reset(stmt);
        break;
    }
    sqlite3_exec(db, "commit", NULL, NULL, NULL);
    sqlite3_finalize(stmt);
    return -1;
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
