#pragma once

#include <QString>
#include <QDBusArgument>

struct APRect{
    int x;
    int y;
    uint w;
    uint h;
};

struct APTitleClass{
    QString title;
    QString wclass;
};

inline QDBusArgument &operator<<(QDBusArgument &argument, const APRect &rect){
    argument.beginStructure();
    argument << rect.x;
    argument << rect.y;
    argument << rect.w;
    argument << rect.h;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, APRect &rect){
    argument.beginStructure();
    argument >> rect.x;
    argument >> rect.y;
    argument >> rect.w;
    argument >> rect.h;
    argument.endStructure();
    return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const APTitleClass &txt){
    argument.beginStructure();
    argument << txt.title;
    argument << txt.wclass;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, APTitleClass &txt){
    argument.beginStructure();
    argument >> txt.title;
    argument >> txt.wclass;
    argument.endStructure();
    return argument;
}

Q_DECLARE_METATYPE(APRect)
Q_DECLARE_METATYPE(APTitleClass)
