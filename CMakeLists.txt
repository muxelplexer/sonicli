cmake_minimum_required(VERSION 4.0)
project(sonicli
        VERSION 0.0.1)

include(FetchContent)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
FetchContent_Declare(nlohmann_json
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/json
    BINARY_DIR ${CMAKE_BINARY_DIR}/ext/json
    OVERRIDE_FIND_PACKAGE
    SYSTEM
)
FetchContent_MakeAvailable(nlohmann_json)
FetchContent_Declare(cpr
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/cpr
    BINARY_DIR ${CMAKE_BINARY_DIR}/ext/cpr
    OVERRIDE_FIND_PACKAGE
    SYSTEM
)
FetchContent_MakeAvailable(cpr)
FetchContent_Declare(ftxui
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/FTXUI
    BINARY_DIR ${CMAKE_BINARY_DIR}/ext/FTXUI
    OVERRIDE_FIND_PACKAGE
    SYSTEM
)
FetchContent_MakeAvailable(ftxui)

set(CRYPTOPP_BUILD_TESTING OFF CACHE BOOL INTERNAL)
FetchContent_Declare(cryptopp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/cryptopp-cmake
    BINARY_DIR ${CMAKE_BINARY_DIR}/ext/cryptopp-cmake
    OVERRIDE_FIND_PACKAGE
    SYSTEM
)
FetchContent_MakeAvailable(cryptopp)
FetchContent_Declare(tomlplusplus
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/tomlplusplus
    BINARY_DIR ${CMAKE_BINARY_DIR}/ext/tomlplusplus
    OVERRIDE_FIND_PACKAGE
    SYSTEM
)
FetchContent_MakeAvailable(tomlplusplus)

find_package(ftxui REQUIRED)
find_package(PkgConfig REQUIRED) 

pkg_check_modules(MPV REQUIRED IMPORTED_TARGET GLOBAL mpv)

add_executable(sonicli
    src/main.cpp
    src/crypto/md5.cpp
    src/crypto/password.cpp
    src/oss/data/subsonic_response.cpp
    src/oss/endpoints.cpp
    src/oss/server_config.cpp
    src/ui/album_view.cpp
    src/ui/login_component.cpp
    src/util/xdg.cpp
)

target_include_directories(sonicli PRIVATE
    inc/
)

target_link_libraries(sonicli PRIVATE
    PkgConfig::MPV
    cryptopp::cryptopp
    cpr::cpr
    ftxui::dom
    ftxui::screen
    ftxui::component
    nlohmann_json::nlohmann_json
    tomlplusplus::tomlplusplus
)
set_target_properties(sonicli
    PROPERTIES
        CXX_STANDARD 20
)
target_compile_options(sonicli PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Weffc++
    -Wno-missing-designated-field-initializers
)

set_property(TARGET sonicli PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
