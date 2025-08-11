#include "oss/server_config.hpp"
#include "cpr/cpr.h"
#include <memory>

import nlohmann.json;

namespace oss
{
    server_config::server_config(const std::string& user, const std::string& url)
        : user{user},  url_string{url}
    {
    }

    bool server_config::login()
    {
        if (!parameters)
            parameters = std::make_unique<cpr::Parameters>();

        parameters->Add({"v","1.1"});
        parameters->Add({"f","json"});
        parameters->Add({"c",std::string(client_id)});
        parameters->Add({"u",user});
        parameters->Add({"s",salt});
        parameters->Add({"t",password});

        if (this->ping().has_value())
        {
            parameters.reset();
            return false;
        } else
        {
            return true;
        }
    }

    std::optional<std::string> server_config::ping() const try
    {
        using nlohmann::json;
        const auto res{cpr::Get(cpr::Url(url_string  + "/rest/ping.view"), *parameters)};

        if (res.error)
        {
            return res.error.message;
        }

        const auto body(json::parse(res.text));
        const auto status = body.at("subsonic-response").at("status").get<std::string>();

        if (status == "ok")
            return std::nullopt;
        else 
            return body.at("subsonic-response").at("error").at("message").get<std::string>();

    } catch (const std::exception& ex)
    {
        return ex.what();
    }
}

