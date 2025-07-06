/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#include "arkpetsintegration.h"

#include <QDebug>

#include "arkpetsadaptor.h"
namespace ArkPets
{

ArkPetsIntegration::ArkPetsIntegration()
{
    new ArkPetsAdaptor(this);
    qDBusRegisterMetaType<APDetails>();
    qDBusRegisterMetaType<APDetailsList>();
    qDBusRegisterMetaType<APMousePos>();
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/ArkPets"), this);
    QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.KWin"));
    qInfo() << "[ArkPets] Loaded arkpets integration plugin";
}

uint ArkPetsIntegration::Version()
{
    return 2;
}

ArkPetsIntegration::~ArkPetsIntegration()
{
    qInfo() << "[ArkPets] Unloaded arkpets integration";
}

} // namespace ArkPets

#include "moc_arkpetsintegration.cpp"
