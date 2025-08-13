#include "ui/album_view.hpp"
#include "oss/endpoints.hpp"
import ftxui;

namespace ui
{
    album_view::album_view(ftxui::ScreenInteractive& screen, oss::server_config& config)
        : mScreen { &screen }
        , mConfig { &config }
    {
    }

    std::vector<std::string> album_view::get_albums()
    {
        const auto res { oss::getAlbumList(*mConfig) };
        std::vector<std::string> albums{};
        if (!res.has_value())
        {
            return {};
        }

        if (res->error.has_value())
        {
            return {};
        }

        if (res->album_list.has_value())
        {
            albums.reserve(res->album_list->album.size());
            for (const auto& album : res->album_list->album)
            {
                albums.emplace_back(album.title);
            }
        }
        return albums;
    }

    std::vector<std::vector<std::string>> album_view::get_albums_tracks()
    {
        const auto res { oss::getAlbumList(*mConfig) };
        std::vector<std::vector<std::string>> album_tracks{};
        if (!res.has_value())
        {
            return {};
        }

        if (res->error.has_value())
        {
            return {};
        }

        if (res->album_list.has_value())
        {
            album_tracks.reserve(res->album_list->album.size());
            for ([[maybe_unused]] const auto& album : res->album_list->album)
            {
                std::vector<std::string> tracks{};
            }
        }
        return album_tracks;

    }

    ftxui::Element album_view::render()
    {
        return ftxui::vbox({
            ftxui::text("Albums"),
            ftxui::separator(),
            mContainer->Render()
        }) | ftxui::border;
    }
} // namespace ui
