#pragma once
#include "Config.h"
#include "Global.h"

namespace DeathMessages {

class Entry {

public:
    static Entry& getInstance();

    Entry() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    /// @return True if the mod is unloaded successfully.
    bool unload();

    Config& getConfig();

    std::optional<GMLIB::Files::I18n::JsonI18n> getI18n();

    void loadI18n();

    void loadResourcePack();

private:
    ll::mod::NativeMod&                         mSelf;
    std::optional<Config>                       mConfig;
    std::optional<GMLIB::Files::I18n::JsonI18n> mI18n;
};

} // namespace DeathMessages
