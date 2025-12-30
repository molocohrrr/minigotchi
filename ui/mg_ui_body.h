// Minigotchi - UI Body (Header)
// Declara o desenho do corpo do Minigotchi,
// incluindo olhos, boca e coração quando feliz.

#pragma once

#include <gui/gui.h>
#include <stdbool.h>

void mg_ui_body_draw(Canvas* canvas,
                     int x,
                     int y,
                     bool happy,
                     bool eating,
                     bool hungry);