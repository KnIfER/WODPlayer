
#pragma once

#include <vector>
#include "bookmark.h"

class sqlite3;

class WODBase{
public:
	WODBase();
	~WODBase();

	bool Init();
	
	int AddBookmark(const char* folder, const char* filename, char* markName, __int64 & folderId, int pos, int duration, int flag);
	int DelBookmark(__int64 rowId);
	
	__int64 GetBookMarks(const char* folder, const char* filename, std::vector<BookMark> & _bookmarks);

private:
	sqlite3* db;

};