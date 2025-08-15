#pragma once
#include "crypto/password.hpp"
#include <cpr/parameters.h>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace oss
{
    namespace
    {
        constexpr std::string_view client_id { "sonicli/0.0.1" };
    }
    struct server_config
    {
        server_config(std::string user, std::string url);
        server_config(std::string&& user, std::string&& url, std::unique_ptr<crypto::password>&& password);
        std::optional<std::string> login();

        std::string url_string;
        std::string user;
        std::unique_ptr<crypto::password> password { nullptr };
        std::unique_ptr<cpr::Parameters> parameters { std::make_unique<cpr::Parameters>() };

        static std::optional<server_config> from_file(const std::string_view path);
        static std::optional<server_config> from_file();
    };
} // namespace oss
