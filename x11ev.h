#ifndef TMKEV_X11EV_H
#define TMKEV_X11EV_H
#pragma once

#ifdef __linux__

#include <X11/Xlib.h>

typedef long(_stdcall* TERMPROC)(void*, unsigned int, unsigned int, long);

typedef struct _TermClass {
    UINT      cbSize;
    UINT      style;
    TERMPROC  lpfnTermProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HANDLE    stdHandle;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCSTR    lpszMenuName;
    LPCSTR    lpszClassName;
} TermClass;

#endif // __linux__

#endif // !TMKEV_X11EV_H
