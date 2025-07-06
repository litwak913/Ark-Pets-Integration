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

struct APMousePos {
    int x;
    int y;
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

inline const QDBusArgument &operator<<(QDBusArgument &argument, const APMousePos &pos)
{
    argument.beginStructure();
    argument << pos.x;
    argument << pos.y;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, APMousePos &pos)
{
    argument.beginStructure();
    argument >> pos.x;
    argument >> pos.y;
    argument.endStructure();
    return argument;
}

}
Q_DECLARE_METATYPE(ArkPets::APDetails)
Q_DECLARE_METATYPE(ArkPets::APDetailsList)
Q_DECLARE_METATYPE(ArkPets::APMousePos)
