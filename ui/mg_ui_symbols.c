// Minigotchi - UI Symbols (Implementation)
// Desenha símbolos representativos.
// Apenas renderiza baseado no estado recebido.

#include <furi.h>
#include "mg_ui_body.h"
#include "../images/syringe_20x20.xbm"

#define HEART_W 13
#define HEART_H 9

// Coração meior
// '.' = vazio, '#' = pixel aceso
static const char* heart_sprite[HEART_H] = {
    "..###...###..",
    ".#####.#####.",
    "#############",
    "#############",
    ".###########.",
    "..#########..",
    "...#######...",
    "....#####....",
    ".....###.....",
};

static void draw_big_heart(Canvas* canvas, int origin_x, int origin_y) {
    for(int y = 0; y < HEART_H; y++) {
        const char* row = heart_sprite[y];
        for(int x = 0; x < HEART_W; x++) {
            if(row[x] == '#') {
                canvas_draw_dot(canvas, origin_x + x, origin_y + y);
            }
        }
    }
}

void mg_ui_draw_big_heart(Canvas* canvas, int x, int y) {
    draw_big_heart(canvas, x, y);
}

// Coração menor
// '.' = vazio, '#' = pixel aceso

#define SMALL_HEART_W 9
#define SMALL_HEART_H 7

static const char* small_heart_sprite[SMALL_HEART_H] = {
    ".###.###.",
    "#########",
    "#########",
    "#########",
    ".#######.",
    "..#####..",
    "...###...",
};

static void draw_small_heart(Canvas* canvas, int origin_x, int origin_y) {
    for(int y = 0; y < SMALL_HEART_H; y++) {
        const char* row = small_heart_sprite[y];
        for(int x = 0; x < SMALL_HEART_W; x++) {
            if(row[x] == '#') {
                canvas_draw_dot(canvas, origin_x + x, origin_y + y);
            }
        }
    }
}

void mg_ui_draw_small_heart(Canvas* canvas, int x, int y) {
    draw_small_heart(canvas, x, y);
}

// Cruz quando morto
// '.' = vazio, '#' = pixel aceso

#define CRUX_W 11
#define CRUX_H 11

static const char* crux_sprite[CRUX_H] = {
    "....###....",
    "....###....",
    "....###....",
    "###########",
    "###########",
    "....###....",
    "....###....",
    "....###....",
    "....###....",
    "....###....",
    "....###....",
};

static void draw_crux(Canvas* canvas, int origin_x, int origin_y) {
    for(int y = 0; y < CRUX_H; y++) {
        const char* row = crux_sprite[y];
        for(int x = 0; x < CRUX_W; x++) {
            if(row[x] == '#') {
                canvas_draw_dot(canvas, origin_x + x, origin_y + y);
            }
        }
    }
}

void mg_ui_draw_crux(Canvas* canvas, int x, int y) {
    draw_crux(canvas, x, y);
}

void mg_ui_draw_syringe(Canvas* canvas, int x, int y) {
    // animação pra cima/baixo
    uint32_t t = furi_get_tick();
    int bounce = ((t / 500) % 2) ? -2 : 2; // velocidade da animação
    int hw = syringe_20x20_width;
    int hh = syringe_20x20_height;
    int radius = 10;

    // desenha a seringa ao lado
    int syringe_x = x + radius + 9;
    int syringe_y = y - (hh / 2) + bounce;

    canvas_draw_xbm(
        canvas,
        syringe_x,
        syringe_y,
        hw,
        hh,
        syringe_20x20_bits
    );
}
