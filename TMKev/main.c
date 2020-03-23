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
        read_event();
        // dispatch events to handler
        dispatch_event();
    }
    return 0;
}

LRESULT CALLBACK term_proc(HANDLE h, UINT e, WPARAM w, LPARAM l)
{
    if (TMKEV_GET_EVENT_TYPE(e) == KEY_EVENT) {
        switch (w) {
        case VK_ESCAPE:
            printf("%s", "ESC");
            break;
        case VK_KEY_W_LOW:
            printf("%s", "w");
            break;
        default:
            ;
            //printf("%d", l);
        }
    }
    else if (TMKEV_GET_EVENT_TYPE(e) == MOUSE_EVENT) {
        switch (TMKEV_GET_EVENT_STYPE(e)) {
            case MOUSE_MOVED:
                printf("%d,", TMKEV_MOUSE_GET_X_COORD(l));
                printf("%d", TMKEV_MOUSE_GET_Y_COORD(l));
                break;
            case DOUBLE_CLICK:
                printf("%s", "DC");
                printf("%d,", TMKEV_MOUSE_GET_X_COORD(l));
                printf("%d", TMKEV_MOUSE_GET_Y_COORD(l));
                break;
            /*case MOUSE_WHEELED:
                printf("%s", "Wheel");
                break;*/
            default:
                switch (TMKEV_GET_MOUSE_BUTTON_STATUS(w)) {
                case VK_LBUTTON:
                    printf("%s", "LB");
                    printf("%d,", TMKEV_MOUSE_GET_X_COORD(l));
                    printf("%d", TMKEV_MOUSE_GET_Y_COORD(l));
                    break;
                case VK_RBUTTON:
                    printf("%s", "RB");
                    printf("%d,", TMKEV_MOUSE_GET_X_COORD(l));
                    printf("%d", TMKEV_MOUSE_GET_Y_COORD(l));
                    break;
                case VK_MBUTTON:
                    printf("%s", "MB");
                    printf("%d,", TMKEV_MOUSE_GET_X_COORD(l));
                    printf("%d", TMKEV_MOUSE_GET_Y_COORD(l));
                    break;
                default:
                    ;
                }

        }
    }
    else {

    }
    return 0;
}