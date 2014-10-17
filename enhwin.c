/**************************************

Dan Akselrod (c) 1995.

Sample program. Calls enhframe module to add enhanced features to
it's frame.

**************************************/
#define INCL_WIN
#define INCL_DOS
#include <os2.h>
#include "enhframe.h"

HAB hab;
MRESULT EXPENTRY EnhWindProc(HWND, unsigned long, MPARAM, MPARAM);


main() {
HMQ hmq;
QMSG qmsg;
HWND hwnd, hwndFrame;
unsigned long flCreate= \
                FCF_TITLEBAR | FCF_TASKLIST | FCF_SIZEBORDER |
                FCF_SHELLPOSITION | FCF_SYSMENU | FCF_MINMAX | FCF_AUTOICON;

        hab=WinInitialize(0);
        hmq=WinCreateMsgQueue(hab, 0);

        WinRegisterClass(hab, "PMEnhancements", (PFNWP) EnhWindProc, WS_SYNCPAINT, 0);
        hwndFrame=WinCreateStdWindow(
                HWND_DESKTOP,        //parent
                WS_VISIBLE | WS_SYNCPAINT,          //style
                &flCreate,           //create flags
                "PMEnhancements",    //class of client
                "Enhancements of a PM window",//title
                0,                   //Client style
                0,                   //resourses (in .EXE)
                1,                   //resourse id for the window
                &hwnd);

        EnhanceFrame(hwndFrame);

                while (WinGetMsg(hab, &qmsg, 0, 0, 0))
                        WinDispatchMsg(hab, &qmsg);
return 0;
}




MRESULT EXPENTRY EnhWindProc(HWND hwnd, unsigned long msg, MPARAM mp1, MPARAM mp2) {
HPS hps;
        switch (msg) {
        case WM_ERASEBACKGROUND:
           return (MRESULT)TRUE;
        case WM_SIZE:
           WinInvalidateRect(WinQueryWindow(hwnd, QW_PARENT), NULL, TRUE);
           return 0;
        case WM_PAINT: {
                   RECTL rectl;
                   POINTL pointl;
                   hps=WinBeginPaint(hwnd, NULLHANDLE, NULLHANDLE);
                   WinQueryWindowRect(hwnd, &rectl);

                   pointl.x=rectl.xLeft;pointl.y=rectl.yBottom;
                   GpiMove(hps, &pointl);
                   pointl.x=rectl.xRight;pointl.y=rectl.yTop;
                   GpiLine(hps, &pointl);

                   pointl.x=rectl.xRight;pointl.y=rectl.yBottom;
                   GpiMove(hps, &pointl);
                   pointl.x=rectl.xLeft;pointl.y=rectl.yTop;
                   GpiLine(hps, &pointl);

                   WinDrawText(hps,-1 ,"Cool PM Stuff", &rectl, CLR_BLACK, 0,
                                DT_CENTER | DT_VCENTER );
                   WinEndPaint(hps);
                   return 0;
           }
        }
        return WinDefWindowProc(hwnd, msg, mp1, mp2);
}
