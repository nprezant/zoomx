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
   Display *d;
   Window w;
   XEvent e;
   int s;
   int width;
   int height;

   /* Open the X display. If this doesn't work, we can't do anything */
   d = XOpenDisplay(NULL);
   if (d == NULL)
   {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }

   /* Create the screen and compute its dimensions */
   s = DefaultScreen(d);
   width = DisplayWidth(d, s);
   height = DisplayHeight(d, s);

   /* Create the window to look at */
   w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1, BlackPixel(d, s), WhitePixel(d, s));
   XSelectInput(d, w, ExposureMask | KeyPressMask);
   XMapWindow(d, w);

   /* Get an image of the current the full screen.
    * TODO find screen size automatically */
   XImage *img = NULL;
   img = XGetImage(d, RootWindow(d, s), 0, 0, width, height, AllPlanes, ZPixmap);

   XImage *newImage = NULL;

   /* Event loop */
   while (1)
   {
      XNextEvent(d, &e);

      /* Window was exposed, so draw it! 
       * In this case we just draw the image onto the window. */
      if (e.type == Expose)
      {
         XPutImage(d, w, DefaultGC(d, s), img, 0, 0, 0, 0, img->width, img->height);
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
            newImage = XCreateImage(d, DefaultVisual(d, s), DefaultDepth(d, s), ZPixmap, 0, img->data, width, height, 8, 0);
            XPutImage(d, w, DefaultGC(d, s), newImage, 100, 100, 0, 0, newImage->width, newImage->height);
            newImage->data = NULL; /* XDestroyImage frees the data struct, but we need to keep it around because it points to the original image */
            XDestroyImage(newImage);
         }

         /* Zoom in when plus key is pressed */
         else if (keysym == XK_plus)
         {
            double scale = 2.0;
            double scaleWidth = scale;
            double scaleHeight = scale;
            
            double w2 = width * scale;
            double h2 = height * scale;
            char *dataz = malloc(w2 * h2 * 3);

            int byte_order = img->byte_order; /* data byte order, LSBFirst, MSBFirst */
            int bits_per_pixel = img->bits_per_pixel;      /* bits per pixel (ZPixmap) */
            LSBFirst;
            char* data = img->data;

            for(int cy = 0; cy < h2; cy++)
            {
                  for(int cx = 0; cx < w2; cx++)
                  {
                     int pixel = (cy * (w2 *3)) + (cx*3);
                     int nearestMatch =  (((int)(cy / scaleHeight) * (width *3)) + ((int)(cx / scaleWidth) *3) );
                     
                     dataz[pixel    ] =  data[nearestMatch    ];
                     dataz[pixel + 1] =  data[nearestMatch + 1];
                     dataz[pixel + 2] =  data[nearestMatch + 2];
                  }
            }

            /*
            Example from stack overflow
            dest[dx,dy] = src[dx*src_width/dest_width,dy*src_height/dest_height]
            */

            /*
            int x_ratio = (int)((width << 16) / w2) + 1;
            int y_ratio = (int)((height << 16) / h2) + 1;
            int x2, y2;
            for (int i = 0; i < h2; i++)
            {
               for (int j = 0; j < w2; j++)
               {
                  x2 = ((j * x_ratio) >> 16);
                  y2 = ((i * y_ratio) >> 16);
                  dataz[(i * w2) + j] = dataz[(y2 * width) + x2];
               }
            }*/

            /*
            for (int y = 0; y < height; y++)
               for (int x = 0; x < width; x++)
                  for (int y0 = 0; y0 < scale; y0++)
                     for (int x0 = 0; x0 < scale; x0++)
                     {
                        int srcIndex = x + y;
                        int targetIndex = x + y; // (y * scale + y0) * width * scale + x * scale + x0
                        dataz[targetIndex] = img->data[srcIndex];
                     }
            */
            
            XImage *newImage = XCreateImage(d, DefaultVisual(d, s), DefaultDepth(d, s), ZPixmap, 0, dataz, w2, h2, 8, 0);
            XPutImage(d, w, DefaultGC(d, s), newImage, 0, 0, 0, 0, newImage->width, newImage->height);
            newImage->data = NULL;
            XDestroyImage(newImage);
            free(dataz);
         }
      }
   }

   /* Clean up and exit */
   if (img != NULL)
      XDestroyImage(img);
   XCloseDisplay(d);
   return 0;
}