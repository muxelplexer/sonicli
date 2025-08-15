#include "oss/server_config.hpp"
#include "oss/endpoints.hpp"
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "util/xdg.hpp"

import nlohmann.json;
import tomlplusplus;

namespace oss
{
    server_config::server_config(std::string user, std::string url)
        : url_string { std::move(url) }
        , user { std::move(user) }
    {
    }

    server_config::server_config(std::string&& user, std::string&& url, std::unique_ptr<crypto::password>&& password)
        : url_string{std::move(url)}
        , user{std::move(user)}
        , password{std::move(password)}
    {
        if (auto log{this->login()}; log)
        {
            throw std::runtime_error(*log);
        }
    }

    std::optional<std::string> server_config::login()
    {
        if (!parameters)
        {
            parameters = std::make_unique<cpr::Parameters>();
        }

        parameters->Add({ "v", "1.1" });
        parameters->Add({ "f", "json" });
        parameters->Add({ "c", std::string(client_id) });
        parameters->Add({ "u", user });
        if (!password)
        {
            return std::make_optional("No Password!");
        }

        parameters->Add({ "s", password->salt() });
        parameters->Add({ "t", password->hash() });

        const auto response { oss::ping(*this) };
        if (!response.has_value())
        {
            parameters.reset();
            return "Could not reach server";
        }

        if (response->error.has_value())
        {
            return response->error->message.value_or(
                std::format("Got error code from server: {}", response->error->code));
        }

        return std::nullopt;
    }

    std::optional<server_config> server_config::from_file(const std::string_view path) try
    {
        using namespace std::string_literals;
        std::cerr << "Config path: " << path << "\n";
        const auto toml_file{toml::parse_file(path)};

        auto username{toml_file["user"]["username"].value_or(""s)};
        auto url{toml_file["server"]["url"].value_or(""s)};
        if (auto password_val{toml_file["user"]["password"]}; password_val)
        {
            auto password_str{password_val.value<std::string>()};
            std::unique_ptr<crypto::password> password{std::make_unique<crypto::password>(std::string_view{*password_str})};
            return std::make_optional<server_config>(std::move(username), std::move(url), std::move(password));
        }

        return std::make_optional<server_config>(username, url);
    } catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return std::nullopt;
    }

    std::optional<server_config> server_config::from_file()
    {
        std::filesystem::path config_path{util::xdg_config_home().value_or(util::xdg_config_default())};
        config_path /= "sonicli";

        std::cerr << "Config path: " << config_path << "\n";

        if (!std::filesystem::exists(config_path))
        {
            std::cerr << "Creating directory" << "\n";
            std::filesystem::create_directory(config_path);
        }

        config_path /= "config.toml";
        if (!std::filesystem::exists(config_path))
        {
            std::cerr << "No config" << "\n";
            return std::nullopt;
        }
        const auto path_str{config_path.string()};
        return from_file(path_str);
    }
} // namespace oss
