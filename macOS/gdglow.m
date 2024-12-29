 /* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#import "gdglow.h"

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

NSApplication* APGetApp()
{
    return [NSApplication sharedApplication];
}

void APSetDock(const NSApplication* app, BOOL enable)
{
    if (enable) {
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
    } else {
        [app setActivationPolicy:NSApplicationActivationPolicyAccessory];
    }
}

void APSetTopmost(const NSWindow* win, BOOL enable) {
    if (enable) {
        win.level = NSStatusWindowLevel;
    } else {
        win.level = NSNormalWindowLevel;
    }
}

NSWindow* APGetNSWindow(const NSApplication* app, long cgid)
{
    return [app windowWithWindowNumber:cgid];
}

void APActive(const NSWindow* win) {
    [win orderFrontRegardless];
}

int APVersion() { return GDGLOW_VER; }
