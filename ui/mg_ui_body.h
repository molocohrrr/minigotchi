// Minigotchi - UI Body (Header)
// Declara o desenho do corpo do Minigotchi,
// incluindo olhos, boca, etc.

#pragma once

#include <gui/gui.h>
#include <stdbool.h>
#include "../core/mg_state.h"

void mg_ui_body_draw(Canvas* canvas,
                     int x,
                     int y,
                     MinigotchiForm form,
                     bool happy,
                     bool eating,
                     bool hungry,
                     bool sleeping,
                     bool lonely);
