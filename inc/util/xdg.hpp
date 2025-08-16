#pragma once
#include <optional>
#include <string>

namespace util
{
    std::string xdg_config_default();
    std::optional<std::string> xdg_config_home();
} // namespace util
