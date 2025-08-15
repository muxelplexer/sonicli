#include "ui/album_view.hpp"
#include "oss/endpoints.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <ranges>
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
        std::vector<std::string> albums {};
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
        std::vector<std::vector<std::string>> album_tracks {};
        if (!res.has_value())
        {
            return {};
        }

        if (res->error.has_value())
        {
            std::cerr << "ERROR: " << *res->error->message << "\n";
            return {};
        }

        if (res->album_list.has_value())
        {
            album_tracks.reserve(res->album_list->album.size());
            const auto track_responses { oss::getAlbum(*mConfig, res->album_list->album) };
            if (!track_responses.has_value())
            {
                return {};
            }
            std::ranges::transform(
                std::views::filter(*track_responses, [](const auto& album){
                    if (album.error.has_value())
                    {
                        std::cerr << "ERROR: " << *album.error->message << "\n";
                        return false;
                    }
                    if (!album.album.has_value())
                    {
                        std::cerr << "ERROR: No album\n";
                        return false;
                    }
                    if (!album.album.has_value())
                    {
                        std::cerr << "ERROR: No album\n";
                        return false;
                    }
                    if (!album.album->children.has_value())
                    {
                        std::cerr << "ERROR: No album children\n";
                        return false;
                    }
                    return true;
                }),
                std::back_inserter(album_tracks),
                [](const auto& album) {
                    std::vector<std::string> tracks {};
                    tracks.reserve(album.album->children->size());
                    for (const auto& track : *album.album->children)
                    {
                        tracks.emplace_back(track.title);
                    }
                    return tracks;
                }
            );
        }

        return album_tracks;
    }

    ftxui::Component album_view::render()
    {
        return ftxui::Renderer(mContainer,
                               [&]
                               {
                                   return ftxui::vbox({ ftxui::text("Albums"),
                                                        ftxui::separator(),
                                                        ftxui::hbox({
                                                            mAlbumMenu->Render() | ftxui::yframe | ftxui::yflex,
                                                            mTrackMenu->Render() | ftxui::xflex,
                                                        }) })
                                          | ftxui::border;
                               });
    }
} // namespace ui
