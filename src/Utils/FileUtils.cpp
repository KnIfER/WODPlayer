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

int MoveToTrash(PCZZWSTR path)
{
    SHFILEOPSTRUCT pm{};
    pm.wFunc = FO_DELETE;
    pm.pFrom = path;
    pm.pTo = NULL;
    pm.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_FILESONLY;
    return SHFileOperation(&pm);
}
