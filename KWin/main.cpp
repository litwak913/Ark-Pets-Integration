/*
    SPDX-FileCopyrightText: 2024 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "arkpetsintegration.h"

#include <KPluginFactory>

namespace KWin
{

class KWIN_EXPORT ArkPetsIntegrationFactory : public PluginFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginFactory_iid FILE "metadata.json")
    Q_INTERFACES(KWin::PluginFactory)

public:
    explicit ArkPetsIntegrationFactory() = default;

    std::unique_ptr<Plugin> create() const override
    {
        return std::make_unique<ArkPetsIntegration>();
    }
};

} // namespace KWin

#include "main.moc"
