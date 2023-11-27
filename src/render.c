#include "raylib.h""

void DrawBounds(Rectangle *const bounds, Color color) {
    DrawRectangleLines(
        bounds->x,
        bounds->y,
        bounds->width,
        bounds->height,
        color
    );
}

void DrawRect(Rectangle *const bounds, Color color) {
    DrawRectangle(
        bounds->x,
        bounds->y,
        bounds->width,
        bounds->height,
        color
    );
}
