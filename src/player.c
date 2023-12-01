#include "player.h"

#include "objectPool.h"
#include "raylib.h"

extern ObjectPool objectPool;

bool grounded = false;
bool jumpReleased = false;
float coyoteTimer = 0.0f;
bool canJump = false;

const float coyoteTime = 0.125f;
const float gravity = 500.0f;
const float fallingGravityMultiplier = 2.5f;

static void PlayerInput(Object *const player) {

    if (IsKeyPressed(KEY_SPACE) && canJump) {
        player->velocity->y = -400;
        canJump = false;
        jumpReleased = false;
    }

    if (IsKeyReleased(KEY_SPACE)) {
        jumpReleased = true;
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

static void CoyoteTimer(float deltaTime) {
    if (grounded) {
        coyoteTimer = 0.0f;
        canJump = true;
        return;
    }

    coyoteTimer += deltaTime;
    if (coyoteTimer >= coyoteTime) {
        canJump = false;
    }
}

static void VariableGravity(Object *const player) {
    if (player->velocity->y > 0) {
        player->acceleration->y = gravity * fallingGravityMultiplier;
    } else if (jumpReleased) {
        player->acceleration->y = gravity * fallingGravityMultiplier;
    } else {
        player->acceleration->y = gravity;
    }

    if (grounded) {
        jumpReleased = false;
    }
}

static void SideCollision(Object *const player, float intersectionWidth) {
    float direction = player->velocity->x > 0 ? 1 : -1;
    player->position->x -= intersectionWidth * direction;
}

static void VerticalCollision(Object *const player, float intersectionHeight) {
    float direction = player->velocity->y > 0 ? 1 : -1;

    if (direction > 0) {
        grounded = true;
    }

    player->position->y -= intersectionHeight * direction;
    player->velocity->y = 0;
}

static void Collisions(Object *const player) {
    Vector2 *position = player->position;
    Vector2 *velocity = player->velocity;
    Rectangle bounds = *player->bounds;
    grounded = false;

    for (int i = 0; i < objectPool.top; ++i) {
        if (i == player->id) {
            continue;
        }

        if (velocity->x == 0 && velocity->y == 0) {
            continue;
        }

        Rectangle other = objectPool.bounds[i];
        if (!CheckCollisionRecs(bounds, other)) {
            continue;
        }

        Rectangle intersection = GetCollisionRec(bounds, other);
        if (intersection.width == 0 || intersection.height == 0) {
            continue;
        }

        bool sideCollision = intersection.width < intersection.height;
        if (sideCollision) {
            SideCollision(player, intersection.width);
        } else {
            VerticalCollision(player, intersection.height);
        }
    }
}

void PlayerUpdate(Object *const player, float deltaTime) {
    Collisions(player);
    CoyoteTimer(deltaTime);
    PlayerInput(player);
    VariableGravity(player);
}
