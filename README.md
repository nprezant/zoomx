# ZoomX

A simple and lightweight tool for zooming in and panning around on an X display.

Inspired by ZoomIt for Windows.

## Usage

Launchable from the command line with `zoomx`, but recommended to bind it to a hotkey within your window manager.

Configuration with i3:

```shell
# ~/.config/i3/config
bindsym $mod+Shift+z exec zoomx
```

## Building from source

Build from source with gcc

```shell
gcc -g zoomx.c -L/usr/X11R6/lib -lX11 -o zoomx
./zoomx
```