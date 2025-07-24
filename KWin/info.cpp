/* Copyright (c) 2024-2025, Litwak913
   At GPL-3.0 License
*/

#include "arkpetsintegration.h"

#include "kwin/cursor.h"
#include "kwin/virtualdesktops.h"
#include "kwin/window.h"
#include "kwin/workspace.h"

using namespace KWin;
namespace ArkPets
{

Details ArkPetsIntegration::Details(const QString &uuid)
{
    const auto window = Workspace::self()->findWindow(QUuid::fromString(uuid));
    if (!window || !window->isClient()) {
        qWarning() << "[ArkPets] Window Not Found";
        return {0, 0, 0, 0, QLatin1String(""), QLatin1String(""), false, QLatin1String("")};
    }
    bool minimized = window->isMinimized();
    bool in_current_workspace = false;
    if (window->isOnAllDesktops() || window->isOnCurrentDesktop()) {
        in_current_workspace = true;
    }
    struct Details detail = {.x = qRound(window->x()),
                             .y = qRound(window->y()),
                             .w = static_cast<uint>(qRound(window->width())),
                             .h = static_cast<uint>(qRound(window->height())),
                             .title = window->caption(),
                             .wclass = window->resourceClass(),
                             .visible = (in_current_workspace && !minimized),
                             .id = window->internalId().toString(QUuid::WithoutBraces)};
    return detail;
}

DetailsList ArkPetsIntegration::List()
{
    DetailsList winids;
    const auto current_desktop = VirtualDesktopManager::self()->currentDesktop()->id();
    for (const Window *window : Workspace::self()->stackingOrder()) {
        bool minimized = window->isMinimized();
        bool in_current_workspace = false;
        if (window->isOnAllDesktops() || window->isOnCurrentDesktop()) {
            in_current_workspace = true;
        }
        struct Details detail = {.x = 0,
                                 .y = 0,
                                 .w = 0,
                                 .h = 0,
                                 .title = window->caption(),
                                 .wclass = window->resourceClass(),
                                 .visible = (in_current_workspace && !minimized),
                                 .id = window->internalId().toString(QUuid::WithoutBraces)};
        auto struts = window->strutRects();
        if (struts.empty()) {
            detail.x = window->x();
            detail.y = window->y();
            detail.w = static_cast<uint>(window->width());
            detail.h = static_cast<uint>(window->height());
            winids << detail;
            continue;
        } else {
            for (const StrutRect &sr : struts) {
                detail.x = sr.x();
                detail.y = sr.y();
                detail.w = static_cast<uint>(sr.width());
                detail.h = static_cast<uint>(sr.height());
                winids << detail;
            }
            continue;
        }
    }
    return winids;
}

MousePos ArkPetsIntegration::Mouse()
{
    const auto point = Cursors::self()->mouse()->pos().toPoint();
    return {.x = point.x(), .y = point.y()};
}

} // namespace ArkPets
