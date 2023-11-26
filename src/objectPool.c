#include "objectPool.h"
#include "raylib.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

ObjectPool objectPool = {0};

static Vector2 *GetPosition(int id) {
    return objectPool.positions + id;
}

static Vector2 *GetVelocity(int id) {
    return objectPool.velocities + id;
}

static Vector2 *GetAcceleration(int id) {
    return objectPool.accelerations + id;
}

static Rectangle *GetBounds(int id) {
    return objectPool.bounds + id;
}

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
        GetVelocity(i)->x += GetAcceleration(i)->x * deltaTime;
        GetVelocity(i)->y += GetAcceleration(i)->y * deltaTime;
        GetPosition(i)->x += GetVelocity(i)->x * deltaTime;
        GetPosition(i)->y += GetVelocity(i)->y * deltaTime;

        GetBounds(i)->x = GetPosition(i)->x - GetBounds(i)->width / 2;
        GetBounds(i)->y = GetPosition(i)->y - GetBounds(i)->height / 2;
    }
}

Object CreateObject(int width, int height) {
    Object object;

    object.id = objectPool.top;

    object.position = GetPosition(objectPool.top);
    object.velocity = GetVelocity(objectPool.top);
    object.acceleration = GetAcceleration(objectPool.top);

    object.bounds = GetBounds(objectPool.top);
    objectPool.top++;

    object.bounds->width = width;
    object.bounds->height = height;

    return object;
}

void SetObjectPosition(Object *object, int x, int y) {
    object->position->x = x;
    object->position->y = y;
}
