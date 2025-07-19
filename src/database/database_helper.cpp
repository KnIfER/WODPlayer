#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <string>
#include "sqlite3.h"
#include <Duilib/Core/InsituDebug.h>
#include "database_helper.h"

#include "shlwapi.h"
#include "Utils\ProfileStd.h"


WODBase::WODBase() 
{
}

WODBase::~WODBase() 
{
    if(db) {
        sqlite3_close_v2(db);
        db = 0;
    }
}

bool WODBase::Init() 
{
    const char *sql;
    int res = -100;
    auto path = GetProfString("base");
    if (path && ::PathFileExistsA(path->c_str()))
    {
        res = sqlite3_open(path->c_str(), &db);
    }
    if (res != SQLITE_OK)
    {
        CHAR usrDir[MAX_PATH];
        ::GetModuleFileNameA(NULL, usrDir, MAX_PATH);
        ::PathRemoveFileSpecA(usrDir);
        ::PathAppendA(usrDir, "wodbase.db");
        res = sqlite3_open(usrDir, &db);
    }
    if (res != SQLITE_OK) {
        LogIs(L"打开失败!\n ERR: %s\n", sqlite3_errmsg(db));
        // todo throw???
    }

    sql = "CREATE TABLE IF NOT EXISTS timemarks(\
id INTEGER PRIMARY KEY AUTOINCREMENT\
, vid INTEGER DEFAULT 0\
, name TEXT DEFAULT NULL\
, fname TEXT DEFAULT NULL\
, fpath TEXT DEFAULT NULL\
, pos INTEGER\
, duration INTEGER DEFAULT 0\
, thumbnail INTEGER DEFAULT 0\
, opt INTEGER DEFAULT 0\
, param TEXT DEFAULT NULL\
, folder INTEGER DEFAULT 0\
, layer INTEGER DEFAULT 0\
, layerInfo TEXT DEFAULT NULL\
, color INTEGER DEFAULT 0\
, creation_time INTEGER NOT NULL\
        )";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    // 文件夹-位置-创建时间排序，按照此顺序收集，构建排序好的数组
    sqlite3_exec(db, "CREATE INDEX if not exists timemarks_index ON timemarks(vid, pos, creation_time, id)", NULL, NULL, NULL);
    // 打开新文件时，依照此索引查询得唯一vid，然后收集所有书签
    // query by |fname|-|fpath| where |folder|==1, fetch the unique vid as folder id, collect rows with the same vid in above order.
    // |pos| serve as bookmark counter when folder==1 (indicating folder);
    sqlite3_exec(db, "CREATE INDEX if not exists timemarks_folder_index ON timemarks(fname, fpath, id, pos) where folder==1", NULL, NULL, NULL);

    return 0;
}

int WODBase::AddBookmark(const char* folder, const char* filename, char* markName, __int64 & folderId, int pos, int duration, int flag, int layer)
{
    //LogIs(2, "AddBookmark %s %ld", fullpath, folderId);
    sqlite3_exec(db, "begin", NULL, NULL, NULL);
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt = NULL;
    if(folderId==-1) {
        const char *sql = "INSERT INTO timemarks(vid, fname, fpath, pos, duration, folder, creation_time) VALUES(?, ?, ?, ? ,?, ?, ?)";
        int succ = sqlite3_prepare_v2(db, sql, -1, &stmt1, NULL);
        if (succ != SQLITE_OK) {
            LogIs("2, sql prepare error:  %d %s ", succ, sql);
            return -1;
        }
        char buffer_path[MAX_PATH]{};
        while(TRUE) {
            int num = 1;
            sqlite3_bind_int(stmt1, num++, 0); // vid

            //PathCanonicalizeW(buffer_path, fullpath);
            //LogIs(2, "PathCanonicalizeA %s %ld", buffer_path, folderId);
            //::PathRemoveFileSpecA(buffer_path);
            //LogIs(2, "PathRemoveFileSpecA %s %ld", buffer_path, folderId);

            //size_t basePathLen = strlen(buffer_path);
            //if(basePathLen>0 && buffer_path[basePathLen-1]=='\\')
            //{
            //    basePathLen--;
            //    ::PathCanonicalizeA(buffer_path, fullpath);
            //    buffer_path[basePathLen]='\0';
            //}

            sqlite3_bind_text(stmt1, num++, filename, -1, NULL); // 文件名
            sqlite3_bind_text(stmt1, num++, folder, -1, NULL); // 父目录
            //LogIs(2, "%s \n %s", buffer_path, buffer_path+strlen(buffer_path)+1);

            sqlite3_bind_int(stmt1, num++, 0); // pos
            sqlite3_bind_int(stmt1, num++, duration); // duration
            sqlite3_bind_int(stmt1, num++, 1); // folder
            sqlite3_bind_int(stmt1, num++, GetTickCount64()); // creation_time
            //sqlite3_bind_int(stmt1, num++, layer); // layer

            succ = sqlite3_step(stmt1);
            if (succ != SQLITE_DONE) {
                LogIs(2, "folder insert error. res = %d\n", succ);
                break;
            }
            folderId = sqlite3_last_insert_rowid(db);
            sqlite3_reset(stmt1);
            break;
        }
    }
    if(folderId!=-1) {
        const char *sql = "INSERT INTO timemarks(vid, name, pos, layer, duration, creation_time) VALUES(?, ?, ?, ?, ?, ?)";
        int succ = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (succ != SQLITE_OK) {
            LogIs("2, sql prepare error:  %d %s ", succ, sql);
            return -1;
        }
        while(TRUE) {
            int num = 1;
            sqlite3_bind_int(stmt, num++, folderId); // vid
            sqlite3_bind_text(stmt, num++, markName, -1, NULL); // 书签名
            sqlite3_bind_int(stmt,  num++, pos);
            sqlite3_bind_int(stmt, num++, layer); // layer
            sqlite3_bind_int(stmt,  num++, duration);
            sqlite3_bind_int64(stmt,  num++, GetTickCount64());
            succ = sqlite3_step(stmt);
            if (succ != SQLITE_DONE) {
                LogIs(2, "insert error. res = %d\n", succ);
                break;
            }
            sqlite3_reset(stmt);
            break;
        }
    }

    sqlite3_exec(db, "commit", NULL, NULL, NULL);
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt);
    return sqlite3_last_insert_rowid(db);
}

