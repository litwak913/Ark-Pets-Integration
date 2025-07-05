/* Copyright (c) 2024-2025, Litwak913
   At GPL-3.0 License
*/

#include "arkpetsintegration.h"

#include "kwin/virtualdesktops.h"
#include "kwin/workspace.h"
#include "kwin/window.h"

namespace ArkPets
{

APDetails ArkPetsIntegration::Details(const QString &uuid)
{
    const auto window = KWin::Workspace::self()->findWindow(QUuid::fromString(uuid));
    if (!window || !window->isClient()) {
        qWarning() << "[ArkPets] Window Not Found";
        return {0, 0, 0, 0, QLatin1String(""), QLatin1String(""), false, QLatin1String("")};
    }
    bool minimized = window->isMinimized();
    bool in_current_workspace = false;
    if (window->isOnAllDesktops() || window->isOnCurrentDesktop()) {
        in_current_workspace = true;
    }
    APDetails detail = {.x = qRound(window->x()),
                        .y = qRound(window->y()),
                        .w = static_cast<uint>(qRound(window->width())),
                        .h = static_cast<uint>(qRound(window->height())),
                        .title = window->caption(),
                        .wclass = window->resourceClass(),
                        .visible = (in_current_workspace && !minimized),
                        .id = window->internalId().toString(QUuid::WithoutBraces)};
    return detail;
}

APDetailsList ArkPetsIntegration::List()
{
    APDetailsList winids;
    const auto current_desktop = KWin::VirtualDesktopManager::self()->currentDesktop()->id();
    for (const KWin::Window *window : KWin::Workspace::self()->stackingOrder()) {
        bool minimized = window->isMinimized();
        bool in_current_workspace = false;
        QStringList windesk = window->desktopIds();
        if (windesk.isEmpty()) {
            in_current_workspace = true; // all desktop
        } else {
            if (windesk.contains(current_desktop)) {
                in_current_workspace = true;
            }
        }
        APDetails detail = {.x = qRound(window->x()),
                            .y = qRound(window->y()),
                            .w = static_cast<uint>(qRound(window->width())),
                            .h = static_cast<uint>(qRound(window->height())),
                            .title = window->caption(),
                            .wclass = window->resourceClass(),
                            .visible = (in_current_workspace && !minimized),
                            .id = window->internalId().toString(QUuid::WithoutBraces)};
        winids << detail;
    }
    return winids;
}

}  // namespace ArkPets
