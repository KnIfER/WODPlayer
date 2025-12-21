#pragma once


int getDesktopHeight() {
    RECT workArea;
    BOOL success = SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

    if (success)
    {
        return workArea.bottom - workArea.top;
    }
    return -1;
}