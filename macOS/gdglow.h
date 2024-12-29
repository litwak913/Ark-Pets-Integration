/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/
#import <AppKit/AppKit.h>

#ifndef _GDGLOW_H
#define _GDGLOW_H
#define GDGLOW_VER 1

void APResize(const NSWindow* win, int x, int y, int w, int h);
void APResizeOnMain(const NSWindow* win, int x, int y, int w, int h);
NSApplication* APGetApp();
void APSetDock(const NSApplication* app, BOOL enable);
void APSetTopmost(const NSWindow* app, BOOL enable);
NSWindow* APGetNSWindow(const NSApplication* app, long cgid);
void APActive(const NSWindow* win);
int APVersion();
#endif
