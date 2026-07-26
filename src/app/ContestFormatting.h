#pragma once

#include "AppTypes.h"

#include <cstdint>
#include <string>
#include <vector>

std::int64_t currentUnixTimestamp();

std::string formatLocalTime(
    std::int64_t timestamp,
    const char* format
);

std::string formatTimeRemaining(
    std::int64_t startTimestamp,
    std::int64_t now
);

std::string joinCrops(
    const std::vector<std::string>& crops
);

std::string selectedCropSummary(
    const skyblock::ContestFilter& filter
);

void mergeDiscoveredCrops(
    std::vector<CropFilterOption>& options,
    const std::vector<skyblock::JacobContest>& contests
);

skyblock::ContestFilter buildFilter(
    const std::vector<CropFilterOption>& options,
    skyblock::MatchMode matchMode
);

std::vector<CropFilterOption> createCropOptions(
    const skyblock::ContestFilter& filter
);

std::string formatLastUpdated(
    std::int64_t timestamp
);
