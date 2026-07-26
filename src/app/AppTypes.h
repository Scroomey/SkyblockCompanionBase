#pragma once

#include <raylib.h>

#include "skyblock/SkyblockCore.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

enum class Page
{
    Dashboard,
    Contests,
    Filters,
    Settings,
    Customization
};

enum class ApiStatus
{
    Loading,
    Ready,
    Error
};

struct CropFilterOption
{
    std::string name;
    bool selected = false;
};

struct FontOption
{
    std::string name;
    std::string fileName;
    Font font{};
    bool loaded = false;
    bool pixelStyle = false;
};

struct ContestApiState
{
    ApiStatus status = ApiStatus::Loading;
    std::vector<skyblock::JacobContest> contests;
    int skyBlockYear = 0;
    int reportedCount = 0;
    bool complete = false;
    std::string error;
    std::int64_t lastUpdatedTimestamp = 0;
};

struct ContestListState
{
    float scrollOffset = 0.0f;
    std::size_t loadedCount = 10;
};
