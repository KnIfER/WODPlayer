
#pragma once

#include <vector>
#include "bookmark.h"

class sqlite3;

class WODBase{
public:
	WODBase();
	~WODBase(){};

	bool Init();
	
	int AddBookmark(const char* fullpath, char* markName, __int64 & folderId, int pos, int duration, int flag);
	
	__int64 GetBookMarks(const char* fullpath, std::vector<BookMark> & _bookmarks);

private:
	sqlite3* db;

};