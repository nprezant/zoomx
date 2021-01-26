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
   int s;
 
   /* Open the X display. If this doesn't work, we can't do anything */
   d = XOpenDisplay(NULL);
   if (d == NULL) {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }
 
   /* Create the screen and window to look at */
   s = DefaultScreen(d);
   w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1, BlackPixel(d, s), WhitePixel(d, s));
   XSelectInput(d, w, ExposureMask | KeyPressMask);
   XMapWindow(d, w);

   /* Get an image of the current the full screen.
    * TODO find screen size automatically */
   XImage* img = NULL;
   img = XGetImage(d, RootWindow(d, s), 0, 0, 1920, 1080, AllPlanes, ZPixmap);
 
   /* Event loop */
   while (1) {
      XNextEvent(d, &e);

      /* Window was exposed, so draw it! 
       * In this case we just draw the image onto the window. */
      if (e.type == Expose) {
         XPutImage(d, w, DefaultGC(d, s), img, 0, 0, 0, 0, img->width, img->height);
      }

      /* On a key press, figure out which key. */
      if (e.type == KeyPress) {
        char buf[128] = {0};
        KeySym keysym;
        int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);

        /* Exit program for Escape key or q key*/
        if (keysym == XK_Escape || keysym == XK_q)
            break;
      }
   }

   /* Clean up and exit */
   if(img != NULL)
      XDestroyImage(img);
   XCloseDisplay(d);
   return 0;
}