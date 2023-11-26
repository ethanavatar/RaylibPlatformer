#include <stdlib.h>

#include "raylib.h"
#include "objectPool.h"

const int screenWidth = 800;
const int screenHeight = 600;

void DrawBounds(Rectangle *bounds, Color color) {
    DrawRectangleLines(bounds->x, bounds->y, bounds->width, bounds->height, color);
}

void PlayerInput(Object *player) {
    if (IsKeyPressed(KEY_SPACE)) {
        player->velocity->y = -400;
    }

    if (IsKeyDown(KEY_A)) {
        player->velocity->x = -200;
    }

    if (IsKeyDown(KEY_D)) {
        player->velocity->x = 200;
    }

    if (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D)) {
        player->velocity->x = 0;
    }
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Hello, World!");
    SetTargetFPS(60);

    InitObjectPool(10);

    Object player = CreateObject(50, 50);
    SetObjectPosition(&player, screenWidth / 2, screenHeight / 2);

    Object platform = CreateObject(200, 50);
    SetObjectPosition(&platform, screenWidth / 2, screenHeight - 50);

    float gravity = 500.0f;
    float fallMultiplier = 2.5f;

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();
        float deltaTimeMs = deltaTime * 1000.0f;
        const char *title = TextFormat("Platformer - %.2f ms", deltaTimeMs);
        SetWindowTitle(title);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawBounds(player.bounds, RED);
            DrawBounds(platform.bounds, BLUE);
        EndDrawing();

        PlayerInput(&player);
        TickObjectPool(deltaTime);

        // Variable jump height
        if (player.velocity->y > 0) {
            player.acceleration->y = gravity * fallMultiplier;
        } else if (!IsKeyDown(KEY_SPACE)) {
            player.acceleration->y = gravity * fallMultiplier;
        } else {
            player.acceleration->y = gravity;
        }

        // Player collision with the edge of the screen
        if (player.position->y + player.bounds->height / 2 > screenHeight) {
            player.position->y = screenHeight - player.bounds->height / 2;
            player.velocity->y = 0;
        }
    }

    DestroyObjectPool();
    CloseWindow();

    return 0;
}
