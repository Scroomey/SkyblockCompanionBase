#include "Pages.h"

#include "UiSupport.h"

bool fontCard(
    const FontOption& option,
    Rectangle bounds,
    bool selected
)
{
    const Vector2 mousePosition =
        GetMousePosition();

    const bool hovered =
        CheckCollisionPointRec(
            mousePosition,
            bounds
        );

    DrawRectangleRec(
        bounds,
        selected
            ? Color{73, 52, 96, 255}
            : (
                hovered
                    ? Color{43, 34, 56, 255}
                    : Color{30, 25, 40, 255}
            )
    );

    DrawRectangleLinesEx(
        bounds,
        selected ? 3.0f : 2.0f,
        selected
            ? Color{218, 145, 255, 255}
            : Color{62, 50, 82, 255}
    );

    const Font previewFont =
        option.loaded
            ? option.font
            : GetFontDefault();

    drawUiText(
        previewFont,
        option.name.c_str(),
        bounds.x + 18.0f,
        bounds.y + 13.0f,
        22.0f,
        RAYWHITE
    );

    drawUiText(
        previewFont,
        "Jacob Contest in 10 minutes",
        bounds.x + 18.0f,
        bounds.y + 46.0f,
        18.0f,
        option.loaded
            ? Color{210, 204, 220, 255}
            : Color{244, 130, 130, 255}
    );

    if (selected)
    {
        drawUiText(
            previewFont,
            "SELECTED",
            bounds.x +
                bounds.width -
                108.0f,
            bounds.y + 18.0f,
            14.0f,
            Color{181, 232, 167, 255}
        );
    }

    return
        option.loaded &&
        hovered &&
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        );
}

void drawCustomization(
    Font activeFont,
    const std::vector<FontOption>& fonts,
    int& selectedFont,
    AppSettings& settings,
    const fs::path& settingsPath
)
{
    drawUiText(
        activeFont,
        "UI Customization",
        248.0f,
        24.0f,
        30.0f,
        RAYWHITE
    );

    drawUiText(
        activeFont,
        "Choose the font used throughout the application",
        248.0f,
        63.0f,
        17.0f,
        Color{165, 155, 180, 255}
    );

    float y = 100.0f;

    for (
        std::size_t index = 0;
        index < fonts.size();
        ++index
    )
    {
        const Rectangle card{
            248.0f,
            y,
            680.0f,
            77.0f
        };

        if (
            fontCard(
                fonts[index],
                card,
                selectedFont ==
                    static_cast<int>(index)
            )
        )
        {
            selectedFont =
                static_cast<int>(index);

            settings.fontName =
                fonts[index].name;

            saveAppSettings(
                settingsPath,
                settings
            );
        }

        y += 86.0f;
    }
}
