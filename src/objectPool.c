#include "objectPool.h"
#include "raylib.h"

#include <stdlib.h>
#include <string.h>

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
