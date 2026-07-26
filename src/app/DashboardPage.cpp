#include "Pages.h"

#include "ContestFormatting.h"
#include "UiSupport.h"

#include <string>

void drawDashboard(
    Font font,
    const ContestApiState& apiState,
    const skyblock::ContestFilter& filter,
    bool fetchInProgress
)
{
    drawUiText(
        font,
        "Dashboard",
        248.0f,
        36.0f,
        30.0f,
        RAYWHITE
    );

    drawUiText(
        font,
        "Your SkyBlock activity at a glance",
        248.0f,
        76.0f,
        18.0f,
        Color{165, 155, 180, 255}
    );

    const Rectangle contestPanel{
        248.0f,
        120.0f,
        680.0f,
        190.0f
    };

    panel(contestPanel);

    drawUiText(
        font,
        "NEXT MATCHING JACOB CONTEST",
        272.0f,
        144.0f,
        17.0f,
        Color{214, 137, 255, 255}
    );

    const std::int64_t now =
        currentUnixTimestamp();

    const skyblock::JacobContest* nextContest =
        skyblock::ContestQuery::next(
            apiState.contests,
            now,
            filter
        );

    if (nextContest != nullptr)
    {
        const std::string crops =
            joinCrops(nextContest->crops);

        const std::string remaining =
            formatTimeRemaining(
                nextContest->startTimestamp,
                now
            );

        const std::string startTime =
            formatLocalTime(
                nextContest->startTimestamp,
                "%A at %I:%M:%S %p"
            );

        drawUiText(
            font,
            crops.c_str(),
            272.0f,
            186.0f,
            25.0f,
            RAYWHITE
        );

        drawUiText(
            font,
            remaining.c_str(),
            272.0f,
            228.0f,
            21.0f,
            Color{181, 232, 167, 255}
        );

        drawUiText(
            font,
            startTime.c_str(),
            272.0f,
            267.0f,
            16.0f,
            Color{145, 137, 156, 255}
        );
    }
    else if (fetchInProgress)
    {
        drawUiText(
            font,
            "Loading contest schedule...",
            272.0f,
            190.0f,
            24.0f,
            RAYWHITE
        );
    }
    else
    {
        drawUiText(
            font,
            "No contest matches your filter",
            272.0f,
            190.0f,
            24.0f,
            RAYWHITE
        );

        const std::string summary =
            selectedCropSummary(filter);

        drawUiText(
            font,
            summary.c_str(),
            272.0f,
            232.0f,
            16.0f,
            Color{244, 180, 109, 255}
        );
    }

    const Rectangle filterPanel{
        248.0f,
        334.0f,
        320.0f,
        145.0f
    };

    panel(filterPanel);

    drawUiText(
        font,
        "Active filter",
        272.0f,
        358.0f,
        20.0f,
        RAYWHITE
    );

    drawUiText(
        font,
        filter.matchMode == skyblock::MatchMode::Any
            ? "Match any selected crop"
            : "Match all selected crops",
        272.0f,
        395.0f,
        17.0f,
        Color{181, 232, 167, 255}
    );

    const std::string filterSummary =
        selectedCropSummary(filter);

    drawUiText(
        font,
        filterSummary.c_str(),
        272.0f,
        429.0f,
        14.0f,
        Color{165, 155, 180, 255}
    );

    const Rectangle apiPanel{
        592.0f,
        334.0f,
        336.0f,
        145.0f
    };

    panel(apiPanel);

    drawUiText(
        font,
        "Contest data",
        616.0f,
        358.0f,
        20.0f,
        RAYWHITE
    );

    std::string statusText;
    Color statusColor;

    if (fetchInProgress)
    {
        statusText = "Refreshing...";
        statusColor = Color{244, 180, 109, 255};
    }
    else if (apiState.status == ApiStatus::Ready)
    {
        statusText = "Connected";
        statusColor = Color{181, 232, 167, 255};
    }
    else
    {
        statusText = "Connection error";
        statusColor = Color{244, 130, 130, 255};
    }

    drawUiText(
        font,
        statusText.c_str(),
        616.0f,
        398.0f,
        20.0f,
        statusColor
    );

    const std::string updateText =
        "Updated: " +
        formatLastUpdated(
            apiState.lastUpdatedTimestamp
        );

    drawUiText(
        font,
        updateText.c_str(),
        616.0f,
        430.0f,
        15.0f,
        Color{165, 155, 180, 255}
    );
}
