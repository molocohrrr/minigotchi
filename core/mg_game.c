// Minigotchi - Game Engine (Implementation)
// Implementa a lógica do jogo: movimentação, carinho,
// alimentação, timers e atualização do estado.
// Não contém código de desenho nem depende de UI.

#include <furi.h>
#include "mg_game.h"
#include "mg_evolution.h"
#include <furi_hal_rtc.h>

#define HUNGER_INTERVAL_SEC (60u * 60u) // 1h por barrinha
#define AFFECTION_INTERVAL_SEC (2u * 60u * 60u)  // 2h por barrinha

static uint32_t mg_now_epoch(void) {
    return furi_hal_rtc_get_timestamp();
}

static bool mg_is_sleep_time(void) {
    DateTime dt;
    furi_hal_rtc_get_datetime(&dt);
    uint8_t hour = dt.hour;
    return (hour >= 22 || hour < 9);
}

void mg_init(MinigotchiState* state) {
    uint32_t now_ts = mg_now_epoch();

    // movimento
    state->position_index   = 1;
    state->last_move_tick   = furi_get_tick();
    state->move_interval    = 800;
    state->petting          = false;
    state->pet_end_tick     = 0;
    state->running          = true;
    state->direction        = 1;

    // comida 
    state->eating       = false;
    state->eat_end_tick = 0;
    state->current_food = MinigotchiFoodNone;

    // fome em barrinhas (1–5)
    state->hunger_level          = 5;        
    state->last_hunger_update_ts = now_ts;   
    state->hungry                = false;

    // carência em barrinhas (1-5)
    state->affection_level = 5;
    state->last_affection_update_ts = now_ts;
    state->lonely = false;

    // sono
    state->sleeping = mg_is_sleep_time();

    // evolução
    mg_evolution_init(state, now_ts);
}

void mg_update(MinigotchiState* state, uint32_t now_tick) {
    uint32_t now_ts = mg_now_epoch();

    state->sleeping = mg_is_sleep_time();

    if(state->sleeping) {
        state->position_index = 1;            
        state->petting        = false;
        state->eating         = false;
        state->current_food   = MinigotchiFoodNone;
        state->hungry         = false;

        mg_evolution_update(state, now_ts);
        return;
    }

    // 1) movimento 
    if(now_tick - state->last_move_tick >= state->move_interval) {
        state->last_move_tick = now_tick;

        if(state->position_index <= 0) {
            state->direction = 1;
        } else if(state->position_index >= 2) {
            state->direction = -1;
        }

        state->position_index += state->direction;
        if(state->position_index < 0) state->position_index = 0;
        if(state->position_index > 2) state->position_index = 2;
    }

    // 2) timers de carinho
    if(state->petting && now_tick >= state->pet_end_tick) {
        state->petting = false;
    }

    // 3) timers de comida
    if(state->eating && now_tick >= state->eat_end_tick) {
        state->eating = false;
        state->current_food = MinigotchiFoodNone;
    }

    // 4) fominha
    if(now_ts > state->last_hunger_update_ts) {
        uint32_t delta_sec = now_ts - state->last_hunger_update_ts;
        uint32_t steps = delta_sec / HUNGER_INTERVAL_SEC;

        if(steps > 0 && state->hunger_level > 0) {
            uint32_t loss = steps;
            uint32_t max_loss = (uint32_t)state->hunger_level;

            if(loss > max_loss) {
                loss = max_loss;
            }

            state->hunger_level -= loss;
            state->last_hunger_update_ts += loss * HUNGER_INTERVAL_SEC;
        }
    }

    // bool hungry: só é verdade quando está na última barrinha
    state->hungry = (state->hunger_level == 0);

    // 5) carência
    if(!state->sleeping && now_ts > state->last_affection_update_ts) {
        uint32_t delta_sec = now_ts - state->last_affection_update_ts;
        uint32_t steps = delta_sec / AFFECTION_INTERVAL_SEC;

        if(steps > 0 && state->affection_level > 0) {
            uint32_t loss = steps;
            uint32_t max_loss = (uint32_t)state->affection_level;

            if(loss > max_loss) {
                loss = max_loss;
            }

            state->affection_level -= loss;
            state->last_affection_update_ts += loss * AFFECTION_INTERVAL_SEC;
        }
    }

    // bool lonely: só é verdade quando está na última barrinha
    state->lonely = (state->affection_level == 0);

    // 6) evolução (mudança de forma às 9h)
    mg_evolution_update(state, now_ts);
}

void mg_pet(MinigotchiState* state, uint32_t now_tick) {
    if(state->sleeping) return;
    if(state->form >= MinigotchiFormStage7) return;

    state->petting = true;
    state->pet_end_tick = now_tick + 1500;

    uint32_t now_ts = mg_now_epoch();

    if(state->affection_level < 5) {
        state->affection_level++;
    }

    state->last_affection_update_ts = now_ts;
    state->lonely = (state->affection_level == 0);
}

void mg_feed(MinigotchiState* state, uint32_t now_tick, MinigotchiFood food) {
    if(food == MinigotchiFoodNone) return;
    if(state->sleeping) return;
    if(state->form >= MinigotchiFormStage7) return;

    state->eating       = true;
    state->current_food = food;
    state->eat_end_tick = now_tick + 1500;

    uint32_t now_ts = mg_now_epoch();

    if(state->hunger_level < 5) {
        state->hunger_level++;
    }
    state->last_hunger_update_ts = now_ts;
    state->hungry = (state->hunger_level == 0);
}
