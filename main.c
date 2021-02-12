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

int main(void)
{
   /* Open the X display. If this doesn't work, we can't do anything */
   Display* display = XOpenDisplay(NULL);
   if (display == NULL)
   {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }

   /* Get the screen and compute its dimensions */
   int screen = DefaultScreen(display);
   int screenWidth = DisplayWidth(display, screen);
   int screenHeight = DisplayHeight(display, screen);

   /* Get some default visual items */
   Window rootWindow = RootWindow(display, screen);
   GC graphicsContext = DefaultGC(display, screen);
   Visual* visual = DefaultVisual(display, screen);
   int depth = DefaultDepth(display, screen);

   /* Create the window to look at and subscribe to events */
   long blackPixel = BlackPixel(display, screen);
   long whitePixel = WhitePixel(display, screen);
   Window window = XCreateSimpleWindow(display, rootWindow, 10, 10, 100, 100, 1, blackPixel, whitePixel);
   XSelectInput(display, window, ExposureMask | KeyPressMask);
   XMapWindow(display, window);

   /* Get an image of the current the full screen. */
   XImage *image = NULL;
   image = XGetImage(display, rootWindow, 0, 0, screenWidth, screenHeight, AllPlanes, ZPixmap);

   XImage *newImage = NULL;

   /* Event loop */
   XEvent e;
   while (1)
   {
      XNextEvent(display, &e);

      /* Window was exposed, so draw it! 
       * In this case we just draw the image onto the window. */
      if (e.type == Expose)
      {
         XPutImage(display, window, graphicsContext, image, 0, 0, 0, 0, image->width, image->height);
      }

      /* On a key press, figure out which key. */
      if (e.type == KeyPress)
      {
         char buf[128] = {0};
         KeySym keysym;
         int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);

         /* Exit program for Escape key or q key*/
         if (keysym == XK_Escape || keysym == XK_q)
            break;

         /* Test refresh when r key is pressed to see if regenerating the image is okay */
         else if (keysym == XK_r)
         {
            newImage = XCreateImage(display, visual, depth, ZPixmap, 0, image->data, screenWidth, screenHeight, 8, 0);
            XPutImage(display, window, graphicsContext, newImage, 100, 100, 0, 0, newImage->width, newImage->height);
            newImage->data = NULL; /* XDestroyImage frees the data struct, but we need to keep it around because it points to the original image */
            XDestroyImage(newImage);
         }

         /* Zoom in when plus key is pressed */
         else if (keysym == XK_plus)
         {
            double scale = 2.0;
            double scaleWidth = scale;
            double scaleHeight = scale;
            
            double w2 = screenWidth * scale;
            double h2 = screenHeight * scale;

            /* Allocate space for scaled image data */
            char *dataz = malloc(w2 * h2 * 4);
            memset(dataz, 0, w2 * h2 * 4);

            int byte_order = image->byte_order; /* data byte order, LSBFirst, MSBFirst */
            int bits_per_pixel = image->bits_per_pixel;      /* bits per pixel (ZPixmap) */
            char* data = image->data;

            /* Copy existing image to modify */
            long pixel = 0;
            XImage *newImage = XCreateImage(display, visual, depth, ZPixmap, 0, dataz, w2, h2, 8, 0);

            for (int x = 0; x < image->width; x++)
            {
               for (int y = 0; y < image->height; y++)
               {
                  // Invert the color of each pixel
                  pixel = XGetPixel(image, x, y);
                  XPutPixel(newImage, x, y, ~pixel);
               }
            }
            
            XPutImage(display, window, graphicsContext, newImage, 0, 0, 0, 0, newImage->width, newImage->height);
            newImage->data = NULL;
            XDestroyImage(newImage);
            free(dataz);
         }
      }
   }

   /* Clean up and exit */
   if (image != NULL)
      XDestroyImage(image);
   XCloseDisplay(display);
   return 0;
}