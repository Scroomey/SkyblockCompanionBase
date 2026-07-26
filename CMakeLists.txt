cmake_minimum_required(VERSION 3.21)

project(SkyblockCompanion LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(SkyblockCore STATIC
    src/core/ContestFilter.cpp
    src/core/ContestQuery.cpp
    src/providers/EliteContestProvider.cpp
)

add_library(Skyblock::Core ALIAS SkyblockCore)

target_include_directories(SkyblockCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SkyblockCore
    PUBLIC
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

add_executable(SkyblockCompanion
    src/app/main.cpp
    src/app/AppSettings.cpp
)

target_include_directories(SkyblockCompanion
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app
)

target_link_libraries(SkyblockCompanion
    PRIVATE
        raylib
        Skyblock::Core
)

add_custom_command(
    TARGET SkyblockCompanion
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:SkyblockCompanion>/assets
)
