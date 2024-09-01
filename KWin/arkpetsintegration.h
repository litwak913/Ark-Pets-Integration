/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#pragma once
#include "dbus.h"

#include "kwin/plugin.h"

#include <QDBusContext>

#define AP_FIND_WINDOW(winuuid, notfound)                                                                                                                      \
    Workspace::self()->findWindow(QUuid::fromString(winuuid));                                                                                                 \
    if (!window || !window->isClient()) {                                                                                                                      \
        qWarning() << "[ArkPets] Window Not Found";                                                                                                            \
        return notfound;                                                                                                                                       \
    }

namespace KWin
{

class ArkPetsIntegration : public Plugin, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KWin.ArkPets")
public:
    explicit ArkPetsIntegration();
    ~ArkPetsIntegration();
    uint Version();
    APDetails List();
    void Above(const QString &uuid);
    void Unabove(const QString &uuid);
    void Alpha(const QString &uuid, qreal alpha);
    APDetail Detail(const QString &uuid);
    void Activate(const QString &uuid);
    bool IsActive(const QString &uuid);
    void MoveResize(const QString &uuid, int x, int y, uint w, uint h);
};

} // namespace KWin
