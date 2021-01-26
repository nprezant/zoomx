#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compile with
 * gcc -g main.c -L/usr/X11R6/lib -lX11 -o main
 * ./main
 */
 
int main(void) {
   Display *d;
   Window w;
   XEvent e;
   const char *msg = "Hello, World!";
   int s;
 
   d = XOpenDisplay(NULL);
   if (d == NULL) {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }
 
   s = DefaultScreen(d);
   w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1, BlackPixel(d, s), WhitePixel(d, s));
   XSelectInput(d, w, ExposureMask | KeyPressMask);
   XMapWindow(d, w);

   XImage* img = NULL;
   img = XGetImage(d, RootWindow(d, s), 0, 0, 1920, 1080, AllPlanes, ZPixmap);
 
   while (1) {
      XNextEvent(d, &e);
      if (e.type == Expose) {
          /* Window was exposed, so draw it! */
         XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
         XDrawString(d, w, DefaultGC(d, s), 10, 50, msg, strlen(msg));
         XPutImage(d, w, DefaultGC(d, s), img, 0, 0, 0, 0, img->width, img->height);
      }

      if (e.type == KeyPress) {
        /* Key was pressed. Figure out which key. */
        char buf[128] = {0};
        KeySym keysym;
        int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);

        /* Exit program for Escape key or q key*/
        if (keysym == XK_Escape || keysym == XK_q)
            break;
      }
   }
 
   XCloseDisplay(d);
   return 0;
}