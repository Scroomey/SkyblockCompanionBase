#include "UiSupport.h"

#include <filesystem>

void drawUiText(
    Font font,
    const char* text,
    float x,
    float y,
    float fontSize,
    Color color,
    float spacing
)
{
    DrawTextEx(
        font,
        text,
        Vector2{x, y},
        fontSize,
        spacing,
        color
    );
}

Vector2 measureUiText(
    Font font,
    const char* text,
    float fontSize,
    float spacing
)
{
    return MeasureTextEx(
        font,
        text,
        fontSize,
        spacing
    );
}

bool button(
    Font font,
    Rectangle bounds,
    const char* text,
    Color normalColor,
    Color hoverColor,
    bool enabled
)
{
    const Vector2 mousePosition =
        GetMousePosition();

    const bool hovered =
        enabled &&
        CheckCollisionPointRec(
            mousePosition,
            bounds
        );

    const bool clicked =
        hovered &&
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        );

    DrawRectangleRec(
        bounds,
        enabled
            ? (
                hovered
                    ? hoverColor
                    : normalColor
            )
            : Color{60, 55, 67, 255}
    );

    DrawRectangleLinesEx(
        bounds,
        2.0f,
        enabled
            ? Color{91, 72, 120, 255}
            : Color{72, 67, 78, 255}
    );

    const Vector2 textSize =
        measureUiText(
            font,
            text,
            18.0f
        );

    drawUiText(
        font,
        text,
        bounds.x +
            bounds.width / 2.0f -
            textSize.x / 2.0f,
        bounds.y +
            bounds.height / 2.0f -
            textSize.y / 2.0f,
        18.0f,
        enabled
            ? RAYWHITE
            : Color{145, 140, 150, 255}
    );

    return clicked;
}

bool navigationButton(
    Font font,
    Rectangle bounds,
    const char* text,
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

    Color background =
        Color{35, 29, 48, 255};

    if (selected)
    {
        background =
            Color{100, 69, 140, 255};
    }
    else if (hovered)
    {
        background =
            Color{55, 43, 72, 255};
    }

    DrawRectangleRec(
        bounds,
        background
    );

    if (selected)
    {
        DrawRectangle(
            static_cast<int>(bounds.x),
            static_cast<int>(bounds.y),
            5,
            static_cast<int>(bounds.height),
            Color{218, 145, 255, 255}
        );
    }

    drawUiText(
        font,
        text,
        bounds.x + 18.0f,
        bounds.y + 15.0f,
        19.0f,
        RAYWHITE
    );

    return
        hovered &&
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        );
}

void panel(Rectangle bounds)
{
    DrawRectangleRec(
        bounds,
        Color{30, 25, 40, 255}
    );

    DrawRectangleLinesEx(
        bounds,
        2.0f,
        Color{62, 50, 82, 255}
    );
}

bool checkbox(
    Font font,
    Rectangle bounds,
    const char* label,
    bool& checked
)
{
    const Vector2 mousePosition =
        GetMousePosition();

    const Rectangle clickArea{
        bounds.x,
        bounds.y,
        bounds.width + 190.0f,
        bounds.height
    };

    const bool hovered =
        CheckCollisionPointRec(
            mousePosition,
            clickArea
        );

    bool changed = false;

    if (
        hovered &&
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        )
    )
    {
        checked = !checked;
        changed = true;
    }

    DrawRectangleRec(
        bounds,
        hovered
            ? Color{71, 55, 91, 255}
            : Color{43, 35, 56, 255}
    );

    DrawRectangleLinesEx(
        bounds,
        2.0f,
        Color{129, 91, 168, 255}
    );

    if (checked)
    {
        DrawRectangle(
            static_cast<int>(
                bounds.x + 5.0f
            ),
            static_cast<int>(
                bounds.y + 5.0f
            ),
            static_cast<int>(
                bounds.width - 10.0f
            ),
            static_cast<int>(
                bounds.height - 10.0f
            ),
            Color{214, 137, 255, 255}
        );
    }

    drawUiText(
        font,
        label,
        bounds.x +
            bounds.width +
            10.0f,
        bounds.y + 2.0f,
        18.0f,
        hovered
            ? RAYWHITE
            : Color{
                210,
                204,
                220,
                255
            }
    );

    return changed;
}

