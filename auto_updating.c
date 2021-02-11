#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

const int TEST_SIZE = 512;

int main()
{
  // Open default display
  Display *display = XOpenDisplay(NULL);
  int screen = DefaultScreen(display);
  Window rootWin = RootWindow(display, screen);
  GC graphicsContext = DefaultGC(display, screen);

  // Create new window and subscribe to events
  long blackPixel = BlackPixel(display, screen);
  long whitePixel = WhitePixel(display, screen);
  Window newWin = XCreateSimpleWindow(display, rootWin, 0, 0, TEST_SIZE, TEST_SIZE, 1, blackPixel, whitePixel);
  XMapWindow(display, newWin);
  XSelectInput(display, newWin, ExposureMask | KeyPressMask);

  // Compute screen sizes
  int screenWidth = DisplayWidth(display, screen);
  int screenHeight = DisplayHeight(display, screen);

  // Main event loop for new window
  XImage *image;
  XEvent event;
  int exposed = False;
  int killWindow = False;
  while (!killWindow)
  {
    // Handle pending events
    if (XPending(display) > 0)
    {
      XNextEvent(display, &event);
      if (event.type == Expose)
      {
        exposed = True;
      } else if (event.type == NoExpose)
      {
        exposed = False;
      } else if (event.type == KeyPress)
      {
        killWindow = True;
      }
    }

    // Capture the original image
    image = XGetImage(display, rootWin, 0, 0, screenWidth, screenHeight, AllPlanes, ZPixmap);

    // Modify the image
    if (image->data != NULL)
    {
      long pixel = 0;
      for (int x = 0; x < image->width; x++)
      {
        for (int y = 0; y < image->height; y++)
        {
          // Invert the color of each pixel
          pixel = XGetPixel(image, x, y);
          XPutPixel(image, x, y, ~pixel);
        }
      }
    }

    // Output the modified image
    if (exposed && killWindow == False)
    {
      XPutImage(display, newWin, graphicsContext, image, 0, 0, 0, 0, screenWidth, screenHeight);
    }
    XDestroyImage(image);
  }

  // Goodbye
  XCloseDisplay(display);
}
