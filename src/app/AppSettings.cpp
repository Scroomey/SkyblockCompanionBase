#include "AppSettings.h"

#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

AppSettings loadAppSettings(
    const std::filesystem::path& settingsPath
)
{
    AppSettings settings;

    std::ifstream input(settingsPath);

    if (!input)
    {
        return settings;
    }

    try
    {
        json data;
        input >> data;

        settings.fontName =
            data.value(
                "font",
                settings.fontName
            );

        if (
            data.contains("filters") &&
            data.at("filters").is_object()
        )
        {
            const json& filters =
                data.at("filters");

            if (
                filters.contains("selectedCrops") &&
                filters.at("selectedCrops").is_array()
            )
            {
                settings.contestFilter.selectedCrops =
                    filters.at("selectedCrops")
                        .get<std::vector<std::string>>();
            }

            const std::string matchMode =
                filters.value(
                    "matchMode",
                    std::string("any")
                );

            settings.contestFilter.matchMode =
                matchMode == "all"
                    ? skyblock::MatchMode::All
                    : skyblock::MatchMode::Any;
        }
    }
    catch (const std::exception&)
    {
        return AppSettings{};
    }

    return settings;
}

void saveAppSettings(
    const std::filesystem::path& settingsPath,
    const AppSettings& settings
)
{
    const json data{
        {
            "font",
            settings.fontName
        },
        {
            "filters",
            {
                {
                    "selectedCrops",
                    settings
                        .contestFilter
                        .selectedCrops
                },
                {
                    "matchMode",
                    settings
                        .contestFilter
                        .matchMode ==
                            skyblock::MatchMode::All
                        ? "all"
                        : "any"
                }
            }
        }
    };

    std::ofstream output(settingsPath);

    if (output)
    {
        output << data.dump(4);
    }
}
