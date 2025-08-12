#include "ui/debug_view.hpp"
#include "oss/endpoints.hpp"
#include <format>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
import ftxui;

namespace ui
{
    debug_view::debug_view(ftxui::ScreenInteractive& screen, oss::server_config& config)
        : mScreen{&screen}
        , mConfig{&config}
        , mContainer(ftxui::Container::Vertical(
            {
                mPing,
                mMusicFolders,
                mQuit
            } , &mIndex
        ))
    {

    }

    ftxui::Element debug_view::render()
    {
        return ftxui::vbox({
            ftxui::text("Debugging Page"),
            ftxui::separator(),
            ftxui::text(std::format("Connected to server: {}", mConfig->url_string)),
            ftxui::text(std::format("User: {}", mConfig->user)),
            ftxui::separator(),
            ftxui::paragraph(mDebugText),
            ftxui::separator(),
            mPing->Render(),
            mMusicFolders->Render(),
            mQuit->Render()
        }) | ftxui::border;
    }

    void debug_view::ping()
    {
        const auto response{oss::ping(*mConfig)};
        if (!response.has_value())
        {
            mDebugText = "Could not reach server";
            return;
        }

        if (response->error.has_value())
        {
            mDebugText = *response->error->message;
            return;
        }

        mDebugText = "Pong!";
    }
    void debug_view::musicFolders()
    {
        const auto response{oss::getMusicFolders(*mConfig)};
        if (!response.has_value())
        {
            mDebugText = "Could not reach server";
            return;
        }

        if (response->error.has_value())
        {
            mDebugText = *response->error->message;
            return;
        }
    }
}
