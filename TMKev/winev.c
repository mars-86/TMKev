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
            KEY_EVENT_RECORD ker = read_buff[i].Event.KeyEvent;
            ev = (read_buff[i].Event.KeyEvent.bKeyDown) ? (ev | (1 << 16)) : (ev | (0 << 16));
            wparam = ((wparam | ker.dwControlKeyState) << 16) | ker.wRepeatCount;
            lparam = ((lparam | ker.uChar.UnicodeChar) << 16) | ker.uChar.AsciiChar;
        }
        if (ev == MOUSE_EVENT){
            MOUSE_EVENT_RECORD mer = read_buff[i].Event.MouseEvent;
            ev |= (mer.dwEventFlags << 16);
            wparam = ((wparam | mer.dwControlKeyState) << 16) | mer.dwButtonState;
            lparam = ((lparam | mer.dwMousePosition.Y) << 16) | mer.dwMousePosition.X;
        }
        _tc.lpfnTermProc(_stdinh, ev, wparam, lparam);
    }
    return 0;
}