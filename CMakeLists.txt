cmake_minimum_required(VERSION 4.0)
project(sonicli
        VERSION 0.0.1)

set(NLOHMANN_JSON_BUILD_MODULES ON)
add_subdirectory(ext/json)

add_subdirectory(ext/cpr)

set(FTXUI_BUILD_MODULES ON)
add_subdirectory(ext/FTXUI)


set(CRYPTOPP_ENABLE_NAMESPACE_WEAK 1)
set(CRYPTOPP_BUILD_TESTING OFF)
add_subdirectory(ext/cryptopp-cmake)

add_executable(sonicli
    src/main.cpp
    src/crypto/md5.cpp
    src/crypto/password.cpp
    src/oss/data/subsonic_response.cpp
    src/oss/endpoints.cpp
    src/oss/server_config.cpp
    src/ui/album_view.cpp
    src/ui/login_component.cpp
)

target_include_directories(sonicli PRIVATE
    inc/
)

target_link_libraries(sonicli PRIVATE
    cryptopp::cryptopp
    cpr::cpr
    ftxui::modules
    nlohmann_json_modules
)
set_target_properties(sonicli
    PROPERTIES
        CXX_STANDARD 20
)
target_compile_options(sonicli PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Weffc++
    -Wno-missing-designated-field-initializers
)
