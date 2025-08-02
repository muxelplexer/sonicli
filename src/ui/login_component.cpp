#include "ui/login_component.hpp"
#include <cassert>
#include <cstdint>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "crypto/md5.hpp"

import ftxui;
namespace ui = ftxui;

login_component::login_component(ftxui::ScreenInteractive& screen, server_config& config)
    : mScreen(&screen)
    , mConfig{&config}
    , mSubmit{ftxui::Button("Submit", [&]{
        const std::string pass{mPassword + mConfig->salt};
        const std::span<const uint8_t> pass_span{reinterpret_cast<const uint8_t*>(pass.data()), pass.size()};
        if (auto val{crypto::md5_digest(pass_span)})
        {
            mConfig->password = val.value();
        }
        mConnection = mConfig->login();
        if (!mConnection)
            serverText = "Connection Failed";
        else
            serverText = "";
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

