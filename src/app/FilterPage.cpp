#include "Pages.h"

#include "UiSupport.h"

bool drawFilters(
    Font font,
    std::vector<CropFilterOption>& crops,
    skyblock::MatchMode& matchMode,
    bool& dirty
)
{
    drawUiText(
        font,
        "Contest Filters",
        248.0f,
        28.0f,
        30.0f,
        RAYWHITE
    );

    drawUiText(
        font,
        "Dashboard and contest list update immediately",
        248.0f,
        66.0f,
        16.0f,
        Color{165, 155, 180, 255}
    );

    const Rectangle filterPanel{
        248.0f,
        102.0f,
        680.0f,
        410.0f
    };

    panel(filterPanel);

    const std::size_t rows =
        (crops.size() + 1) / 2;

    for (
        std::size_t index = 0;
        index < crops.size();
        ++index
    )
    {
        const bool rightColumn =
            index >= rows;

        const std::size_t row =
            rightColumn
                ? index - rows
                : index;

        const float x =
            rightColumn
                ? 590.0f
                : 278.0f;

        const float y =
            132.0f +
            static_cast<float>(row) *
                42.0f;

        if (
            checkbox(
                font,
                Rectangle{
                    x,
                    y,
                    23.0f,
                    23.0f
                },
                crops[index].name.c_str(),
                crops[index].selected
            )
        )
        {
            dirty = true;
        }
    }

    drawUiText(
        font,
        "Match mode",
        278.0f,
        429.0f,
        17.0f,
        RAYWHITE
    );

    if (
        selectionButton(
            font,
            Rectangle{
                400.0f,
                419.0f,
                130.0f,
                38.0f
            },
            "Any selected",
            matchMode ==
                skyblock::MatchMode::Any
        )
    )
    {
        matchMode =
            skyblock::MatchMode::Any;

        dirty = true;
    }

    if (
        selectionButton(
            font,
            Rectangle{
                542.0f,
                419.0f,
                130.0f,
                38.0f
            },
            "All selected",
            matchMode ==
                skyblock::MatchMode::All
        )
    )
    {
        matchMode =
            skyblock::MatchMode::All;

        dirty = true;
    }

    const bool saved =
        button(
            font,
            Rectangle{
                710.0f,
                419.0f,
                190.0f,
                38.0f
            },
            dirty
                ? "Save filters"
                : "Filters saved",
            dirty
                ? Color{100, 69, 140, 255}
                : Color{55, 75, 61, 255},
            dirty
                ? Color{125, 83, 166, 255}
                : Color{55, 75, 61, 255},
            dirty
        );

    drawUiText(
        font,
        "No crops selected means every contest is shown.",
        278.0f,
        477.0f,
        14.0f,
        Color{165, 155, 180, 255}
    );

    return saved;
}
