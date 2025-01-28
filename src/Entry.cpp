#include "Entry.h"
#include "Global.h"
#include "Language.h"

ll::io::LoggerRegistry&         loggerRegistry = ll::io::LoggerRegistry::getInstance();
std::shared_ptr<ll::io::Logger> logger         = loggerRegistry.getOrCreate(MOD_NAME);
std::shared_ptr<ll::io::Logger> deathLogger    = loggerRegistry.getOrCreate("Death");
std::shared_ptr<ll::io::Logger> infoLogger     = loggerRegistry.getOrCreate("Server");


namespace DeathMessages {

Entry& Entry::getInstance() {
    static Entry instance;
    return instance;
}

bool Entry::load() {
    mConfig.emplace();
    if (!ll::config::loadConfig(*mConfig, getSelf().getConfigDir() / u8"config.json")) {
        ll::config::saveConfig(*mConfig, getSelf().getConfigDir() / u8"config.json");
    }
    if (getConfig().ServerSideTranslation.Enabled) {
        loadI18n();
    } else {
        loadResourcePack();
    }
    if (getConfig().FileLog.Enabled) {
        auto&                                     path = getConfig().FileLog.Path;
        ll::Polymorphic<ll::io::PatternFormatter> polymorphicFormatter =
            ll::makePolymorphic<ll::io::PatternFormatter>("[{3:.3%F %T.} {2}][{1}] {0}", false);
        deathLogger->addSink(std::make_shared<ll::io::FileSink>(path, polymorphicFormatter));
    }
    return true;
}

bool Entry::enable() {
    RegisterDamageDefinition();
    ListenEvents();
    return true;
}

bool Entry::disable() { return true; }

bool Entry::unload() {
    mConfig.reset();
    mI18n.reset();
    // getInstance().reset();
    return true;
}

Config& Entry::getConfig() { return mConfig.value(); }

std::optional<GMLIB::Files::I18n::JsonI18n> Entry::getI18n() { return mI18n; }

void Entry::loadI18n() {
    mI18n.emplace(getSelf().getLangDir(), getConfig().ServerSideTranslation.Language);
    mI18n->updateOrCreateLanguage("en_US", en_US);
    mI18n->updateOrCreateLanguage("zh_CN", zh_CN);
    mI18n->loadAllLanguages();
    mI18n->setDefaultLanguage("zh_CN");
}

void Entry::loadResourcePack() {
    GMLIB::Mod::VanillaFix::setFixI18nEnabled();
    auto resource = GMLIB::Files::ResourceLanguage(getSelf().getModDir() / u8"resource", MOD_NAME, 0, 8, 0);
    resource.addLanguage("en_US", en_US);
    resource.addLanguage("zh_CN", zh_CN);
}

} // namespace DeathMessages

LL_REGISTER_MOD(DeathMessages::Entry, DeathMessages::Entry::getInstance());

std::string tr(std::string const& key, std::vector<std::string> const& params) {
    if (auto i18n = DeathMessages::Entry::getInstance().getI18n()) {
        return i18n->get(key, params);
    }
    return ::I18nAPI::get(key, params);
}