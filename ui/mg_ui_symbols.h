// Minigotchi - UI Symbols (Header)
// Declara o símbolos que acompanham o Minigotchi,
// incluindo coração quando feliz, cruz se morto, etc.

#pragma once

#include <gui/gui.h>
#include <stdbool.h>
#include "../core/mg_state.h"

void mg_ui_draw_small_heart(Canvas* canvas, int x, int y);

void mg_ui_draw_big_heart(Canvas* canvas, int x, int y);

void mg_ui_draw_crux(Canvas* canvas, int x, int y);

void mg_ui_draw_syringe(Canvas* canvas, int x, int y);