bool selectionButton(
    Font font,
    Rectangle bounds,
    const char* text,
    bool selected
)
{
    const Vector2 mouse =
        GetMousePosition();

    const bool hovered =
        CheckCollisionPointRec(
            mouse,
            bounds
        );

    DrawRectangleRec(
        bounds,
        selected
            ? Color{100, 69, 140, 255}
            : (
                hovered
                    ? Color{55, 43, 72, 255}
                    : Color{35, 29, 48, 255}
            )
    );

    DrawRectangleLinesEx(
        bounds,
        2.0f,
        selected
            ? Color{218, 145, 255, 255}
            : Color{72, 60, 88, 255}
    );

    const Vector2 size =
        measureUiText(
            font,
            text,
            16.0f
        );

    drawUiText(
        font,
        text,
        bounds.x +
            bounds.width / 2.0f -
            size.x / 2.0f,
        bounds.y +
            bounds.height / 2.0f -
            size.y / 2.0f,
        16.0f,
        RAYWHITE
    );

    return
        hovered &&
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        );
}

std::vector<FontOption> loadFonts()
{
    const fs::path fontDirectory =
        fs::path(
            GetApplicationDirectory()
        ) /
        "assets" /
        "fonts";

    std::vector<FontOption> fonts{
        {
            "Pixel Game",
            "Pixel Game.otf",
            {},
            false,
            true
        },
        {
            "Arcade Classic",
            "ARCADECLASSIC.TTF",
            {},
            false,
            true
        },
        {
            "Star Crush",
            "Star Crush.ttf",
            {},
            false,
            false
        },
        {
            "I Hate Comic Sans",
            "IHateComicSans.ttf",
            {},
            false,
            false
        },
        {
            "Quiet Miracle",
            "Quiet Miracle.ttf",
            {},
            false,
            false
        }
    };

    for (FontOption& option : fonts)
    {
        const fs::path fontPath =
            fontDirectory /
            option.fileName;

        if (!fs::exists(fontPath))
        {
            option.font =
                GetFontDefault();

            continue;
        }

        option.font =
            LoadFontEx(
                fontPath
                    .string()
                    .c_str(),
                48,
                nullptr,
                0
            );

        option.loaded =
            option.font.texture.id != 0 &&
            option.font.glyphCount > 0;

        if (option.loaded)
        {
            SetTextureFilter(
                option.font.texture,
                option.pixelStyle
                    ? TEXTURE_FILTER_POINT
                    : TEXTURE_FILTER_BILINEAR
            );
        }
        else
        {
            option.font =
                GetFontDefault();
        }
    }

    return fonts;
}

int findSelectedFont(
    const std::vector<FontOption>& fonts,
    const std::string& selectedFontName
)
{
    int fallback = 0;

    for (
        std::size_t index = 0;
        index < fonts.size();
        ++index
    )
    {
        if (fonts[index].loaded)
        {
            fallback =
                static_cast<int>(index);

            break;
        }
    }

    for (
        std::size_t index = 0;
        index < fonts.size();
        ++index
    )
    {
        if (
            fonts[index].loaded &&
            fonts[index].name ==
                selectedFontName
        )
        {
            return
                static_cast<int>(index);
        }
    }

    return fallback;
}

Font getActiveFont(
    const std::vector<FontOption>& fonts,
    int selectedFont
)
{
    if (
        selectedFont >= 0 &&
        selectedFont <
            static_cast<int>(
                fonts.size()
            ) &&
        fonts[selectedFont].loaded
    )
    {
        return
            fonts[selectedFont].font;
    }

    return GetFontDefault();
}
