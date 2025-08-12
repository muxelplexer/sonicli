#include "ui/login_component.hpp"
#include <cassert>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include "crypto/password.hpp"

import ftxui;
namespace ui
{
    login_component::login_component(ftxui::ScreenInteractive& screen, oss::server_config& config)
        : mScreen(&screen)
        , mConfig{&config}
        , mSubmit{ftxui::Button("Submit", [&]{
            try
            {
                mConfig->password = std::make_unique<crypto::password>(mPassword);
                if (auto err{mConfig->login()})
                {
                    mConnection = false;
                    serverText = std::format("Connection Failed: {}", *err);
                    mConfig->password.reset();
                } else {
                    mConnection = true;
                    mScreen->Exit();
                }
            } catch (const std::exception&)
            {
                serverText = "Connection Failed - could not hash password";
            }
        })}
        , mContainer(ftxui::Container::Vertical(
            {
                mURLInput,
                mUserNameInput,
                mPasswordInput,
                ftxui::Container::Horizontal(
                    {
                        mSubmit,
                        mQuit,
                    }
                    , &buttonIndex
                ),
            },
            &index
        ))
    {

    }

    ftxui::Element login_component::render()
    {
        const auto submit_color{mConnection ?
            ftxui::Color::Green :
            ftxui::Color::Red
        };

        return ftxui::vbox({
            ftxui::text("SubSonic Server Configuration & Login"),
            ftxui::separator(),
            ftxui::hbox({
                ftxui::text("URL:"),
                mURLInput->Render(),
            }),
            ftxui::hbox({
                ftxui::text("Username:"),
                mUserNameInput->Render(),
            }),
            ftxui::hbox({
                ftxui::text("Password:"),
                mPasswordInput->Render(),
            }),
            ftxui::separator(),
            ftxui::text(serverText),
            ftxui::hbox({
                mSubmit->Render() | ftxui::flex,
                mQuit->Render()
            })
        }) | ftxui::borderStyled(submit_color);
    }
}

