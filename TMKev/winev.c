#include "winev.h"

static HANDLE _stdinh = NULL;
static TermClass _tc;

int init_event_handler(void)
{
    if ((_stdinh = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "GetStdHandle: %u", GetLastError());
        return -1;
    }
    DWORD mode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if (!(SetConsoleMode(_stdinh, mode))) {
        fprintf(stderr, "SetConsoleMode: %u", GetLastError());
        return -1;
    }
    return 0;
}

void register_term_class(const TermClass* tc)
{
    _tc.cbSize = tc->cbSize;
    _tc.style = tc->style;
    _tc.lpfnTermProc = tc->lpfnTermProc;
    _tc.cbClsExtra = tc->cbClsExtra;
    _tc.cbWndExtra = tc->cbClsExtra;
    _tc.stdHandle = tc->stdHandle;
    _tc.hCursor = tc->hCursor;
    _tc.hbrBackground = tc->hbrBackground;
    _tc.lpszMenuName = tc->lpszMenuName;
    _tc.lpszClassName = tc->lpszClassName;
}

#define TMKEV_READ_BUFF_SIZE 128
static INPUT_RECORD read_buff[TMKEV_READ_BUFF_SIZE];
static int records_read = 0;
static int reading_events = 1;

int get_event(void)
{
    return reading_events;
}

int read_event(void)
{
    // Wait for the events.
    if (!ReadConsoleInput(
        _stdinh,                // input buffer handle 
        read_buff,              // buffer to read into 
        TMKEV_READ_BUFF_SIZE,   // size of read buffer 
        &records_read))         // number of records read
    {
        fprintf(stderr, "ReadConsoleInput: %u", GetLastError());
        return -1;
    }
    return 0;
}

int dispatch_event(void)
{
    int i, ev = 0, wparam = 0, lparam = 0;
    // Dispatch the events to the appropriate handler.
    for (i = 0; i < records_read; ++i){
        ev = read_buff[i].EventType;
        if (ev == KEY_EVENT){
            if (read_buff[i].Event.KeyEvent.bKeyDown == TRUE){
                read_buff[i].Event.KeyEvent.bKeyDown; // KEY_DOWN EV
                _tc.lpfnTermProc(
                    _stdinh,
                    read_buff[i].EventType,
                    read_buff[i].Event.KeyEvent.uChar.AsciiChar,
                    0
                );
            }
        }
        if (ev == MOUSE_EVENT){
            ev |= (read_buff[i].Event.MouseEvent.dwEventFlags << 16);
            wparam = ((wparam | read_buff[i].Event.MouseEvent.dwControlKeyState) << 16) | read_buff[i].Event.MouseEvent.dwButtonState;
            lparam = ((lparam | read_buff[i].Event.MouseEvent.dwMousePosition.X) << 16) | read_buff[i].Event.MouseEvent.dwMousePosition.Y;
            _tc.lpfnTermProc(_stdinh, ev, wparam, lparam);
        }
    }
    return 0;
}

/*
    INPUT_RECORD irInBuf[128];
    int i, run = 1;
    while (run)
    {
        // Wait for the events.
        if (!ReadConsoleInput(
            _stdinh,     // input buffer handle 
            irInBuf,    // buffer to read into 
            128,        // size of read buffer 
            &cNumRead)) // number of records read
        {
            fprintf(stderr, "Error Read %u", GetLastError());
            exit(1);
        }
        // Dispatch the events to the appropriate handler.
        for (i = 0; i < cNumRead; ++i) {
            if (irInBuf[i].EventType == KEY_EVENT) {
                if (irInBuf[i].Event.KeyEvent.bKeyDown == TRUE) {
                    irInBuf[i].Event.KeyEvent.bKeyDown; // KEY_DOWN EV
                    run = _tc.lpfnTermProc(
                        _stdinh,
                        0,
                        irInBuf[i].EventType,
                        irInBuf[i].Event.KeyEvent.uChar.AsciiChar
                    );
                }
            }
            if (irInBuf[i].EventType == MOUSE_EVENT) {
                run = _tc.lpfnTermProc(
                    _stdinh,
                    0,
                    irInBuf[i].EventType,
                    irInBuf[i].Event.MouseEvent.dwButtonState
                );
            }
        }
    }
    return 0;
}
*/