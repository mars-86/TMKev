#ifndef TMKEV_WINEV_H
#define TMKEV_WINEV_H
#pragma once

#include <stdio.h>
#include <windows.h>

#define TMKEV_GET_EVENT_TYPE(ev) (ev & 0x0000FFFF)
#define TMKEV_KEYB_EVENT_IS_KEY_PRESSED(ev) ((ev & 0xFFFF0000) >> 16)
#define TMKEV_MOUSE_EVENT_STYPE(ev) ((ev & 0xFFFF0000) >> 16)
#define TMKEV_KEYB_GET_KEY_AS_ASCII(key) (key & 0x0000FFFF)
#define TMKEV_KEYB_GET_KEY_AS_UNICODE(key) ((key & 0xFFFF0000) >> 16)
#define TMKEV_GET_MOUSE_BUTTON_STATUS(mb) (mb & 0x0000FFFF)
#define TMKEV_GET_CONTROL_KEY_STATUS(mb) ((mb & 0xFFFF0000) >> 16)
#define TMKEV_MOUSE_GET_X_COORD(coords) (coords & 0x0000FFFF)
#define TMKEV_MOUSE_GET_Y_COORD(coords) ((coords & 0xFFFF0000) >> 16)
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
