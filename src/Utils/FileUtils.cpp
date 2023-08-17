#include "WindowBase.h"

int MoveToVacuum(PCZZWSTR path)
{
    SHFILEOPSTRUCT pm{};
    pm.wFunc = FO_DELETE;
    pm.pFrom = path;
    pm.pTo = NULL;
    pm.fFlags = FOF_NOCONFIRMATION | FOF_FILESONLY;
    //pm.fFlags = FOF_NOCONFIRMATION;
    return SHFileOperation(&pm);
}

int MoveToTrash(PCZZSTR path)
{
    SHFILEOPSTRUCTA fileOp;
    ZeroMemory(&fileOp, sizeof(SHFILEOPSTRUCT));
    fileOp.wFunc = FO_DELETE;
    fileOp.pFrom = path;
    fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT | FOF_FILESONLY;
    // fAnyOperationsAborted
    return SHFileOperationA(&fileOp);


    //char Buffer[2048+4];

    //strncpy_s (Buffer, 2048+4, path, 2048);
    //Buffer[strlen(Buffer)+1]=0;

    //SHFILEOPSTRUCTA s;
    //s.hwnd                  = NULL;
    //s.wFunc                 = FO_DELETE;
    //s.pFrom                 = Buffer;
    //s.pTo                   = NULL;
    //s.fFlags                = FOF_ALLOWUNDO | (/*bConfirm?0:*/FOF_NOCONFIRMATION) | FOF_SILENT|FOF_NOERRORUI;
    //s.fAnyOperationsAborted = false;
    //s.hNameMappings         = NULL;
    //s.lpszProgressTitle     = NULL;

    //int rc = SHFileOperationA(&s);

    //return rc;

}
