/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#pragma once

#include <QDBusArgument>
#include <QString>
namespace ArkPets
{

struct APDetails {
    //(iiuussbs)
    int x;
    int y;
    uint w;
    uint h;
    QString title;
    QString wclass;
    bool visible;
    QString id;
};

typedef QList<APDetails> APDetailsList;

inline QDBusArgument &operator<<(QDBusArgument &argument, const APDetails &details)
{
    argument.beginStructure();
    argument << details.x;
    argument << details.y;
    argument << details.w;
    argument << details.h;
    argument << details.title;
    argument << details.wclass;
    argument << details.visible;
    argument << details.id;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, APDetails &details)
{
    argument.beginStructure();
    argument >> details.x;
    argument >> details.y;
    argument >> details.w;
    argument >> details.h;
    argument >> details.title;
    argument >> details.wclass;
    argument >> details.visible;
    argument >> details.id;
    argument.endStructure();
    return argument;
}

}
Q_DECLARE_METATYPE(ArkPets::APDetails)
Q_DECLARE_METATYPE(ArkPets::APDetailsList)
