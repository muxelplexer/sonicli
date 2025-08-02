#pragma once
#include <cpr/parameters.h>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace
{
    constexpr static std::string_view client_id{"sonicli/0.0.1"};
}


struct server_config
{
    server_config(const std::string& user, const std::string& url);
    bool login();
    std::optional<std::string> ping() const;

    std::string url_string{};
    std::string user{};
    std::string password{""};
    std::string salt{"12345"};
    std::unique_ptr<cpr::Parameters> parameters{std::make_unique<cpr::Parameters>()};
};
