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
namespace ArkPets
{

ArkPetsIntegration::ArkPetsIntegration()
{
    new ArkPetsAdaptor(this);
    qDBusRegisterMetaType<APDetails>();
    qDBusRegisterMetaType<APDetailsList>();
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/ArkPets"), this);
    QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.KWin"));
    qInfo() << "[ArkPets] Loaded arkpets integration plugin";
}

uint ArkPetsIntegration::Version()
{
    return 2;
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

APDetails ArkPetsIntegration::Details(const QString &uuid)
{
    const auto window = KWin::Workspace::self()->findWindow(QUuid::fromString(uuid));
    if (!window || !window->isClient()) {
        qWarning() << "[ArkPets] Window Not Found";
        return {0, 0, 0, 0, QStringLiteral(""), QStringLiteral(""), false, QStringLiteral("")};
    }
    const auto current_desktop = KWin::VirtualDesktopManager::self()->currentDesktop()->id();
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
    APDetails detail = {.x = qRound(rect.x()),
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

ArkPetsIntegration::~ArkPetsIntegration()
{
    qInfo() << "[ArkPets] Unloaded arkpets integration";
}

} // namespace ArkPets

#include "moc_arkpetsintegration.cpp"
