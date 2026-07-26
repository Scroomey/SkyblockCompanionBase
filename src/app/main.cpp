#include <raylib.h>

#include "AppSettings.h"
#include "AppTypes.h"
#include "ContestFormatting.h"
#include "Pages.h"
#include "UiSupport.h"
#include "skyblock/SkyblockCore.h"

#include <chrono>
#include <future>
#include <string>
#include <utility>
#include <vector>

int main()
{
    SetConfigFlags(
        FLAG_WINDOW_RESIZABLE |
        FLAG_VSYNC_HINT
    );

    InitWindow(
        960,
        560,
        "SkyBlock Companion"
    );

    SetWindowMinSize(
        900,
        560
    );

    SetTargetFPS(60);

    const fs::path settingsPath =
        fs::path(
            GetApplicationDirectory()
        ) /
        "settings.json";

    AppSettings settings =
        loadAppSettings(
            settingsPath
        );

    std::vector<FontOption> fonts =
        loadFonts();

    int selectedFont =
        findSelectedFont(
            fonts,
            settings.fontName
        );

    std::vector<CropFilterOption> cropOptions =
        createCropOptions(
            settings.contestFilter
        );

    skyblock::MatchMode matchMode =
        settings.contestFilter.matchMode;

    bool filtersDirty = false;
    Page currentPage = Page::Dashboard;

    ContestApiState apiState;
    ContestListState listState;

    bool fetchInProgress = false;

    std::future<
        skyblock::ContestFetchResult
    > fetchFuture;

    double lastFetchFinishedTime =
        -300.0;

    std::string previousFilterKey;

    auto startContestFetch = [&]()
    {
        if (fetchInProgress)
        {
            return;
        }

        fetchInProgress = true;

        if (apiState.contests.empty())
        {
            apiState.status =
                ApiStatus::Loading;
        }

        apiState.error.clear();

        fetchFuture =
            std::async(
                std::launch::async,
                []()
                {
                    skyblock::EliteContestProvider provider;

                    return
                        provider.fetchCurrentContests();
                }
            );
    };

    startContestFetch();

    while (!WindowShouldClose())
    {
        if (
            fetchInProgress &&
            fetchFuture.valid() &&
            fetchFuture.wait_for(
                std::chrono::seconds(0)
            ) ==
                std::future_status::ready
        )
        {
            skyblock::ContestFetchResult result =
                fetchFuture.get();

            fetchInProgress = false;
            lastFetchFinishedTime = GetTime();

            if (result.success)
            {
                apiState.status =
                    ApiStatus::Ready;

                apiState.contests =
                    std::move(result.contests);

                apiState.skyBlockYear =
                    result.skyBlockYear;

                apiState.reportedCount =
                    result.reportedCount;

                apiState.complete =
                    result.complete;

                apiState.error.clear();

                apiState.lastUpdatedTimestamp =
                    currentUnixTimestamp();

                mergeDiscoveredCrops(
                    cropOptions,
                    apiState.contests
                );
            }
            else
            {
                apiState.status =
                    ApiStatus::Error;

                apiState.error =
                    result.error;
            }
        }

        if (
            !fetchInProgress &&
            GetTime() -
                lastFetchFinishedTime >=
                300.0
        )
        {
            startContestFetch();
        }

        const skyblock::ContestFilter activeFilter =
            buildFilter(
                cropOptions,
                matchMode
            );

        const std::string currentFilterKey =
            selectedCropSummary(activeFilter) +
            (
                activeFilter.matchMode ==
                    skyblock::MatchMode::All
                    ? "|all"
                    : "|any"
            );

        if (currentFilterKey != previousFilterKey)
        {
            listState.scrollOffset = 0.0f;
            listState.loadedCount = 10;
            previousFilterKey = currentFilterKey;
        }

        const Font activeFont =
            getActiveFont(
                fonts,
                selectedFont
            );

        bool refreshRequested = false;

        BeginDrawing();

        ClearBackground(
            Color{21, 18, 28, 255}
        );

        DrawRectangle(
            0,
            0,
            216,
            GetScreenHeight(),
            Color{27, 22, 36, 255}
        );

        drawUiText(
            activeFont,
            "SKYBLOCK",
            24.0f,
            25.0f,
            25.0f,
            Color{230, 188, 255, 255}
        );

        drawUiText(
            activeFont,
            "COMPANION",
            24.0f,
            57.0f,
            21.0f,
            RAYWHITE
        );

        if (
            navigationButton(
                activeFont,
                Rectangle{
                    0.0f,
                    105.0f,
                    216.0f,
                    48.0f
                },
                "Dashboard",
                currentPage == Page::Dashboard
            )
        )
        {
            currentPage = Page::Dashboard;
        }

        if (
            navigationButton(
                activeFont,
                Rectangle{
                    0.0f,
                    155.0f,
                    216.0f,
                    48.0f
                },
                "Contests",
                currentPage == Page::Contests
            )
        )
        {
            currentPage = Page::Contests;
        }

        if (
            navigationButton(
                activeFont,
                Rectangle{
                    0.0f,
                    205.0f,
                    216.0f,
                    48.0f
                },
                "Filters",
                currentPage == Page::Filters
            )
        )
        {
            currentPage = Page::Filters;
        }

        if (
            navigationButton(
                activeFont,
                Rectangle{
                    0.0f,
                    255.0f,
                    216.0f,
                    48.0f
                },
                "Settings",
                currentPage == Page::Settings
            )
        )
        {
            currentPage = Page::Settings;
        }

        if (
            navigationButton(
                activeFont,
                Rectangle{
                    0.0f,
                    305.0f,
                    216.0f,
                    58.0f
                },
                "Customize UI",
                currentPage == Page::Customization
            )
        )
        {
            currentPage = Page::Customization;
        }

        drawUiText(
            activeFont,
            "Version 0.3",
            24.0f,
            static_cast<float>(
                GetScreenHeight() - 38
            ),
            15.0f,
            Color{119, 108, 132, 255}
        );

        switch (currentPage)
        {
            case Page::Dashboard:
                drawDashboard(
                    activeFont,
                    apiState,
                    activeFilter,
                    fetchInProgress
                );
                break;

            case Page::Contests:
                refreshRequested =
                    drawContests(
                        activeFont,
                        apiState,
                        activeFilter,
                        listState,
                        fetchInProgress
                    );
                break;

            case Page::Filters:
            {
                const bool saveRequested =
                    drawFilters(
                        activeFont,
                        cropOptions,
                        matchMode,
                        filtersDirty
                    );

                if (saveRequested)
                {
                    settings.contestFilter =
                        buildFilter(
                            cropOptions,
                            matchMode
                        );

                    saveAppSettings(
                        settingsPath,
                        settings
                    );

                    filtersDirty = false;
                }

                break;
            }

            case Page::Settings:
                drawSettings(activeFont);
                break;

            case Page::Customization:
                drawCustomization(
                    activeFont,
                    fonts,
                    selectedFont,
                    settings,
                    settingsPath
                );
                break;
        }

        EndDrawing();

        if (refreshRequested)
        {
            startContestFetch();
        }
    }

    if (
        fetchInProgress &&
        fetchFuture.valid()
    )
    {
        fetchFuture.wait();
    }

    for (FontOption& option : fonts)
    {
        if (option.loaded)
        {
            UnloadFont(option.font);
        }
    }

    CloseWindow();

    return 0;
}
