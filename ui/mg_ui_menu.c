// Minigotchi - UI Menu (Implementation)
// Desenha os símbolos do menu (HUD).
// Apenas renderiza baseado no estado recebido.

#include <furi.h>
#include "mg_ui_menu.h"

static void mg_ui_draw_sprite(
    Canvas* canvas,
    const char* const* sprite,
    int w,
    int h,
    int center_x,
    int top_y
) {
    int origin_x = center_x - (w / 2);

    for(int yy = 0; yy < h; yy++) {
        const char* row = sprite[yy];
        for(int xx = 0; xx < w; xx++) {
            if(row[xx] == '#') {
                canvas_draw_dot(canvas, origin_x + xx, top_y + yy);
            }
        }
    }
}

#define ARROW_W 7
#define UD_H 6
static const char* up_sprite[UD_H] = {
    "...#...",
    "..###..",
    "..###..",
    ".#####.",
    ".#####.",    
    "#######",
};

static const char* down_sprite[UD_H] = {
    "#######",
    ".#####.",
    ".#####.",
    "..###..",
    "..###..",
    "...#...",
};

#define LR_H 7
static const char* left_sprite[LR_H] = {
    "......#",
    "....###",
    "..#####",
    "#######",
    "..#####",
    "....###",
    "......#",
};

static const char* right_sprite[LR_H] = {
    "#......",
    "###....",
    "#####..",
    "#######",
    "#####..",
    "###....",
    "#......",
};

#define BALL_W 7
#define BALL_H 7
static const char* ball_sprite[BALL_H] = {
    "..###..",
    ".#####.",
    "#######",
    "#######",
    "#######",
    ".#####.",
    "..###..",
};

// Seta cima
void mg_ui_menu_draw_up(Canvas* canvas, int x, int y) {
    mg_ui_draw_sprite(canvas, up_sprite, ARROW_W, UD_H, x, y);
}

// Seta baixo
void mg_ui_menu_draw_down(Canvas* canvas, int x, int y) {
    mg_ui_draw_sprite(canvas, down_sprite, ARROW_W, UD_H, x, y - 6);
    canvas_draw_str(canvas, x + 6, y, ": Cure");
}

// Seta esquerda
void mg_ui_menu_draw_left(Canvas* canvas, int x, int y) {
    mg_ui_draw_sprite(canvas, left_sprite, ARROW_W, LR_H, x, y - 7);
    canvas_draw_str(canvas, x + 6, y, "&");
}

// Seta direita
void mg_ui_menu_draw_right(Canvas* canvas, int x, int y) {
    mg_ui_draw_sprite(canvas, right_sprite, ARROW_W, LR_H, x, y - 7);
    canvas_draw_str(canvas, x + 6, y, ": Food");
}

// Círculo Ok
void mg_ui_menu_draw_ball(Canvas* canvas, int x, int y) {
    mg_ui_draw_sprite(canvas, ball_sprite, BALL_W, BALL_H, x + 2, y - 7);
    canvas_draw_str(canvas, x + 8, y, ": Love");
}
