/* Copyright (c) 2024-2025, Litwak913
   At GPL-3.0 License
*/

#include "arkpetsintegration.h"

#include "kwin/window.h"
#include "kwin/workspace.h"
#include "kwin/outline.h"

using namespace KWin;
namespace ArkPets
{

void ArkPetsIntegration::Above(const QString &uuid, bool enable)
{
    const auto window = AP_FIND_WINDOW(uuid, ) window->setKeepAbove(enable);
}

void ArkPetsIntegration::Stick(const QString &uuid, bool enable)
{
    const auto window = AP_FIND_WINDOW(uuid, ) window->setSkipTaskbar(!enable);
    window->setSkipSwitcher(!enable);
    window->setSkipPager(!enable);
    window->setOnAllDesktops(!enable);
}

bool ArkPetsIntegration::IsActive(const QString &uuid)
{
    const auto window = AP_FIND_WINDOW(uuid, false) const auto active = Workspace::self()->activeWindow();
    return window->internalId() == active->internalId();
}

void ArkPetsIntegration::Activate(const QString &uuid)
{
    const auto window = AP_FIND_WINDOW(uuid, ) Workspace::self()->activateWindow(window);
}

void ArkPetsIntegration::MoveResize(const QString &uuid, int x, int y, uint w, uint h)
{
    const auto window = AP_FIND_WINDOW(uuid, ) QRectF rect(x, y, w, h);
    window->moveResize(rect);
}

void ArkPetsIntegration::TestStrutOutline(const QString &uuid) {
    const auto window = AP_FIND_WINDOW(uuid, )
    const auto struts = window->strutRects();
    if(struts.empty()) {
        qWarning() << "Window no strut";
        return;
    }
    for (const StrutRect& sr : window->strutRects()) {
        qWarning() << sr;
        Workspace::self()->outline()->show(sr);
    }
}
} // namespace ArkPets
