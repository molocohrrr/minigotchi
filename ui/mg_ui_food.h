// Minigotchi - UI Food (Header)
// Declara as rotinas de desenho de comida,
// como hambúrguer e refrigerante.

#pragma once

#include <gui/gui.h>
#include "../core/mg_state.h"

// Desenha hambúrguer ou refrigerante
void mg_ui_food_draw(Canvas* canvas, int x, int y, MinigotchiFood food);
void mg_ui_food_draw_soda(Canvas* canvas, int x, int y);
