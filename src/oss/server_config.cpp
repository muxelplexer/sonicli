#include "oss/server_config.hpp"
#include "oss/endpoints.hpp"
#include <memory>
#include <optional>

import nlohmann.json;

namespace oss
{
    server_config::server_config(const std::string& user, const std::string& url)
        : user{user},  url_string{url}
    {
    }

    std::optional<std::string> server_config::login()
    {
        if (!parameters)
            parameters = std::make_unique<cpr::Parameters>();

        parameters->Add({"v","1.1"});
        parameters->Add({"f","json"});
        parameters->Add({"c",std::string(client_id)});
        parameters->Add({"u",user});
        if (!password)
            return std::make_optional("No Password!");

        parameters->Add({"s",password->salt()});
        parameters->Add({"t",password->hash()});

        const auto response{oss::ping(*this)};
        if (!response.has_value())
        {
            parameters.reset();
            return "Could not reach server";
        }

        if (response->error.has_value())
        {
            return *response->error->message;
        }

        return std::nullopt;
    }
}