int WODBase::DelBookmark(__int64 rowId)
{
    std::string localBuffer = "delete from timemarks where id=";
    localBuffer += std::to_string(rowId);
    sqlite3_exec(db, localBuffer.c_str(), NULL, NULL, NULL);
    return 0;
}

int WODBase::SetBookmarkColor(__int64 rowId, int color)
{
    std::string localBuffer = "UPDATE timemarks set color=";
    localBuffer += std::to_string(color);
    localBuffer += " where id=";
    localBuffer += std::to_string(rowId);
    sqlite3_exec(db, localBuffer.c_str(), NULL, NULL, NULL);
    return 0;
}

int exec_callback1(void *para, int columenCount, char **columnValue, char **columnName)
{
    int i;
    LogIs("exec_callback1=%s", columnValue[0]);

    *(__int64 *)para = atoi(columnValue[0]);

    return 0;
}

// todo atoi 精确度
int exec_callback2(void *para, int columenCount, char **columnValue, char **columnName)
{
    //((std::vector<BookMark>*)para)->push_back(BookMark{atoi(columnValue[0]), atoi(columnValue[1]), atoi(columnValue[2]), (int)4278190335 });
    ((std::vector<BookMark>*)para)->push_back(BookMark{atoi(columnValue[0]), atoi(columnValue[1]), atoi(columnValue[2]), (int)std::stoul(columnValue[3])});
    //((std::vector<BookMark>*)para)->push_back(BookMark{atoi(columnValue[0]), atoi(columnValue[1]), atoi(columnValue[2]), atoi(columnValue[3])});
    return 0;
}

__int64 WODBase::GetBookMarks(const char* folder, const char* filename, std::vector<BookMark>& _bookmarks)
{
    _bookmarks.clear();
    char buffer_path[MAX_PATH];
    std::string localBuffer;

    //::PathCanonicalizeA(buffer_path, fullpath);
    //::PathRemoveFileSpecA(buffer_path);

    //size_t basePathLen = strlen(buffer_path);
    //if(basePathLen>0 && buffer_path[basePathLen-1]=='\\')
    //{
    //    basePathLen--;
    //    ::PathCanonicalizeA(buffer_path, fullpath);
    //    buffer_path[basePathLen]='\0';
    //}

    // 首先查询得 folder vid
    localBuffer = "select id from timemarks where folder=1 and fname=\"";
    localBuffer += filename;
    localBuffer += "\"";
    //localBuffer += " and fpath=\"";
    //localBuffer += folder;
    //localBuffer += "\"";
    localBuffer += " limit 1";
    //LogIs("\nsql=%s", localBuffer.c_str());
    __int64 folderVid = -1;
    char *errmsg = 0;
    int succ =  sqlite3_exec(db, localBuffer.c_str(), exec_callback1, &folderVid, &errmsg);

    LogIs("\t首先查询得 folder vid=%ld succ=%d msg=%d", folderVid, succ, errmsg?sqlite3_errmsg(db):"");

    if(folderVid>=0)
    {
        localBuffer = "select pos,id,layer,color from timemarks where vid=";
        localBuffer += std::to_string(folderVid);
        localBuffer += " and folder!=1";
        localBuffer += " order by pos asc, creation_time asc, id asc";
        LogIs("\tsql=%s", localBuffer.c_str());
        __int64 folderVid = -1;
        char *errmsg = 0;
        int succ =  sqlite3_exec(db, localBuffer.c_str(), exec_callback2, &_bookmarks, &errmsg);
        LogIs("\t收集了%d个书签 succ=%d msg=%s", _bookmarks.size(), succ, errmsg?errmsg:"");
        for (size_t i = 0; i < _bookmarks.size(); i++)
        {
            LogIs("\t书签=%d", _bookmarks[i].pos);
        }
    }


    return folderVid;
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
