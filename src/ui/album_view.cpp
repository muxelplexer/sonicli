#include "ui/album_view.hpp"
#include "oss/endpoints.hpp"
#include <algorithm>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <mpv/client.h>

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

    static std::atomic<bool> stop_thread{false};
    static mpv_handle *mpv = nullptr;
    void album_view::stream_start()
    {
        this->stream_stop();
        std::string url{mConfig->url_string};
        const auto album_id { mAlbumTracks[mAlbums[mAlbumSelected].id][mTrackSelected].id };
        auto params{*mConfig->parameters};
        url += "/rest/stream?";
        params.Add({
                "id", album_id
        });
        url += params.GetContent();

        mStreamThread = std::make_unique<std::thread>([url](){
            mpv = mpv_create();
            mpv_initialize(mpv);


            const char *command[] = {"loadfile", url.c_str(), nullptr};
            mpv_set_property_string(mpv, "vid", "no");
            mpv_command(mpv, command);
            while (!stop_thread) {
                std::cerr << url << std::endl;
                mpv_event *event = mpv_wait_event(mpv, 10000);
                // std::cerr << std::format("event: {}\n", static_cast<int>(event->event_id));
                if (event->event_id == MPV_EVENT_SHUTDOWN)
                {
                    break;
                }
            }

            mpv_terminate_destroy(mpv);
            mpv = nullptr;
        });



    }

    void album_view::stream_stop()
    {
        if (mStreamThread)
        {
            stop_thread.store(true);
            mpv_wakeup(mpv);
            mStreamThread->join();
            mStreamThread.reset();
            stop_thread.store(false);
        }
    }

    ftxui::Component album_view::render()
    {
        return ftxui::Renderer(mContainer,
                               [&]
                               {
                                   return ftxui::vbox({ 
                                                        ftxui::text("Albums"),
                                                        ftxui::separator(),
                                                        ftxui::hbox({
                                                            mAlbumMenu->Render() | ftxui::yframe | ftxui::yflex,
                                                            mTrackMenu->Render() | ftxui::yframe | ftxui::xflex,
                                                        }) })
                                          | ftxui::border;
                               }) | ftxui::CatchEvent([this](const ftxui::Event& event)
                                {
                                    if (event == ftxui::Event::Return)
                                    {
                                        if (!mStreamThread)
                                        {
                                            stream_start();
                                        } else
                                        {
                                            stream_stop();
                                        }
                                        return true;
                                    }
                                    return false;

                                });
    }
} // namespace ui
