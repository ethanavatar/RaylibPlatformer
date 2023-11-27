#include "player.h"

#include "objectPool.h"
#include "raylib.h"

extern ObjectPool objectPool;

bool playerGrounded = false;
bool canJump = false;
const float coyoteTime = 0.5f;
float coyoteTimer = 0.0f;
int airJumpCount = 0;

const float gravity = 500.0f;
const float fallMultiplier = 2.5f;

void PlayerInput(Object *const player) {

    if (IsKeyPressed(KEY_SPACE) && canJump) {
        player->velocity->y = -400;
        canJump = false;

        if (playerGrounded == false) {
            airJumpCount++;
        }
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

void PlayerUpdate(Object *const player, float deltaTime) {
    // Variable jump height
    if (player->velocity->y > 0) {
        player->acceleration->y = gravity * fallMultiplier;
    } else if (!IsKeyDown(KEY_SPACE)) {
        player->acceleration->y = gravity * fallMultiplier;
    } else {
        player->acceleration->y = gravity;
    }

    playerGrounded = false;

    Rectangle self = *player->bounds;
    for (int i = 0; i < objectPool.top; ++i) {
        if (i == player->id) {
            continue;
        }

        if (player->velocity->x == 0 && player->velocity->y == 0) {
            continue;
        }

        Rectangle other = objectPool.bounds[i];
        if (CheckCollisionRecs(self, other)) {
            Rectangle intersection = GetCollisionRec(self, other);

            if (intersection.width == 0 || intersection.height == 0) {
                continue;
            }

            float xDirection = player->velocity->x > 0 ? 1 : -1;
            float yDirection = player->velocity->y > 0 ? 1 : -1;

            if (intersection.width < intersection.height) {
                player->position->x -= intersection.width * xDirection;;
                player->velocity->x = 0;
            } else {
                player->position->y -= intersection.height * yDirection;
                player->velocity->y = 0;

                playerGrounded = true;
            }
        }

        if (playerGrounded == false) {
            canJump = false;
            coyoteTimer += deltaTime;
            if (airJumpCount == 0 && coyoteTimer < coyoteTime) {
                canJump = true;
            }
        } else {
            coyoteTimer = 0.0f;
            canJump = true;
            airJumpCount = 0;
        }
    }
}
