#include "Pages.h"

#include "ContestFormatting.h"
#include "UiSupport.h"

#include <algorithm>
#include <string>

bool drawContests(
    Font font,
    const ContestApiState& apiState,
    const skyblock::ContestFilter& filter,
    ContestListState& listState,
    bool fetchInProgress
)
{
    drawUiText(
        font,
        "Jacob's Contests",
        248.0f,
        24.0f,
        30.0f,
        RAYWHITE
    );

    std::string subtitle =
        "Filtered live schedule";

    if (apiState.skyBlockYear > 0)
    {
        subtitle +=
            " - SkyBlock Year " +
            std::to_string(
                apiState.skyBlockYear
            );
    }

    drawUiText(
        font,
        subtitle.c_str(),
        248.0f,
        62.0f,
        16.0f,
        Color{165, 155, 180, 255}
    );

    const bool refreshClicked =
        button(
            font,
            Rectangle{
                778.0f,
                25.0f,
                150.0f,
                40.0f
            },
            fetchInProgress
                ? "Refreshing..."
                : "Refresh",
            Color{100, 69, 140, 255},
            Color{125, 83, 166, 255},
            !fetchInProgress
        );

    const std::int64_t now =
        currentUnixTimestamp();

    const std::vector<
        const skyblock::JacobContest*
    > matchingContests =
        skyblock::ContestQuery::allUpcoming(
            apiState.contests,
            now,
            filter
        );

    if (
        listState.loadedCount >
        matchingContests.size()
    )
    {
        listState.loadedCount =
            std::max<std::size_t>(
                10,
                matchingContests.size()
            );
    }

    const Rectangle listArea{
        248.0f,
        98.0f,
        680.0f,
        410.0f
    };

    panel(listArea);

    if (matchingContests.empty())
    {
        drawUiText(
            font,
            fetchInProgress
                ? "Downloading contest data..."
                : "No upcoming contests match the filter.",
            274.0f,
            135.0f,
            22.0f,
            RAYWHITE
        );

        const std::string detail =
            fetchInProgress
                ? "The window remains responsive while loading."
                : selectedCropSummary(filter);

        drawUiText(
            font,
            detail.c_str(),
            274.0f,
            180.0f,
            16.0f,
            fetchInProgress
                ? Color{165, 155, 180, 255}
                : Color{244, 180, 109, 255}
        );

        return refreshClicked;
    }

    std::size_t loadedCount =
        std::min(
            listState.loadedCount,
            matchingContests.size()
        );

    constexpr float rowHeight = 68.0f;
    constexpr float rowGap = 8.0f;
    constexpr float rowPitch =
        rowHeight + rowGap;

    float contentHeight =
        static_cast<float>(
            loadedCount
        ) *
        rowPitch;

    float maximumScroll =
        std::max(
            0.0f,
            contentHeight -
                listArea.height +
                rowGap
        );

    const Vector2 mouse =
        GetMousePosition();

    if (
        CheckCollisionPointRec(
            mouse,
            listArea
        )
    )
    {
        const float wheel =
            GetMouseWheelMove();

        if (wheel != 0.0f)
        {
            listState.scrollOffset -=
                wheel * 58.0f;
        }
    }

    listState.scrollOffset =
        std::clamp(
            listState.scrollOffset,
            0.0f,
            maximumScroll
        );

    if (
        loadedCount <
            matchingContests.size() &&
        listState.scrollOffset >=
            maximumScroll - 2.0f
    )
    {
        listState.loadedCount =
            std::min(
                listState.loadedCount + 10,
                matchingContests.size()
            );

        loadedCount =
            std::min(
                listState.loadedCount,
                matchingContests.size()
            );

        contentHeight =
            static_cast<float>(
                loadedCount
            ) *
            rowPitch;

        maximumScroll =
            std::max(
                0.0f,
                contentHeight -
                    listArea.height +
                    rowGap
            );
    }

    BeginScissorMode(
        static_cast<int>(listArea.x),
        static_cast<int>(listArea.y),
        static_cast<int>(listArea.width),
        static_cast<int>(listArea.height)
    );

    for (
        std::size_t index = 0;
        index < loadedCount;
        ++index
    )
    {
        const skyblock::JacobContest& contest =
            *matchingContests[index];

        const float rowY =
            listArea.y +
            static_cast<float>(index) *
                rowPitch -
            listState.scrollOffset;

        if (
            rowY + rowHeight <
                listArea.y ||
            rowY >
                listArea.y +
                listArea.height
        )
        {
            continue;
        }

        const Rectangle row{
            listArea.x + 8.0f,
            rowY,
            listArea.width - 22.0f,
            rowHeight
        };

        DrawRectangleRec(
            row,
            Color{36, 29, 47, 255}
        );

        DrawRectangleLinesEx(
            row,
            1.0f,
            Color{69, 56, 88, 255}
        );

        const std::string time =
            formatLocalTime(
                contest.startTimestamp,
                "%I:%M:%S %p"
            );

        const std::string date =
            formatLocalTime(
                contest.startTimestamp,
                "%a, %b %d"
            );

        const std::string crops =
            joinCrops(contest.crops);

        const std::string remaining =
            formatTimeRemaining(
                contest.startTimestamp,
                now
            );

        const std::string number =
            std::to_string(index + 1) +
            ".";

        drawUiText(
            font,
            number.c_str(),
            row.x + 10.0f,
            row.y + 20.0f,
            15.0f,
            Color{119, 108, 132, 255}
        );

        drawUiText(
            font,
            time.c_str(),
            row.x + 42.0f,
            row.y + 10.0f,
            18.0f,
            Color{214, 137, 255, 255}
        );

        drawUiText(
            font,
            date.c_str(),
            row.x + 42.0f,
            row.y + 39.0f,
            13.0f,
            Color{165, 155, 180, 255}
        );

        drawUiText(
            font,
            crops.c_str(),
            row.x + 205.0f,
            row.y + 10.0f,
            18.0f,
            RAYWHITE
        );

        drawUiText(
            font,
            remaining.c_str(),
            row.x + 205.0f,
            row.y + 39.0f,
            14.0f,
            contest.isActiveAt(now)
                ? Color{181, 232, 167, 255}
                : Color{165, 155, 180, 255}
        );
    }

    EndScissorMode();

    if (maximumScroll > 0.0f)
    {
        const Rectangle scrollbarTrack{
            listArea.x +
                listArea.width -
                10.0f,
            listArea.y + 5.0f,
            5.0f,
            listArea.height - 10.0f
        };

        DrawRectangleRec(
            scrollbarTrack,
            Color{49, 41, 61, 255}
        );

        const float visibleRatio =
            listArea.height /
            contentHeight;

        const float thumbHeight =
            std::max(
                28.0f,
                scrollbarTrack.height *
                    visibleRatio
            );

        const float thumbTravel =
            scrollbarTrack.height -
            thumbHeight;

        const float thumbY =
            scrollbarTrack.y +
            (
                listState.scrollOffset /
                maximumScroll
            ) *
            thumbTravel;

        DrawRectangle(
            static_cast<int>(
                scrollbarTrack.x
            ),
            static_cast<int>(thumbY),
            static_cast<int>(
                scrollbarTrack.width
            ),
            static_cast<int>(
                thumbHeight
            ),
            Color{166, 111, 210, 255}
        );
    }

    std::string footer =
        "Loaded " +
        std::to_string(loadedCount) +
        " of " +
        std::to_string(
            matchingContests.size()
        ) +
        " matching contests";

    if (
        loadedCount <
        matchingContests.size()
    )
    {
        footer +=
            " - scroll to the bottom "
            "to load 10 more";
    }

    drawUiText(
        font,
        footer.c_str(),
        248.0f,
        525.0f,
        14.0f,
        Color{119, 108, 132, 255}
    );

    return refreshClicked;
}
