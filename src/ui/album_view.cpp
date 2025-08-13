#include "ui/album_view.hpp"
#include "oss/endpoints.hpp"
#include <chrono>
#include <iostream>
#include <ratio>
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
        const auto t1{std::chrono::high_resolution_clock::now()};
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
            const auto track_responses{oss::getAlbum(*mConfig, res->album_list->album)};
            if (!track_responses.has_value())
            {
                return {};
            }
            for ([[maybe_unused]] const auto& album : *track_responses)
            {
                std::vector<std::string> tracks{};
                if (album.error.has_value())
                {
                    return {};
                }
                if (album.album.has_value())
                {
                    if (album.album->children.has_value())
                    {
                        for (const auto& track : *album.album->children)
                        {
                            tracks.emplace_back(track.title);
                        }
                    } else
                    {
                    }
                }
                album_tracks.emplace_back(std::move(tracks));
            }
        }

        const auto t2{std::chrono::high_resolution_clock::now()};
        std::chrono::duration<double, std::milli> ms_double{t2 - t1};
        std::chrono::duration<double> s_double{t2 - t1};
        std::cout << std::format("Album Fetch took: {}s/{}ms\n", s_double.count(), ms_double.count());
        return album_tracks;
    }

    ftxui::Component album_view::render()
    {
        return ftxui::Renderer(mContainer, [&]{
            return ftxui::vbox({
                ftxui::text("Albums"),
                ftxui::separator(),
                ftxui::hbox({
                    mAlbumMenu->Render()| ftxui::yframe | ftxui::yflex,
                    mTrackMenu->Render() | ftxui::xflex,
                })
            }) | ftxui::border;
        });
    }
} // namespace ui
