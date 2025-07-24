/* Copyright (c) 2024, Litwak913
   At GPL-3.0 License
*/

#pragma once

#include <QDBusArgument>
#include <QString>
namespace ArkPets
{

struct Details {
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

struct MousePos {
    int x;
    int y;
};

typedef QList<Details> DetailsList;

inline QDBusArgument &operator<<(QDBusArgument &argument, const Details &details)
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

inline const QDBusArgument &operator>>(const QDBusArgument &argument, Details &details)
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

inline const QDBusArgument &operator<<(QDBusArgument &argument, const MousePos &pos)
{
    argument.beginStructure();
    argument << pos.x;
    argument << pos.y;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, MousePos &pos)
{
    argument.beginStructure();
    argument >> pos.x;
    argument >> pos.y;
    argument.endStructure();
    return argument;
}

}
Q_DECLARE_METATYPE(ArkPets::Details)
Q_DECLARE_METATYPE(ArkPets::DetailsList)
Q_DECLARE_METATYPE(ArkPets::MousePos)
