#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include "raylib.h"

typedef struct {
    Vector2 *positions;
    Vector2 *velocities;
    Vector2 *accelerations;
    Rectangle *bounds;
    int count;
    int top;
} ObjectPool;

void InitObjectPool(int count);
void DestroyObjectPool(void); 
void TickObjectPool(float deltaTime);

typedef struct {
    int id;
    Vector2 *position;
    Vector2 *velocity;
    Vector2 *acceleration;
    Rectangle *bounds;
} Object;

Object CreateObject(int width, int height);
void SetObjectPosition(Object *const object, int x, int y);

#endif // !OBJECTPOOL_H

