// Minigotchi - Evolution System (Implementation)
// Implementa as regras de evolução do Minigotchi com base no tempo:
// cálculo de idade, escolha da forma atual e transições de estágio.
// Não desenha nada e não conhece detalhes de UI.

#include "mg_evolution.h"

void mg_evolution_init(MinigotchiState* state, uint32_t now_ts) {
    state->birth_timestamp = now_ts;
    state->last_feed_timestamp = now_ts;
    state->form = MinigotchiFormStage0;
    state->hungry = false;
}

void mg_evolution_update(MinigotchiState* state, uint32_t now_ts) {
    if(now_ts < state->birth_timestamp) {
        state->birth_timestamp = now_ts;
        state->form = MinigotchiFormStage0;
        return;
    }

    uint32_t elapsed = now_ts - state->birth_timestamp;
    uint32_t days = elapsed / (24u * 60u * 60u); // segundos -> dias
    //uint32_t days = elapsed / (60 * 60); // segundos -> dias

    if(days > 6u) days = 6u;

    state->form = (MinigotchiForm)days;
}
