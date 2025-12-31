// Minigotchi - Game State
// Este arquivo define toda a estrutura de estado do jogo,
// incluindo enums de comida e a struct MinigotchiState.
// Nenhuma lógica de jogo acontece aqui; apenas dados.

#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    MinigotchiFoodNone = 0,
    MinigotchiFoodBurger,
    MinigotchiFoodSoda,
} MinigotchiFood;

typedef enum {
    MinigotchiFormStage0 = 0,
    MinigotchiFormStage1,
    MinigotchiFormStage2,
    MinigotchiFormStage3,
    MinigotchiFormStage4,
    MinigotchiFormStage5,
    MinigotchiFormStage6,
} MinigotchiForm;

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
    // Fome
    uint8_t  hunger_level;          
    uint32_t last_hunger_update_ts; 
    bool hungry;
    // Carência
    uint8_t affection_level;
    uint32_t last_affection_update_ts;
    bool lonely;
    // Sono
    bool sleeping;
    // Evolução
    MinigotchiForm form;
    uint32_t birth_timestamp;
    uint32_t last_feed_timestamp;
    uint32_t last_evolution_day;
} MinigotchiState;
