// Minigotchi - UI Screen (Header)
// Declara o desenho da tela completa do Minigotchi.
// A UI lê o estado e renderiza, sem alterar lógica do jogo.

#pragma once

#include <gui/gui.h>
#include <stdbool.h>
#include "../core/mg_state.h"

// Desenha a tela inteira a partir do estado atual
void mg_ui_screen_draw(Canvas* canvas, const MinigotchiState* state);
