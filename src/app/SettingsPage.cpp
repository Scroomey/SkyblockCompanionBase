#include "Pages.h"

#include "UiSupport.h"

void drawSettings(Font font)
{
    drawUiText(
        font,
        "Settings",
        248.0f,
        36.0f,
        30.0f,
        RAYWHITE
    );

    drawUiText(
        font,
        "Application preferences",
        248.0f,
        76.0f,
        18.0f,
        Color{165, 155, 180, 255}
    );

    const Rectangle settingsPanel{
        248.0f,
        120.0f,
        680.0f,
        240.0f
    };

    panel(settingsPanel);

    drawUiText(
        font,
        "Notification timing",
        276.0f,
        152.0f,
        20.0f,
        RAYWHITE
    );

    drawUiText(
        font,
        "Notification controls will be added next.",
        276.0f,
        190.0f,
        18.0f,
        Color{185, 177, 198, 255}
    );

    drawUiText(
        font,
        "Reusable core library",
        276.0f,
        248.0f,
        20.0f,
        RAYWHITE
    );

    drawUiText(
        font,
        "API, models, filtering, and queries are separate from raylib.",
        276.0f,
        286.0f,
        17.0f,
        Color{185, 177, 198, 255}
    );
}
