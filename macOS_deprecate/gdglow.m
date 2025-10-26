 /* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#import "gdglow.h"

#define APRunOnMain(blk)\
    if ([NSThread isMainThread]) {\
        blk();\
    } else {\
        dispatch_sync(dispatch_get_main_queue(), blk);\
    }

void APResize(const NSWindow* win, int x, int y, int w, int h)
{
    NSScreen* scr = [win screen];
    CGFloat newY = NSHeight([scr frame]) - (CGFloat)h - (CGFloat)y;
    NSRect rect = NSMakeRect(x, newY, w, h);
    @try {
        [win setFrame:rect display:YES animate:NO];
    } @catch (NSException* e) {
        NSLog(@"Invalid rect: %@: %@", [e name], [e reason]);
    }
}

void APResizeOnMain(const NSWindow* win, int x, int y, int w, int h) {
    APRunOnMain(^{ APResize(win, x, y, w, h); });
}

NSApplication* APGetApp()
{
    return [NSApplication sharedApplication];
}

void APSetDock(const NSApplication* app, BOOL enable)
{
    int policy = enable ? NSApplicationActivationPolicyRegular : NSApplicationActivationPolicyAccessory;
    [app setActivationPolicy:policy];
}

void APSetDockOnMain(const NSApplication* app, BOOL enable)
{
    APRunOnMain(^{ APSetDock(app, enable); })
}

void APSetTopmost(const NSWindow* win, BOOL enable) {
    int level = enable ? NSStatusWindowLevel : NSNormalWindowLevel;
    win.level = level;
}

void APSetTopmostOnMain(const NSWindow* win, BOOL enable)
{
    APRunOnMain(^{ APSetTopmost(win, enable); })
}

NSWindow* APGetNSWindow(const NSApplication* app, long cgid)
{
    NSWindow* win = [app windowWithWindowNumber:cgid];
    return win;
}

void APActive(const NSWindow* win) {
    [win orderFrontRegardless];
}

int APVersion() { return GDGLOW_VER; }
