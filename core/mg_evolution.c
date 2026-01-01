// Minigotchi - Evolution System (Implementation)
// Implementa as regras de evolução do Minigotchi com base no tempo:
// cálculo de idade, escolha da forma atual e transições de estágio.
// Não desenha nada e não conhece detalhes de UI.

#include "mg_evolution.h"
#include <furi_hal_rtc.h>

static uint32_t mg_day_index(const DateTime* dt) {
    return ((uint32_t)dt->year * 512u) +
           ((uint32_t)dt->month * 32u) +
           (uint32_t)dt->day;
}

void mg_evolution_init(MinigotchiState* state, uint32_t now_ts) {
    // Novo bichinho (primeira vez / sem save válido)
    state->birth_timestamp = now_ts;
    state->last_feed_timestamp = now_ts;
    state->form = MinigotchiFormStage0;
    state->hungry = false;

    // Inicializa o "dia de evolução" com o dia de hoje
    DateTime now;
    furi_hal_rtc_get_datetime(&now);
    state->last_evolution_day = mg_day_index(&now);
}

void mg_evolution_update(MinigotchiState* state, uint32_t now_ts) {
    (void)now_ts;

    if(state->form == MinigotchiFormStage7) {
        return;
    }

    DateTime now;
    furi_hal_rtc_get_datetime(&now);

    uint32_t today = mg_day_index(&now);

    // Evolução 9h
    if(now.hour < 9) {
        return;
    }

    if(state->last_evolution_day == 0) {
        state->last_evolution_day = today;
        return;
    }

     if(today <= state->last_evolution_day) {
        return;
    }

    uint32_t diff_days = today - state->last_evolution_day;

    uint32_t new_form = (uint32_t)state->form + diff_days;
    if(new_form > (uint32_t)MinigotchiFormStage7) {
        new_form = (uint32_t)MinigotchiFormStage7;
    }

    state->form = (MinigotchiForm)new_form;
    state->last_evolution_day = today;
}
