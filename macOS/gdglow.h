/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/
#import <AppKit/AppKit.h>

#ifndef _GDGLOW_H
#define _GDGLOW_H

void APResize(const NSWindow* win, int x, int y, int w, int h);
NSApplication* APGetApp();
void APSetTaskbar(const NSApplication* app, bool enable);
NSWindow* APGetNSWindow(const NSApplication* app, long cgid);

#endif
