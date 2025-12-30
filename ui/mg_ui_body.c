// Minigotchi - UI Body (Implementation)
// Desenha o personagem na tela, incluindo corpo,
// olhos, expressões faciais e coração quando feliz.
// Apenas renderiza baseado no estado recebido.

#include <furi.h>
#include "mg_ui_body.h"
#define HEART_W 13
#define HEART_H 9

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

static void draw_heart(Canvas* canvas, int origin_x, int origin_y) {
    for(int y = 0; y < HEART_H; y++) {
        const char* row = heart_sprite[y];
        for(int x = 0; x < HEART_W; x++) {
            if(row[x] == '#') {
                canvas_draw_dot(canvas, origin_x + x, origin_y + y);
            }
        }
    }
};

void mg_ui_body_draw(Canvas* canvas,
                     int x,
                     int y,
                     bool happy,
                     bool eating,
                     bool hungry) {
    int body_w = 30;
    int body_h = 18;
    int body_x = x - (body_w / 2);
    int body_y = y - (body_h / 2);

    // corpo 1
    canvas_draw_rframe(canvas, body_x, body_y, body_w, body_h, 8);

    // olhos
    int eye_y = y - 3;
    canvas_draw_box(canvas, x - 8, eye_y, 3, 4); // esquerdo
    canvas_draw_box(canvas, x + 5, eye_y, 3, 4); // direito

    // boca/expressão
    if(eating) {
        // boca mastigando
        uint32_t t = furi_get_tick();
        bool open = ((t / 500) % 2) == 0; // velocidade da animação
        int mouth_y = y + 3;
        if(open) {
            canvas_draw_box(canvas, x - 3, mouth_y, 7, 2);
            canvas_draw_line(canvas, x - 3, mouth_y + 2, x + 3, mouth_y + 2);
        } else {
            canvas_draw_line(canvas, x, mouth_y - 1, x, mouth_y + 3);
        }
        return;
    }

    if(hungry) {
        // fominha fica :-(
        int mouth_y = y + 4;
        canvas_draw_line(canvas, x - 6, mouth_y + 2, x - 3, mouth_y);
        canvas_draw_line(canvas, x - 3, mouth_y,     x + 3, mouth_y);
        canvas_draw_line(canvas, x + 3, mouth_y,     x + 6, mouth_y + 2);
        return;
    }

    if(happy) {
        // acarinhado fica =D
        int mx = x;
        int top = y + 2;
        canvas_draw_box(canvas, mx - 5, top, 11, 2);
        canvas_draw_line(canvas, mx - 5, top + 2, mx + 5, top + 2);
        canvas_draw_line(canvas, mx - 4, top + 3, mx + 4, top + 3);
        canvas_draw_line(canvas, mx - 3, top + 4, mx + 3, top + 4);

        // coração no canto superior esquerdo
        int heart_x = x - 25;
        int heart_y = y - 18;
        draw_heart(canvas, heart_x, heart_y);
    } else {
        // sorriso normal :-)
        int smile_y = y + 4;
        canvas_draw_line(canvas, x - 6, smile_y,     x - 3, smile_y + 2);
        canvas_draw_line(canvas, x - 3, smile_y + 2, x + 3, smile_y + 2);
        canvas_draw_line(canvas, x + 3, smile_y + 2, x + 6, smile_y);
    }
}
