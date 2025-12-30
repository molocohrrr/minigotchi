// Minigotchi - Evolution System (Header)
// Define os tipos e funções públicas responsáveis pela evolução
// do Minigotchi ao longo do tempo (formas/estágios).
// Não contém lógica de desenho nem código de persistência.

#pragma once

#include <stdint.h>
#include "mg_state.h"

void mg_evolution_init(MinigotchiState* state, uint32_t now_ts);
void mg_evolution_update(MinigotchiState* state, uint32_t now_ts);
