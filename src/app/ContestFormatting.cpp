#include "ContestFormatting.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_set>

std::int64_t currentUnixTimestamp()
{
    return
        std::chrono::duration_cast<
            std::chrono::seconds
        >(
            std::chrono::system_clock::now()
                .time_since_epoch()
        ).count();
}

std::string formatLocalTime(
    std::int64_t timestamp,
    const char* format
)
{
    const std::time_t rawTime =
        static_cast<std::time_t>(
            timestamp
        );

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(
        &localTime,
        &rawTime
    );
#else
    localtime_r(
        &rawTime,
        &localTime
    );
#endif

    std::ostringstream output;

    output <<
        std::put_time(
            &localTime,
            format
        );

    return output.str();
}

std::string formatTimeRemaining(
    std::int64_t startTimestamp,
    std::int64_t now
)
{
    const std::int64_t endTimestamp =
        startTimestamp + 20 * 60;

    if (
        now >= startTimestamp &&
        now < endTimestamp
    )
    {
        const std::int64_t secondsLeft =
            endTimestamp - now;

        const std::int64_t minutes =
            secondsLeft / 60;

        const std::int64_t seconds =
            secondsLeft % 60;

        std::ostringstream output;
        output
            << "In progress - "
            << minutes
            << "m "
            << std::setw(2)
            << std::setfill('0')
            << seconds
            << "s left";

        return output.str();
    }

    const std::int64_t secondsRemaining =
        startTimestamp - now;

    if (secondsRemaining <= 0)
    {
        return "Ended";
    }

    const std::int64_t days =
        secondsRemaining /
        (24 * 60 * 60);

    const std::int64_t hours =
        (
            secondsRemaining %
            (24 * 60 * 60)
        ) /
        (60 * 60);

    const std::int64_t minutes =
        (
            secondsRemaining %
            (60 * 60)
        ) /
        60;

    const std::int64_t seconds =
        secondsRemaining % 60;

    std::ostringstream output;
    output << "Starts in ";

    if (days > 0)
    {
        output
            << days
            << "d ";
    }

    if (
        days > 0 ||
        hours > 0
    )
    {
        output
            << hours
            << "h ";
    }

    output
        << minutes
        << "m "
        << std::setw(2)
        << std::setfill('0')
        << seconds
        << "s";

    return output.str();
}

std::string joinCrops(
    const std::vector<std::string>& crops
)
{
    std::string result;

    for (
        std::size_t index = 0;
        index < crops.size();
        ++index
    )
    {
        result += crops[index];

        if (
            index + 1 <
            crops.size()
        )
        {
            result += " | ";
        }
    }

    return result;
}

std::string selectedCropSummary(
    const skyblock::ContestFilter& filter
)
{
    if (filter.selectedCrops.empty())
    {
        return "Showing every crop";
    }

    std::string summary =
        filter.matchMode ==
            skyblock::MatchMode::Any
            ? "Any: "
            : "All: ";

    for (
        std::size_t index = 0;
        index <
            filter.selectedCrops.size();
        ++index
    )
    {
        summary +=
            filter.selectedCrops[index];

        if (
            index + 1 <
            filter.selectedCrops.size()
        )
        {
            summary += ", ";
        }
    }

    return summary;
}

void mergeDiscoveredCrops(
    std::vector<CropFilterOption>& options,
    const std::vector<skyblock::JacobContest>& contests
)
{
    std::unordered_set<std::string> known;

    for (
        const CropFilterOption& option :
        options
    )
    {
        known.insert(
            skyblock::normalizeCropName(
                option.name
            )
        );
    }

    for (
        const skyblock::JacobContest& contest :
        contests
    )
    {
        for (
            const std::string& crop :
            contest.crops
        )
        {
            const std::string normalized =
                skyblock::normalizeCropName(
                    crop
                );

            if (!known.contains(normalized))
            {
                options.push_back(
                    CropFilterOption{
                        crop,
                        false
                    }
                );

                known.insert(normalized);
            }
        }
    }

    std::sort(
        options.begin(),
        options.end(),
        [](
            const CropFilterOption& first,
            const CropFilterOption& second
        )
        {
            return first.name < second.name;
        }
    );
}

skyblock::ContestFilter buildFilter(
    const std::vector<CropFilterOption>& options,
    skyblock::MatchMode matchMode
)
{
    skyblock::ContestFilter filter;
    filter.matchMode = matchMode;

    for (
        const CropFilterOption& option :
        options
    )
    {
        if (option.selected)
        {
            filter.selectedCrops.push_back(
                option.name
            );
        }
    }

    return filter;
}

std::vector<CropFilterOption> createCropOptions(
    const skyblock::ContestFilter& filter
)
{
    const std::array<const char*, 13>
        knownCrops{
            "Cactus",
            "Carrot",
            "Cocoa Beans",
            "Melon",
            "Moonflower",
            "Mushroom",
            "Nether Wart",
            "Potato",
            "Pumpkin",
            "Sugar Cane",
            "Sunflower",
            "Wheat",
            "Wild Rose"
        };

    std::unordered_set<std::string>
        selected;

    for (
        const std::string& crop :
        filter.selectedCrops
    )
    {
        selected.insert(
            skyblock::normalizeCropName(
                crop
            )
        );
    }

    std::vector<CropFilterOption>
        options;

    for (
        const char* crop :
        knownCrops
    )
    {
        options.push_back(
            CropFilterOption{
                crop,
                selected.contains(
                    skyblock::normalizeCropName(
                        crop
                    )
                )
            }
        );
    }

    return options;
}

std::string formatLastUpdated(
    std::int64_t timestamp
)
{
    if (timestamp == 0)
    {
        return "Never";
    }

    return
        formatLocalTime(
            timestamp,
            "%I:%M:%S %p"
        );
}
