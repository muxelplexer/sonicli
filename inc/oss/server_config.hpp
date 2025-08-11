#pragma once
#include "crypto/password.hpp"
#include <cpr/parameters.h>
#include <memory>
#include <string>
#include <string_view>

namespace oss
{
    namespace
    {
        constexpr static std::string_view client_id{"sonicli/0.0.1"};
    }
    struct server_config
    {
        server_config(const std::string& user, const std::string& url);
        bool login();

        std::string url_string{};
        std::string user{};
        std::unique_ptr<crypto::password> password{nullptr};
        std::unique_ptr<cpr::Parameters> parameters{std::make_unique<cpr::Parameters>()};
    };
}
