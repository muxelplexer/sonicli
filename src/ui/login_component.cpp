#include "ui/login_component.hpp"
#include <cassert>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include "crypto/password.hpp"

import ftxui;
namespace ui = ftxui;

login_component::login_component(ftxui::ScreenInteractive& screen, server_config& config)
    : mScreen(&screen)
    , mConfig{&config}
    , mSubmit{ftxui::Button("Submit", [&]{
        try
        {
            mConfig->password = std::make_unique<crypto::password>(mPassword);
            mConnection = mConfig->login();
            if (!mConnection)
            {
                serverText = "Connection Failed";
                mConfig->password.reset();
            }
            else
                serverText = "";
        } catch (const std::exception&)
        {
            serverText = "Connection Failed - could not hash password";
        }
      })}
    , mContainer(ui::Container::Vertical(
        {
            mURLInput,
            mUserNameInput,
            mPasswordInput,
            mSubmit,
            mQuit,
        },
        &index
    ))
{
    assert(mUserName != nullptr);
    assert(mPassword != nullptr);
    assert(mURL != nullptr);
}

ftxui::Element login_component::render()
{
    const auto submit_color{mConnection ?
        ui::Color::Green :
        ui::Color::Red
    };

    return ui::vbox({
        ui::text("SubSonic Server Configuration & Login"),
        ui::separator(),
        ui::hbox({
            ui::text("URL:"),
            mURLInput->Render(),
        }),
        ui::hbox({
            ui::text("Username:"),
            mUserNameInput->Render(),
        }),
        ui::hbox({
            ui::text("Password:"),
            mPasswordInput->Render(),
        }),
        ui::separator(),
        ui::text(serverText),
        mSubmit->Render(),
        mQuit->Render()
    }) | ui::borderStyled(submit_color);
}

