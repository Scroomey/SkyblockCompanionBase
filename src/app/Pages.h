#pragma once

#include "AppSettings.h"
#include "AppTypes.h"

#include <vector>

void drawDashboard(
    Font font,
    const ContestApiState& apiState,
    const skyblock::ContestFilter& filter,
    bool fetchInProgress
);

bool drawContests(
    Font font,
    const ContestApiState& apiState,
    const skyblock::ContestFilter& filter,
    ContestListState& listState,
    bool fetchInProgress
);

bool drawFilters(
    Font font,
    std::vector<CropFilterOption>& crops,
    skyblock::MatchMode& matchMode,
    bool& dirty
);

void drawSettings(Font font);

bool fontCard(
    const FontOption& option,
    Rectangle bounds,
    bool selected
);

void drawCustomization(
    Font activeFont,
    const std::vector<FontOption>& fonts,
    int& selectedFont,
    AppSettings& settings,
    const fs::path& settingsPath
);
