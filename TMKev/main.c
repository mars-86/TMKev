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

LRESULT CALLBACK term_proc(HANDLE h, UINT m, WPARAM p, LPARAM l)
{
    if (p == KEY_EVENT) {
        switch (l) {
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
    else if (p == MOUSE_EVENT) {
        switch (l) {
        case VK_LBUTTON:
            printf("%s", "LB");
            break;
        case VK_RBUTTON:
            printf("%s", "RB");
            break;
        case VK_MBUTTON:
            printf("%s", "MB");
            break;
        default:
            ;
        }
    }
    else {

    }
    return 0;
}