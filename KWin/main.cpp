/** Copyright (c) 2024, Litwak913
 * At GPL-3.0 License
 */

#include "arkpetsintegration.hpp"

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
