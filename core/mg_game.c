// Minigotchi - Game Engine (Implementation)
// Implementa a lógica do jogo: movimentação, carinho,
// alimentação, timers e atualização do estado.
// Não contém código de desenho nem depende de UI.

#include <furi_hal_rtc.h>
#include "mg_state.h"
#include "mg_game.h"

void mg_init(MinigotchiState* state) {
    state->position_index = 1;
    state->last_move_tick = furi_get_tick();
    state->move_interval = 800;
    state->petting = false;
    state->pet_end_tick = 0;
    state->running = true;
    state->direction = 1;
    state->eating = false;
    state->eat_end_tick = 0;
    state->current_food = MinigotchiFoodNone;
    state->hungry = false;
    state->last_feed_timestamp = furi_hal_rtc_get_timestamp();
}

// movimento baseado em tick
void mg_update(MinigotchiState* state, uint32_t now_ms) {
    if(now_ms - state->last_move_tick >= state->move_interval) {
        state->last_move_tick = now_ms;
        if(state->position_index == 0) {
            state->direction = 1;
        } else if(state->position_index == 2) {
            state->direction = -1;
        }
        state->position_index += state->direction;
        if(state->position_index > 2) state->position_index = 2;
    }

    if(state->petting && now_ms >= state->pet_end_tick) {
        state->petting = false;
    }

    if(state->eating && now_ms >= state->eat_end_tick) {
        state->eating = false;
        state->current_food = MinigotchiFoodNone;
    }

    // fome baseada em RTC, intervalo de alimentação
    const uint32_t HUNGER_INTERVAL_SEC = 60 * 60;
    
    uint32_t now_ts = furi_hal_rtc_get_timestamp();
    if(!state->hungry) {
        if(now_ts - state->last_feed_timestamp >= HUNGER_INTERVAL_SEC) {
            state->hungry = true;
        }
    }
}

void mg_pet(MinigotchiState* state, uint32_t now) {
    state->petting = true;
    state->pet_end_tick = now + 1200;
}

void mg_feed(MinigotchiState* state, uint32_t now_ms, MinigotchiFood food) {
    if(food == MinigotchiFoodNone) return;

    state->eating = true;
    state->current_food = food;
    state->eat_end_tick = now_ms + 1200;

    uint32_t now_ts = furi_hal_rtc_get_timestamp();
    state->hungry = false;
    state->last_feed_timestamp = now_ts;
}
