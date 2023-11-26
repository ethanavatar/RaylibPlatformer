#include <stdio.h>

#include <stdlib.h>

#include "raylib.h"
#include "objectPool.h"
#include "player.h"

long long int frameCount = 0;

const int screenWidth = 800;
const int screenHeight = 450;

void DrawBounds(Rectangle *bounds, Color color) {
    DrawRectangleLines(
        bounds->x,
        bounds->y,
        bounds->width,
        bounds->height,
        color
    );
}

void DrawRect(Rectangle *bounds, Color color) {
    DrawRectangle(
        bounds->x,
        bounds->y,
        bounds->width,
        bounds->height,
        color
    );
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Hello, World!");
    SetTargetFPS(60);

    InitObjectPool(10);

    Object player = CreateObject(50, 50);
    SetObjectPosition(&player, screenWidth / 2, screenHeight / 2);

    Object floor = CreateObject(screenWidth, 10);
    SetObjectPosition(&floor, screenWidth / 2, screenHeight - 5);

    Object platform1 = CreateObject(200, 50);
    SetObjectPosition(&platform1, screenWidth / 2 - 150, screenHeight - 100);

    Object platform2 = CreateObject(200, 50);
    SetObjectPosition(&platform2, screenWidth / 2 + 150, screenHeight - 200);

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();
        float deltaTimeMs = deltaTime * 1000.0f;
        const char *title = TextFormat("Platformer - %.2f ms", deltaTimeMs);
        SetWindowTitle(title);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRect(player.bounds, RED);

            DrawRect(platform1.bounds, BLUE);
            DrawRect(platform2.bounds, BLUE);

            DrawRect(floor.bounds, GRAY);
        EndDrawing();

        PlayerInput(&player);
        TickObjectPool(deltaTime);
        PlayerUpdate(&player, deltaTime);

        frameCount++;
    }

    DestroyObjectPool();
    CloseWindow();

    return 0;
}
