#include "../include/boids.h"

int main() 
{
    // Open a connection to the X server
    Display *disp = XOpenDisplay(NULL);

    // Get the height and width of the default screen
    int screen = DefaultScreen(disp);
    int screen_height = DisplayHeight(disp, screen);
    int screen_width = DisplayWidth(disp, screen);

    // Create a window on the root window of the default screen
    Window win = XCreateWindow(disp, RootWindow(disp, screen), 0, 0,
                            screen_width, screen_height, 0,
                            CopyFromParent, InputOutput,
                            CopyFromParent, 0, NULL);

    // Set the window attributes
    XSetWindowAttributes attrs;
    attrs.background_pixel = BlackPixel(disp, screen);
    attrs.override_redirect = True;
    XChangeWindowAttributes(disp, win, CWBackPixel, &attrs);
    XChangeWindowAttributes(disp, win, CWOverrideRedirect, &attrs);

    // Set the input events the window is interested in
    XSelectInput(disp, win, ExposureMask | KeyPressMask);

    // Punch a hole in the window
    window_hole_punch(disp, win, screen_height, screen_width);

    // Create a cursor and set it as the window's cursor
    Cursor cursor;
    Pixmap blank;
    XColor dummy;
    blank = XCreatePixmap(disp, win, 1, 1, 1);
    dummy.pixel = 0;
    dummy.red = 0;
    dummy.flags = 04;
    cursor = XCreatePixmapCursor(disp, blank, blank, &dummy, &dummy, 0, 0);
    XDefineCursor(disp, win, cursor);

    // Start the main event loop
    main_loop_boid(disp, win, screen_width, screen_height, screen);

    // Make the window always on top
    MakeAlwaysOnTop(disp, RootWindow(disp, screen), win);

    // Close the connection to the X server and return
    XCloseDisplay(disp);
    return 0;

} 