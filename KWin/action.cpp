/* Copyright (c) 2024-2025, Litwak913
   At GPL-3.0 License
*/

#include "arkpetsintegration.h"

#include "kwin/workspace.h"
#include "kwin/window.h"

namespace ArkPets {

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
    const auto window = AP_FIND_WINDOW(uuid, false) const auto active = KWin::Workspace::self()->activeWindow();
    return window->internalId() == active->internalId();
}

void ArkPetsIntegration::Activate(const QString &uuid)
{
    const auto window = AP_FIND_WINDOW(uuid, ) KWin::Workspace::self()->activateWindow(window);
}

void ArkPetsIntegration::MoveResize(const QString &uuid, int x, int y, uint w, uint h)
{
    const auto window = AP_FIND_WINDOW(uuid, ) QRectF rect(x, y, w, h);
    window->moveResize(rect);
}

}  // namespace ArkPets
