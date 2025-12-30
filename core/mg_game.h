// Minigotchi - Game Engine (Header)
// Define a interface do motor do jogo.
// Contém funções que modificam o estado (pet, feed, update, init),
// mas não desenha nada e não conhece UI.

#pragma once

#include "mg_state.h"

void mg_init(MinigotchiState* state);
void mg_update(MinigotchiState* state, uint32_t now);
void mg_pet(MinigotchiState* state, uint32_t now);
void mg_feed(MinigotchiState* state, uint32_t now, MinigotchiFood food);
