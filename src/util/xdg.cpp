#include "util/xdg.hpp"
#include <cstdlib>
#include <optional>
#include <string>

namespace util
{
    std::string xdg_config_default()
    {
        std::string home { ::getenv("HOME") };
        return home + "/.config";
    }
    std::optional<std::string> xdg_config_home()
    {
        auto* home { ::getenv("XDG_CONFIG_HOME") };
        return (home != nullptr) ? std::make_optional(home) : std::nullopt;
    }
} // namespace util
