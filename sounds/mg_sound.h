// Minigotchi - Sounds (Implementation)
// Expõe rotinas de som

#pragma once
#include <stdint.h>
#include <input/input.h>

void mg_play_tone(uint32_t freq, uint32_t duration_ms);
void mg_mario_push(InputKey key);
