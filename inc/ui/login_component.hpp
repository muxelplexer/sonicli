#pragma once
#include <ftxui/component/component_base.hpp>
#include <string>
#include <vector>

#include "server_config.hpp"
import ftxui;

class login_component final
{
public:
    login_component(ftxui::ScreenInteractive& screen, server_config& config);
    ftxui::Element render();
    inline ftxui::Component component()
    {
        return mContainer;
    }
private:
    ftxui::ScreenInteractive* mScreen{nullptr};
    std::vector<std::string> entries{"login"};
    int index{0};
    server_config* mConfig{nullptr};
    bool mConnection{false};
    std::string serverText{""};
    std::string mPassword{""};

    ftxui::Component mUserNameInput{ftxui::Input(&mConfig->user, "username")};
    ftxui::Component mPasswordInput{ftxui::Input(&mPassword, "", {.password = true})};
    ftxui::Component mURLInput{ftxui::Input(&mConfig->url_string, "https://")};
    ftxui::Component mQuit{ftxui::Button("Quit", [&]{ mScreen->Exit(); })};
    ftxui::Component mSubmit;

    ftxui::Component mContainer;
};
