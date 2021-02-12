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

XImage* ScaleXImage(XImage* originalImage, double scale, Display* display, Visual* visual, int depth)
{
   double scaleWidth = scale;
   double scaleHeight = scale;
   
   double w2 = originalImage->width * scale;
   double h2 = originalImage->height * scale;

   /* Allocate space for scaled image data */
   char *dataz = malloc(w2 * h2 * 4);
   memset(dataz, 0, w2 * h2 * 4);

   int byte_order = originalImage->byte_order; /* data byte order, LSBFirst, MSBFirst */
   int bits_per_pixel = originalImage->bits_per_pixel;      /* bits per pixel (ZPixmap) */
   char* data = originalImage->data;

   /* Copy existing image to modify */
   long pixel = 0;
   XImage* scaledImage = XCreateImage(display, visual, depth, ZPixmap, 0, dataz, w2, h2, 8, 0);

   for (int x = 0; x < scaledImage->width; x++)
   {
      for (int y = 0; y < scaledImage->height; y++)
      {
         /* Figure out the closest pixel on the original image */
         int x1 = (int)(x / scaleWidth);
         int y1 = (int)(y / scaleHeight);
         pixel = XGetPixel(originalImage, x1, y1);

         /* Invert the color, just for fun */
         XPutPixel(scaledImage, x, y, ~pixel);
      }
   }

   return scaledImage;
}

struct ViewLocation
{
   int Top;
   int Left;
} viewLocation = { .Top = 0, .Left = 0 };


int main(void)
{
   /* Open the X display. If this doesn't work, we can't do anything */
   Display* display = XOpenDisplay(NULL);
   if (display == NULL)
   {
      fprintf(stderr, "Cannot open X display\n");
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
   XImage* screenshot = NULL;
   screenshot = XGetImage(display, rootWindow, 0, 0, screenWidth, screenHeight, AllPlanes, ZPixmap);

   /* Scale the image by two for initial display */
   double defaultScaleFactor = 2.0;
   double currentScaleFactor = defaultScaleFactor;
   XImage* scaledImage = ScaleXImage(screenshot, currentScaleFactor, display, visual, depth);

   /* Event loop */
   XEvent e;
   while (1)
   {
      XNextEvent(display, &e);

      /* Window was exposed, so draw it! 
       * In this case we just draw the image onto the window. */
      if (e.type == Expose)
      {
         XPutImage(display, window, graphicsContext, scaledImage, viewLocation.Left, viewLocation.Top, 0, 0, scaledImage->width, scaledImage->height);
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

         /* Zoom in when plus key is pressed */
         else if (keysym == XK_plus || keysym == XK_equal)
         {
            currentScaleFactor += 1.0;
            scaledImage = ScaleXImage(screenshot, currentScaleFactor, display, visual, depth);
            XPutImage(display, window, graphicsContext, scaledImage, viewLocation.Left, viewLocation.Top, 0, 0, scaledImage->width, scaledImage->height);
         }

         /* Zoom out for minus key. Scale cannot be less than 1.0 */
         else if (keysym == XK_minus)
         {
            currentScaleFactor -= 1.0;
            if (currentScaleFactor < 1.0)
               currentScaleFactor = 1.0;
            scaledImage = ScaleXImage(screenshot, currentScaleFactor, display, visual, depth);
            XPutImage(display, window, graphicsContext, scaledImage, viewLocation.Left, viewLocation.Top, 0, 0, scaledImage->width, scaledImage->height);
         }

         /* Move view with arrow keys */
         else if (keysym == XK_Right)
         {
            viewLocation.Left += 100;
            XPutImage(display, window, graphicsContext, scaledImage, viewLocation.Left, viewLocation.Top, 0, 0, scaledImage->width, scaledImage->height);
         }
         else if (keysym == XK_Left)
         {
            viewLocation.Left -= 100;
            XPutImage(display, window, graphicsContext, scaledImage, viewLocation.Left, viewLocation.Top, 0, 0, scaledImage->width, scaledImage->height);
         }
         else if (keysym == XK_Up)
         {
            viewLocation.Top -= 100;
            XPutImage(display, window, graphicsContext, scaledImage, viewLocation.Left, viewLocation.Top, 0, 0, scaledImage->width, scaledImage->height);
         }
         else if (keysym == XK_Down)
         {
            viewLocation.Top += 100;
            XPutImage(display, window, graphicsContext, scaledImage, viewLocation.Left, viewLocation.Top, 0, 0, scaledImage->width, scaledImage->height);
         }
      }
   }

   /* Clean up and exit */
   if (screenshot != NULL)
      XDestroyImage(screenshot);

   if (scaledImage != NULL)
      XDestroyImage(scaledImage);

   XCloseDisplay(display);
   return 0;
}