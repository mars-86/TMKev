#include <stdio.h>
#include "tmkev.h"

LRESULT CALLBACK term_proc(HANDLE, UINT, WPARAM, LPARAM);

int main(int argc, char *argv[])
{
    TermClass tc;
    tc.cbSize = sizeof(tc);
    tc.lpfnTermProc = term_proc;

    init_event_handler();
    register_term_class(&tc);

    while (get_event()){
        // read incomming events
        read_event_non_block();
        // dispatch events to handler
        dispatch_event();
    }
    return 0;
}

LRESULT CALLBACK term_proc(HANDLE h, UINT e, WPARAM w, LPARAM l)
{
    if (TMKEV_GET_EVENT_TYPE(e) == KEY_EVENT) {
        if (TMKEV_KEYB_EVENT_IS_KEY_PRESSED(e)) {
            switch (TMKEV_KEYB_GET_KEY_AS_ASCII(l)) {
            case VK_ESCAPE:
                stop_event_handler();
                break;
            case VK_KEY_W_LOW:
                printf("%s", "w");
                break;
            default:
                ;
            }
        }
    }
    else if (TMKEV_GET_EVENT_TYPE(e) == MOUSE_EVENT) {
        switch (TMKEV_MOUSE_EVENT_STYPE(e)) {
        case MOUSE_MOVED:
            printf("MM: %d,%d,", TMKEV_MOUSE_GET_X_COORD(l), TMKEV_MOUSE_GET_Y_COORD(l));
            break;
        case DOUBLE_CLICK:
            printf("DC: %d,%d,", TMKEV_MOUSE_GET_X_COORD(l), TMKEV_MOUSE_GET_Y_COORD(l));
            break;
        case MOUSE_WHEELED:
            printf("%s", "Wheel");
            break;
        // single button pressed
        default:
            switch (TMKEV_GET_MOUSE_BUTTON_STATUS(w)) {
            case VK_LBUTTON:
                printf("LB: %d,%d,", TMKEV_MOUSE_GET_X_COORD(l), TMKEV_MOUSE_GET_Y_COORD(l));
                break;
            case VK_RBUTTON:
                printf("RB: %d,%d,", TMKEV_MOUSE_GET_X_COORD(l), TMKEV_MOUSE_GET_Y_COORD(l));
                break;
            case VK_MBUTTON:
                printf("MB: %d,%d,", TMKEV_MOUSE_GET_X_COORD(l), TMKEV_MOUSE_GET_Y_COORD(l));
                break;
            default:
                ;
            }
        }
    }
    else if (TMKEV_GET_EVENT_TYPE(e) == WINDOW_BUFFER_SIZE_EVENT){
        printf("WS: %d,%d,", TMKEV_WSIZE_GET_X_COORD(l), TMKEV_WSIZE_GET_Y_COORD(l));
    }
    else {

    }
    return 0;
}
