#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

/* Compile with
 * gcc -g screenshot.c -L/usr/X11R6/lib -lX11 -o screenshot
 * ./screenshot
 */

int main()
{
    printf("running\n");

    Display* d = XOpenDisplay(NULL);
    int s = 0;
    Window w = RootWindow(d, s);
    int x = 0;
    int y = 0;
    int width = 1920;
    int height = 1080;
    XImage* img = NULL;
    XEvent e;

    while(True) 
    {
        if(img != NULL)
            XDestroyImage(img);
        img = XGetImage(d, w, x, y, width, height, AllPlanes, ZPixmap);
        printf("%d\n", width);

        /* Listen for events */
        XNextEvent(d, &e);
        if (e.type == Expose) {
            /* Window was exposed, so draw it! */
            XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
        }

        if (e.type == KeyPress) {
            /* Key was pressed. Figure out which key. */
            char buf[128] = {0};
            KeySym keysym;
            int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);

            /* Exit program for Escape key */
            if (keysym == XK_Escape)
                break;
        }
    }

    if(img != NULL)
        XDestroyImage(img);
    XCloseDisplay(d);
}