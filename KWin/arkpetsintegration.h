/*
    SPDX-FileCopyrightText: 2024 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once
#include "dbus.h"

#include "kwin/plugin.h"

#include <QDBusContext>

#define AP_FIND_WINDOW(winuuid, notfound) Workspace::self()->findWindow(QUuid::fromString(winuuid)); \
if (!window || !window->isClient()) {\
    qDebug() << "[ArkPets] Not Found";\
    return notfound;\
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
    QStringList List();
    void Alpha(const QString &uuid, qreal alpha);
    void Above(const QString &uuid);
    void Unabove(const QString &uuid);
    bool IsActive(const QString &uuid);
    void Activate(const QString &uuid);
    bool Visible(const QString &uuid);
    APTitleClass Text(const QString &uuid);
    APRect Rect(const QString &uuid);
    void MoveResize(const QString &uuid, int x, int y, uint w, uint h);
};

} // namespace KWin
