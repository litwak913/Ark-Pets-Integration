/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#pragma once
#include "dbus.h"

#include "kwin/plugin.h"

#include <QDBusContext>
#include <qcontainerfwd.h>

#define AP_FIND_WINDOW(winuuid, notfound)                                                                                                                      \
    KWin::Workspace::self()->findWindow(QUuid::fromString(winuuid));                                                                                           \
    if (!window || !window->isClient()) {                                                                                                                      \
        qWarning() << "[ArkPets] Window Not Found";                                                                                                            \
        return notfound;                                                                                                                                       \
    }

namespace ArkPets
{

class ArkPetsIntegration : public KWin::Plugin, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KWin.ArkPets")
public:
    explicit ArkPetsIntegration();
    ~ArkPetsIntegration();
    uint Version();
    DetailsList List();
    void Above(const QString &uuid, bool enable);
    void Stick(const QString &uuid, bool enable);
    Details Details(const QString &uuid);
    void Activate(const QString &uuid);
    bool IsActive(const QString &uuid);
    void MoveResize(const QString &uuid, int x, int y, uint w, uint h);
    MousePos Mouse();
    void TestStrutOutline(const QString &uuid);
};

} // namespace ArkPets
