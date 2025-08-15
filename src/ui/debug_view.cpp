#include "ui/debug_view.hpp"
#include "oss/endpoints.hpp"
#include <format>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
import ftxui;

namespace ui
{
    debug_view::debug_view(ftxui::ScreenInteractive& screen, oss::server_config& config)
        : mConfig { &config }
        , mScreen { &screen }
        , mContainer(ftxui::Container::Vertical({ mPing, mMusicFolders, mAlbumList, mQuit }, &mIndex))
    {
    }

    ftxui::Element debug_view::render()
    {
        return ftxui::vbox({ ftxui::text("Debugging Page"),
                             ftxui::separator(),
                             ftxui::text(std::format("Connected to server: {}", mConfig->url_string)),
                             ftxui::text(std::format("User: {}", mConfig->user)),
                             ftxui::separator(),
                             ftxui::paragraph(mDebugText),
                             ftxui::separator(),
                             mPing->Render(),
                             mMusicFolders->Render(),
                             mAlbumList->Render(),
                             mQuit->Render() })
               | ftxui::border;
    }

    void debug_view::ping()
    {
        const auto response { oss::ping(*mConfig) };
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
        const auto response { oss::getMusicFolders(*mConfig) };
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
        if (response->music_folders.has_value())
        {
            mDebugText = "";
            for (const auto& folder : *response->music_folders)
            {
                mDebugText += std::format("{} - {}\n", folder.id, folder.name);
            }
        }
    }

    void debug_view::albumList()
    {
        const auto response { oss::getAlbumList(*mConfig) };
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

        if (response->album_list.has_value())
        {
            mDebugText = "";
            for (const auto& album : response->album_list->album)
            {
                mDebugText += std::format("{}\n", album.title);
            }
        }
        else
        {
            mDebugText = "Wo album?";
        }
    }
} // namespace ui
