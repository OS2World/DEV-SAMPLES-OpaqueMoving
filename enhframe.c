/**************************************

Dan Akselrod (c) 1995.

Enhanced frame module. This file has the functionality to subclass
any frame and add Opaqueness to it's move function.

**************************************/
#define INCL_WIN
#define INCL_DOS
#include <os2.h>

#include "enhframe.h"

static MRESULT EXPENTRY EnhFrameProc(HWND, unsigned long, MPARAM, MPARAM);
static MRESULT EXPENTRY (*OldFrameProc)(HWND, unsigned long, MPARAM, MPARAM);
static MRESULT EXPENTRY EnhBarProc(HWND, unsigned long, MPARAM, MPARAM);
static MRESULT EXPENTRY (*OldBarProc)(HWND, unsigned long, MPARAM, MPARAM);

int FrameMaximized=0;
HWND hwndEnhFrame;


int EnhanceFrame(HWND Frame) {
HWND titlebar;

        hwndEnhFrame=Frame;/* save in global var */

        titlebar=WinWindowFromID(hwndEnhFrame,FID_TITLEBAR);
        if (titlebar==NULLHANDLE)
                return ENHFRAMEERR_TITLEBAR_NOTEXIST;

        OldFrameProc=WinSubclassWindow(hwndEnhFrame,(PFNWP) EnhFrameProc);
        if (OldFrameProc==0L)
                return ENHFRAMEERR_FRAME_SUBCLASS;

        OldBarProc=WinSubclassWindow(titlebar,(PFNWP) EnhBarProc);
        if (OldBarProc==0L) {
                WinSubclassWindow(hwndEnhFrame,(PFNWP) OldFrameProc);/* restore old procedure */
                return ENHFRAMEERR_TITLEBAR_SUBCLASS;
        }

return 0;
}



MRESULT EXPENTRY EnhBarProc(HWND hwnd, unsigned long msg, MPARAM mp1, MPARAM mp2) {
static POINTL ptDiff;
/* difference bet. mouse PT and Frame's window lower-left corner.
   When the window is moved, the mouse pointer is supposed to stay in the
   same spot inside the window. This difference between the coords of
   The mouse and coords of the frame window is used in calculating new coords
   of the frame, when the mouse is moved.
 */
static BOOL IsMoving=0;/* true when button went down, but not up */
static BOOL IsMoved=0;/* true on first move. When the button goes up/down
                         w/o movement, a window ACTIVATE is preformed. */

        switch (msg) {
        case WM_BUTTON1DOWN:
        case WM_BUTTON2DOWN:
        case WM_BUTTON3DOWN: /* start the drag process on any of the button clicks */
           IsMoving=1;IsMoved=0; /* moving mode, but haven't moved yet */
           ptDiff.x=SHORT1FROMMP(mp1);
           ptDiff.y=SHORT2FROMMP(mp1);/* get the coords of the mouse inside the title wind. */
           WinMapWindowPoints(hwnd, hwndEnhFrame, &ptDiff, 1);/* conver to frame coords */
           WinSetCapture(HWND_DESKTOP, hwnd);/* now must receive all mouse input */
           return (MRESULT)1;
        case WM_MOUSEMOVE:
           /* when the window is maximized, any movements are disallowed */
           if (IsMoving && !FrameMaximized) { /* allowed to move */
                POINTL newPt;
                IsMoved=1;/* set flag that we've moved at least once */
                newPt.x=(SHORT)SHORT1FROMMP(mp1);
                newPt.y=(SHORT)SHORT2FROMMP(mp1);
                WinMapWindowPoints(hwnd, HWND_DESKTOP, &newPt, 1);/* absolute coords */
                WinSetWindowPos( hwndEnhFrame, NULLHANDLE, newPt.x-ptDiff.x,
                                             newPt.y-ptDiff.y, 0, 0,
                                               SWP_MOVE);/* subtract the difference bet
                                                            pointer and frame, and move there */
                /* IMPORTANT */
                DosSleep(0);/* force a task switch, to allow other windows to update */
           }
           break;
        case WM_BUTTON1UP:
        case WM_BUTTON2UP:
        case WM_BUTTON3UP:/* end of moving */
           if (IsMoving==0)
                break;/* we're not even moving, life is easy. */
           IsMoving=0;
           WinSetCapture(HWND_DESKTOP, NULLHANDLE);/* unset mouse capture, nomal operation */
           if (!IsMoved) {/* Not moved even once ... select the window */
                WinSetWindowPos( hwndEnhFrame, NULLHANDLE, 0, 0, 0, 0, SWP_ACTIVATE);
           }
           return (MRESULT)1;
        }
        return (OldBarProc)(hwnd, msg, mp1, mp2);
}



MRESULT EXPENTRY EnhFrameProc(HWND hwnd, unsigned long msg, MPARAM mp1, MPARAM mp2) {
        switch (msg) {
        case WM_ADJUSTFRAMEPOS:
                /* must capture this msg, to know when we're getting max'ed/restored */
                if ( ((PSWP)mp1)->fl & SWP_MAXIMIZE)
                        FrameMaximized=1;/* this flag prevents window from moving */
                else if ( (((PSWP)mp1)->fl & SWP_RESTORE) || (((PSWP)mp1)->fl & SWP_MINIMIZE) )
                        FrameMaximized=0;/* this flag enables window to move */
        break;
        }
        return (OldFrameProc)(hwnd, msg, mp1, mp2);
}
