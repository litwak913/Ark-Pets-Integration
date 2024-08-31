/** Copyright (c) 2024, Litwak913
 * At GPL-3.0 License
 */

#include "arkpetsintegration.hpp"

#include "kwin/workspace.h"
#include "kwin/window.h"
#include "kwin/virtualdesktops.h"
#include <QDebug>
#include <QDBusConnection>

#include "arkpetsadaptor.h"
namespace KWin
{

ArkPetsIntegration::ArkPetsIntegration()
{
    new ArkPetsAdaptor(this);
    qDBusRegisterMetaType<APTitleClass>();
    qDBusRegisterMetaType<APRect>();
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/ArkPets"), this);
    QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.KWin"));
    qDebug() << "[ArkPets] Loaded arkpets integration plugin";
}

uint ArkPetsIntegration::Version() {
    return 1;
}

QStringList ArkPetsIntegration::List(){
    QStringList winids;
    for (const Window *window : Workspace::self()->stackingOrder()) {
        winids.append(window->internalId().toString(QUuid::WithoutBraces));
    }
    return winids;
}

APTitleClass ArkPetsIntegration::Text(const QString &uuid){
    const auto window = Workspace::self()->findWindow(QUuid::fromString(uuid));
    if (!window || !window->isClient()) {
        qDebug() << "[ArkPets] Not Found";
        return {QLatin1String(""),QLatin1String("")};
    }
    APTitleClass txt {
        .title=window->caption(),
        .wclass=window->resourceClass()
    };
    return txt;
}

void ArkPetsIntegration::Above(const QString &uuid){
    const auto window = AP_FIND_WINDOW(uuid, )
    window->setKeepAbove(true);
}

void ArkPetsIntegration::Unabove(const QString &uuid){
    const auto window = AP_FIND_WINDOW(uuid, )
    window->setKeepAbove(false);
}

void ArkPetsIntegration::Alpha(const QString &uuid, qreal alpha){
    const auto window = AP_FIND_WINDOW(uuid, )
    window->setOpacity(alpha);
}

APRect ArkPetsIntegration::Rect(const QString &uuid){
    const auto window = Workspace::self()->findWindow(QUuid::fromString(uuid));
    if (!window || !window->isClient()) {
        qDebug() << "[ArkPets] Not Found";
        return {0,0,0,0};
    }
    auto rect=window->clientGeometry();
    APRect ret = {
        .x=qRound(rect.x()),
        .y=qRound(rect.y()),
        .w=(uint)qRound(rect.width()),
        .h=(uint)qRound(rect.height())
    };
    return ret;
}

bool ArkPetsIntegration::IsActive(const QString &uuid){
    const auto window = AP_FIND_WINDOW(uuid, false)
    const auto active = Workspace::self()->activeWindow();
    return window->internalId() == active->internalId();
}

void ArkPetsIntegration::Activate(const QString &uuid){
    const auto window = AP_FIND_WINDOW(uuid, )
    Workspace::self()->activateWindow(window);
}

bool ArkPetsIntegration::Visible(const QString &uuid){
    const auto window = AP_FIND_WINDOW(uuid, false)
    const auto current_desktop = VirtualDesktopManager::self()->currentDesktop()->id();
    
    bool minimized = window->isMinimized();
    bool in_current_workspace = false;
    QStringList winids=window->desktopIds();
    if (winids.isEmpty()){
        in_current_workspace=true; //all desktop
    } else {
        if (winids.contains(current_desktop)) {
            in_current_workspace=true;
        }
    }
    return (in_current_workspace && !minimized);
}

void ArkPetsIntegration::MoveResize(const QString &uuid, int x, int y, uint w, uint h){
    const auto window = AP_FIND_WINDOW(uuid, )
    QRectF rect(x, y, w, h);
    window->moveResize(rect);
}

ArkPetsIntegration::~ArkPetsIntegration(){
    qDebug() << "[ArkPets] Unloaded arkpets integration";
}

} // namespace KWin

#include "moc_arkpetsintegration.cpp"
