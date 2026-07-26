#pragma once

#include "AppTypes.h"

#include <string>
#include <vector>

void drawUiText(
    Font font,
    const char* text,
    float x,
    float y,
    float fontSize,
    Color color,
    float spacing = 1.0f
);

Vector2 measureUiText(
    Font font,
    const char* text,
    float fontSize,
    float spacing = 1.0f
);

bool button(
    Font font,
    Rectangle bounds,
    const char* text,
    Color normalColor,
    Color hoverColor,
    bool enabled = true
);

bool navigationButton(
    Font font,
    Rectangle bounds,
    const char* text,
    bool selected
);

void panel(Rectangle bounds);

bool checkbox(
    Font font,
    Rectangle bounds,
    const char* label,
    bool& checked
);

bool selectionButton(
    Font font,
    Rectangle bounds,
    const char* text,
    bool selected
);

std::vector<FontOption> loadFonts();

int findSelectedFont(
    const std::vector<FontOption>& fonts,
    const std::string& selectedFontName
);

Font getActiveFont(
    const std::vector<FontOption>& fonts,
    int selectedFont
);
