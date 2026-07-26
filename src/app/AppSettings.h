#pragma once

#include "skyblock/ContestFilter.h"

#include <filesystem>
#include <string>

struct AppSettings
{
    std::string fontName = "Pixel Game";

    skyblock::ContestFilter contestFilter{
        {"Pumpkin", "Melon"},
        skyblock::MatchMode::Any
    };
};

[[nodiscard]] AppSettings loadAppSettings(
    const std::filesystem::path& settingsPath
);

void saveAppSettings(
    const std::filesystem::path& settingsPath,
    const AppSettings& settings
);
