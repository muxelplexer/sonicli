#pragma once
#include "crypto/password.hpp"
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

    std::unique_ptr<crypto::password> password{nullptr};
    std::unique_ptr<cpr::Parameters> parameters{std::make_unique<cpr::Parameters>()};
    std::optional<std::string> ping() const;
    std::string url_string{};
    std::string user{};
};
