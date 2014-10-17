/**************************************

Dan Akselrod (c) 1995.

Enhanced frame header. A few definitions, and the EnhaceFrame function
declaraion. Include this .h file into any module that will call
EnhanceFrame.

**************************************/
#ifndef __enhframe_h__
#define __enhframe_h__

/* Error returns */
#define ENHFRAMEERR_TITLEBAR_NOTEXIST 1
#define ENHFRAMEERR_FRAME_SUBCLASS 2
#define ENHFRAMEERR_TITLEBAR_SUBCLASS 3

int EnhanceFrame(HWND);
/* call this function with a !frame window! handle to enhance it. */
#endif
