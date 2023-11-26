#ifndef PLAYER_H
#define PLAYER_H

#include "objectPool.h"
#include "raylib.h"

void PlayerInput(Object *player);
void PlayerUpdate(Object *player, float deltaTime);

#endif // PLAYER_H
