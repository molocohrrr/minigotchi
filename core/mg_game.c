// Minigotchi - Game Engine (Implementation)
// Implementa a lógica do jogo: movimentação, carinho,
// alimentação, timers e atualização do estado.
// Não contém código de desenho nem depende de UI.

#include <furi.h>
#include "mg_game.h"
#include "mg_evolution.h"
#include <furi_hal_rtc.h>

#define HUNGER_INTERVAL_SEC (60u * 60u) // 1h pra sentir fome

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
    // movimento
    state->position_index = 1;
    state->last_move_tick = furi_get_tick();
    state->move_interval = 800;
    state->petting = false;
    state->pet_end_tick = 0;
    state->running = true;
    state->direction = 1;

    // carinho
    state->petting = false;
    state->pet_end_tick = 0;

    // alimentação
    state->eating = false;
    state->eat_end_tick = 0;
    state->current_food = MinigotchiFoodNone;
    state->hungry = false;

    // sono
    state->sleeping = mg_is_sleep_time();

    // evolução + timestamps (nascimento, last_feed)
    uint32_t now_ts = mg_now_epoch();
    mg_evolution_init(state, now_ts);
}

void mg_update(MinigotchiState* state, uint32_t now_tick) {
    uint32_t now_ts = mg_now_epoch();

    // 1) atualiza flag de sono com base no RTC
    state->sleeping = mg_is_sleep_time();

    // 2) se estiver DORMINDO:
    if(state->sleeping) {

        state->position_index = 1;
        state->petting = false;
        state->eating = false;
        state->current_food = MinigotchiFoodNone;
        state->hungry = false;
        mg_evolution_update(state, now_ts);
        return; // nada de movimento / timers acordado
    }

    // 3) movimento (só ACORDADO)
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

    // 4) timers de carinho
    if(state->petting && now_tick >= state->pet_end_tick) {
        state->petting = false;
    }

    // 5) timers de comida 
    if(state->eating && now_tick >= state->eat_end_tick) {
        state->eating = false;
        state->current_food = MinigotchiFoodNone;

        state->last_feed_timestamp = now_ts;
        state->hungry = false;
    }

    // 6) fome (só progride acordado)
    if(now_ts >= state->last_feed_timestamp + HUNGER_INTERVAL_SEC) {
        state->hungry = true;
    } else {
        state->hungry = false;
    }

    // 7) evolução
    mg_evolution_update(state, now_ts);
}

void mg_pet(MinigotchiState* state, uint32_t now_tick) {
    if(state->sleeping) return;

    state->petting = true;
    state->pet_end_tick = now_tick + 1500;
}

void mg_feed(MinigotchiState* state, uint32_t now_tick, MinigotchiFood food) {
    if(food == MinigotchiFoodNone) return;

    if(state->sleeping) return;

    state->eating = true;
    state->current_food = food;
    state->eat_end_tick = now_tick + 1500;
}
