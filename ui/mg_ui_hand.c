// Minigotchi - UI Hand (Implementation)
// Renderiza a mão de carinho usando um sprite XBM,
// incluindo a pequena animação de movimento.

#include <furi.h>
#include <gui/gui.h>
#include "mg_ui_hand.h"
#include "../images/hand_32x32.xbm"

// desenha a mão 
void mg_ui_hand_draw(Canvas* canvas, int gotchi_x, int gotchi_y) {
    // animação pra cima/baixo
    uint32_t t = furi_get_tick();
    int bounce = ((t / 500) % 2) ? -2 : 2; // velocidade da animação
    int hw = hand_32x32_width;
    int hh = hand_32x32_height;
    int radius = 10;

    // desenha a mão ao lado
    int hand_x = gotchi_x + radius + 5;
    int hand_y = gotchi_y - (hh / 2) + bounce;

    canvas_draw_xbm(
        canvas,
        hand_x,
        hand_y,
        hw,
        hh,
        hand_32x32_bits
    );
}
