/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#include "arkpetsintegration.h"

#include "kwin/virtualdesktops.h"
#include "kwin/window.h"
#include "kwin/workspace.h"
#include <QDBusConnection>
#include <QDebug>

#include "arkpetsadaptor.h"
namespace KWin
{

ArkPetsIntegration::ArkPetsIntegration()
{
    new ArkPetsAdaptor(this);
    qDBusRegisterMetaType<APDetail>();
    qDBusRegisterMetaType<APDetails>();
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/ArkPets"), this);
    QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.KWin"));
    qInfo() << "[ArkPets] Loaded arkpets integration plugin";
}

uint ArkPetsIntegration::Version()
{
    return 2;
}

APDetails ArkPetsIntegration::List()
{
    APDetails winids;
    const auto current_desktop = VirtualDesktopManager::self()->currentDesktop()->id();
    for (const Window *window : Workspace::self()->stackingOrder()) {
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
        APDetail detail = {.x = qRound(window->x()),
                           .y = qRound(window->y()),
                           .w = (uint)qRound(window->width()),
                           .h = (uint)qRound(window->height()),
                           .title = window->caption(),
                           .wclass = window->resourceClass(),
                           .visible = (in_current_workspace && !minimized),
                           .id = window->internalId().toString(QUuid::WithoutBraces)};
        winids << detail;
    }
    return winids;
}

void ArkPetsIntegration::Above(const QString &uuid)
{
    const auto window = AP_FIND_WINDOW(uuid, ) window->setKeepAbove(true);
}

void ArkPetsIntegration::Unabove(const QString &uuid)
{
    const auto window = AP_FIND_WINDOW(uuid, ) window->setKeepAbove(false);
}

void ArkPetsIntegration::Alpha(const QString &uuid, qreal alpha)
{
    const auto window = AP_FIND_WINDOW(uuid, ) window->setOpacity(alpha);
}

APDetail ArkPetsIntegration::Detail(const QString &uuid)
{
    const auto window = Workspace::self()->findWindow(QUuid::fromString(uuid));
    if (!window || !window->isClient()) {
        qWarning() << "[ArkPets] Not Found";
        return {0, 0, 0, 0, QStringLiteral(""), QStringLiteral(""), false, QStringLiteral("")};
    }
    const auto current_desktop = VirtualDesktopManager::self()->currentDesktop()->id();
    auto rect = window->clientGeometry();
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
    APDetail detail = {.x = qRound(rect.x()),
                       .y = qRound(rect.y()),
                       .w = (uint)qRound(rect.width()),
                       .h = (uint)qRound(rect.height()),
                       .title = window->caption(),
                       .wclass = window->resourceClass(),
                       .visible = (in_current_workspace && !minimized),
                       .id = window->internalId().toString(QUuid::WithoutBraces)};
    return detail;
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

ArkPetsIntegration::~ArkPetsIntegration()
{
    qInfo() << "[ArkPets] Unloaded arkpets integration";
}

} // namespace KWin

#include "moc_arkpetsintegration.cpp"
