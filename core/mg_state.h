// Minigotchi - Game State
// Este arquivo define toda a estrutura de estado do jogo,
// incluindo enums de comida e a struct MinigotchiState.
// Nenhuma lógica de jogo acontece aqui; apenas dados.

#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <stdbool.h>

typedef enum {
    MinigotchiFoodNone = 0,
    MinigotchiFoodBurger,
    MinigotchiFoodSoda,
} MinigotchiFood;

typedef struct {
    int position_index;
    uint32_t last_move_tick;
    uint32_t move_interval;
    bool petting;
    uint32_t pet_end_tick;
    bool running;
    int direction;
    bool eating;
    uint32_t eat_end_tick;
    MinigotchiFood current_food;
    bool hungry;
    uint32_t last_feed_timestamp;  
} MinigotchiState;
