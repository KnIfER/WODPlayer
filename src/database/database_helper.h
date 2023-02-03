
class sqlite3;

class WODBase{
public:
	WODBase();
	~WODBase(){};

	bool Init();
	
	int AddBookmark(const char* fullpath, char* markName, long rowId, long pos, long duration, int flag);

private:
	sqlite3* db;

};