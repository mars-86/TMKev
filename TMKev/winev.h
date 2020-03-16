#ifndef TMKEV_WINEV_H
#define TMKEV_WINEV_H
#pragma once

#include <stdio.h>
#include <windows.h>

typedef LRESULT(_stdcall* TERMPROC)(HANDLE, UINT, WPARAM, LPARAM);

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

void register_term_class(const TermClass* tc);
int init_event_handler(void);
int get_event(void);
int read_event(void);
int dispatch_event(void);

#endif // !TMKEV_WINEV_H
