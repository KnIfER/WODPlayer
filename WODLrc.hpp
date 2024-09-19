#include "Utils\FU.h"


struct SubtitleNode{
    long from;
    long to;

    BOOL inited;
    QkString line;
    std::string tmp;
};


std::vector<SubtitleNode> subtitles;



LPCTSTR STR2Dec(LPCTSTR STR, int & value)
{
    //long val = _tcstol(STR, 0, 10);
    bool intOpened=false;
    int index=0;
    bool negative = false;
    value = -1;
    while(index<1024) {
        const TCHAR & intVal = *(STR);
        if (intVal>=L'0' && intVal<=L'9')
        {
            if(!intOpened) {
                value = 0;
                intOpened=true;
            }
            value = value*10+(intVal - L'0');
        }
        else if (intOpened || intVal==L'\0' || intVal==L':')
        {
            break;
        }
        STR++;
        index++;
    }
    return STR;
}


long ParseSrtTime(LPCTSTR STR) 
{
    LPCTSTR STR0 = STR;
    int hhmmss[3]{}, cc=0, haomiao=-1;
    long ret = 0;
    int tmp = -1;
    STR = STR2Dec(STR, tmp);
    if(tmp>=0) {
        hhmmss[cc++] = tmp;
        if(*STR==L':') {
            STR++;
            STR = STR2Dec(STR, tmp);
            if(tmp>=0) {
                hhmmss[cc] = tmp;
            }
            cc++;
        }
        if(*STR==L':') {
            STR++;
            STR = STR2Dec(STR, tmp);
            if(tmp>=0) {
                hhmmss[cc] = tmp;
            }
            cc++;
        }
        if(*STR==L',') {
            STR++;
            STR = STR2Dec(STR, haomiao);
        }
    }
    if(cc>=3) ret+=hhmmss[cc-3] * 3600000l;
    if(cc>=2) ret+=hhmmss[cc-2] * 60000l;
    if(cc>=1) ret+=hhmmss[cc-1] * 1000l;
    if(haomiao>=0) ret+=haomiao;
    //lxx(ParseSrtTime ss dd, STR0, ret);
    return ret;
}


int newNode = 1;
long from, to;
QkString subtitle;

void LoadSubtitle(const TCHAR* filePath) 
{
    subsCnt = 0;
    subtitles.resize(0);
    FU::BufferedLineReader reader(filePath);
    
    reader.readlines([](FU::BufferedLineReader* reader, CHAR* lineA, TCHAR* lineB, int length){
        QkString & line = reader->lineBuff;

        if(lineA) line = lineA;
        else if(lineB) line = lineB;
        //lxx(ss, line.GetData())

        line.Trim();
        if(line.IsEmpty()) 
        {
            if(newNode==2) 
            {        
                subtitles.push_back({from, to, 1, subtitle});
                //auto & item = subtitles;
            }
            newNode = 1;
        }
        else if(newNode==1) 
        {
            int idx = line.Find(L"-->");
            if(idx>=0) {
                auto data = STR(line);
                ((TCHAR*)data)[idx] = L'\0';
                from = ParseSrtTime(data);
                to = ParseSrtTime(data+idx+3);
                newNode = 2;
                subtitle = "";
            }
        }
        else if(newNode==2) 
        {
            if(!subtitle.IsEmpty())
                subtitle += "\n";
            subtitle += line;
        }
    });
    subsCnt = subtitles.size();

    //lzz(LoadSubtitle ss, STR(subtitles[1].line))
}

void PickAudio() {
    TCHAR filepath[MAX_PATH]{};

    // Build filter
    TCHAR formats[1024];
    lstrcpy(formats, TEXT(""));
    lstrcat(formats, TEXT("*.wav;*.wmv;*.wmp;*.wm;*.asf;*.wma;*.avi;*.mpg;*.mpeg;*.dat;"));
    lstrcat(formats, TEXT("*.mp3;*.aac;*.ts;*.mpa;*.mp2;*.vob;*.ifo;*.mid;*.ogm;*.ogg;*.cda;"));
    lstrcat(formats, TEXT("*.mp4;*.d2v;*.3gp;*.mkv;*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.mov;"));
    lstrcat(formats, TEXT("*.flv;*.qt;*.amr;*.mpc;*.swf;"));
    lstrcat(formats, TEXT("*.evo;*.tta;*.m4b;"));
    lstrcat(formats, TEXT("*.xv;*.xvx;*.xlmv"));

    TCHAR filters[1024];
    TCHAR * pFilter = filters;
    wsprintf(pFilter, TEXT("全部支持的媒体(%s)"), formats);
    pFilter += (lstrlen(pFilter) + 1);
    lstrcpy(pFilter, formats);
    pFilter += (lstrlen(pFilter) + 1);
    lstrcpy(pFilter, TEXT("所有文件(*.*)"));
    pFilter += (lstrlen(pFilter) + 1);
    lstrcpy(pFilter, TEXT("*.*"));
    pFilter += (lstrlen(pFilter) + 1);
    *pFilter = 0; pFilter++;

    BOOL ret = PickFileDlg(XPP->GetHWND(), FALSE, TEXT("选择音轨文件"), filters, filepath, MAX_PATH, NULL, 0);

    if(ret)
    {
        if (XPP->_audioPlayer.PlayVideoFile(filepath))
        {
            QkString filePath = filepath;
        }
    }
}