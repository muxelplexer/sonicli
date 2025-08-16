#include "ui/album_view.hpp"
#include "oss/endpoints.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
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
        mAlbums.clear();
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
            for (const auto& album : res->album_list->album)
            {
                mAlbums.emplace_back(album);
            }
        }
        std::vector<std::string> album_titles {};
        album_titles.reserve(mAlbums.size());
        std::ranges::transform(
            mAlbums, std::back_inserter(album_titles), [](const auto& album) { return album.title; });
        return album_titles;
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
            std::ranges::transform(std::views::filter(*track_responses,
                                                      [](const auto& album)
                                                      {
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
                                   [](const auto& album)
                                   {
                                       std::vector<std::string> tracks {};
                                       tracks.reserve(album.album->children->size());
                                       for (const auto& track : *album.album->children)
                                       {
                                           tracks.emplace_back(track.title);
                                       }
                                       return tracks;
                                   });
        }

        return album_tracks;
    }

    std::vector<std::string> album_view::get_current_albums_tracks()
    {
        const auto album_id { mAlbums[mAlbumSelected].id };
        if (mAlbumTracks.contains(album_id))
        {
            std::vector<std::string> track_list {};
            std::ranges::transform(
                mAlbumTracks[album_id], std::back_inserter(track_list), [](const auto& track) { return track.title; });
            return track_list;
        }
        const auto res { oss::getAlbum(*mConfig, album_id) };
        if (!res.has_value())
        {
            return {};
        }

        if (res->error.has_value())
        {
            std::cerr << "ERROR: " << *res->error->message << "\n";
            return {};
        }

        if (!res->album.has_value() || !res->album->children.has_value())
        {
            std::cerr << "ERROR: no album children found!\n";
            return {};
        }

        std::vector<std::string> tracks_strings {};
        tracks_strings.reserve(res->album->children->size());
        for (const auto& track : *res->album->children)
        {
            tracks_strings.emplace_back(track.title);
        }
        mAlbumTracks.emplace(album_id, *res->album->children);
        return tracks_strings;
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
                                                            mTrackMenu->Render() | ftxui::yframe | ftxui::xflex,
                                                        }) })
                                          | ftxui::border;
                               });
    }
} // namespace ui
