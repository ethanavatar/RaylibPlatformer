#include "objectPool.h"
#include "raylib.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool playerGrounded = false;

ObjectPool objectPool = {0};

void InitObjectPool(int count) {
    objectPool.positions = (Vector2 *) malloc(count * sizeof(Vector2));
    memset(objectPool.positions, 0, count * sizeof(Vector2));

    objectPool.velocities = (Vector2 *) malloc(count * sizeof(Vector2));
    memset(objectPool.velocities, 0, count * sizeof(Vector2));

    objectPool.accelerations = (Vector2 *) malloc(count * sizeof(Vector2));
    memset(objectPool.accelerations, 0, count * sizeof(Vector2));

    objectPool.bounds = (Rectangle *) malloc(count * sizeof(Rectangle));
    memset(objectPool.bounds, 0, count * sizeof(Rectangle));

    objectPool.count = count;
    objectPool.top = 0;
}

void DestroyObjectPool(void) {
    free(objectPool.positions);
    free(objectPool.velocities);
    free(objectPool.accelerations);
    free(objectPool.bounds);
}

void TickObjectPool(float deltaTime) {
    for (int i = 0; i < objectPool.top; ++i) {
        objectPool.velocities[i].x += objectPool.accelerations[i].x * deltaTime;
        objectPool.velocities[i].y += objectPool.accelerations[i].y * deltaTime;
        objectPool.positions[i].x += objectPool.velocities[i].x * deltaTime;
        objectPool.positions[i].y += objectPool.velocities[i].y * deltaTime;

        objectPool.bounds[i].x = objectPool.positions[i].x - objectPool.bounds[i].width / 2;
        objectPool.bounds[i].y = objectPool.positions[i].y - objectPool.bounds[i].height / 2;

        Rectangle self = objectPool.bounds[i];
        for (int j = 0; j < objectPool.top; ++j) {
            if (i == j) {
                continue;
            }

            if (objectPool.velocities[i].x == 0 && objectPool.velocities[i].y == 0) {
                continue;
            }

            Rectangle other = objectPool.bounds[j];
            if (CheckCollisionRecs(self, other)) {
                Rectangle intersection = GetCollisionRec(self, other);

                float xDirection = objectPool.velocities[i].x > 0 ? 1 : -1;
                float yDirection = objectPool.velocities[i].y > 0 ? 1 : -1;

                if (intersection.width < intersection.height) {
                    objectPool.positions[i].x -= intersection.width * xDirection;;
                    objectPool.velocities[i].x = 0;
                } else {
                    objectPool.positions[i].y -= intersection.height * yDirection;
                    objectPool.velocities[i].y = 0;
                }

                if (i == 0) {
                    if (yDirection == 1) {
                        playerGrounded = true;
                    } else {
                        playerGrounded = false;
                    }
                }
            }
        }

        objectPool.bounds[i].x = objectPool.positions[i].x - objectPool.bounds[i].width / 2;
        objectPool.bounds[i].y = objectPool.positions[i].y - objectPool.bounds[i].height / 2;
    }
}

Object CreateObject(int width, int height) {
    Object object;

    object.id = objectPool.top;

    object.position = &objectPool.positions[objectPool.top];
    object.velocity = &objectPool.velocities[objectPool.top];
    object.acceleration = &objectPool.accelerations[objectPool.top];

    object.bounds = &objectPool.bounds[objectPool.top];
    objectPool.top++;

    object.bounds->width = width;
    object.bounds->height = height;

    return object;
}

void SetObjectPosition(Object *object, int x, int y) {
    object->position->x = x;
    object->position->y = y;
}
