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
        QRect rect;
        rect = window->strutRect(KWin::StrutAreaTop); // first check strut rect
        if (rect.isNull()) {
            rect=QRect(qRound(window->x()),qRound(window->y()),qRound(window->width()),qRound(window->height()));
        }
        struct Details detail = {.x = rect.x(),
                                 .y = rect.y(),
                                 .w = static_cast<uint>(rect.width()),
                                 .h = static_cast<uint>(rect.height()),
                                 .title = window->caption(),
                                 .wclass = window->resourceClass(),
                                 .visible = (in_current_workspace && !minimized),
                                 .id = window->internalId().toString(QUuid::WithoutBraces)};
        winids << detail;
    }
    return winids;
}

MousePos ArkPetsIntegration::Mouse()
{
    const auto point = Cursors::self()->mouse()->pos().toPoint();
    return {.x = point.x(), .y = point.y()};
}

} // namespace ArkPets
