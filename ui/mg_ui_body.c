// Minigotchi - UI Body (Implementation)
// Desenha o personagem na tela, incluindo corpo,
// olhos, expressões faciais, coração, bigode etc.
// Apenas renderiza baseado no estado recebido.

#include <furi.h>
#include "mg_ui_body.h"

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

static void draw_heart(Canvas* canvas, int origin_x, int origin_y) {
    for(int y = 0; y < HEART_H; y++) {
        const char* row = heart_sprite[y];
        for(int x = 0; x < HEART_W; x++) {
            if(row[x] == '#') {
                canvas_draw_dot(canvas, origin_x + x, origin_y + y);
            }
        }
    }
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

void mg_ui_draw_heart(Canvas* canvas, int x, int y) {
    draw_small_heart(canvas, x, y);
}

#define MOUTH_W 9
#define MOUTH_H 4

// Boquinha :-3 / "uu"
// '.' = vazio, '#' = pixel aceso
static const char* mouth_sprite[MOUTH_H] = {
    ".........", 
    "#.......#", 
    "#..##...#", 
    ".##...##.", 
};

static void draw_mouth_uwu(Canvas* canvas, int center_x, int top_y) {
    int origin_x = center_x - (MOUTH_W / 2);

    for(int y = 0; y < MOUTH_H; y++) {
        const char* row = mouth_sprite[y];
        for(int x = 0; x < MOUTH_W; x++) {
            if(row[x] == '#') {
                canvas_draw_dot(canvas, origin_x + x, top_y + y);
            }
        }
    }
}

void mg_ui_body_draw(Canvas* canvas,
                     int x,
                     int y,
                     MinigotchiForm form,
                     bool happy,
                     bool eating,
                     bool hungry,
                     bool lonely,
                     bool sleeping) {
   
    int draw_y = y;

    // formas 5 e 6 um pouco mais embaixo (coelho maior)
    if(form >= MinigotchiFormStage5) {
        draw_y += 4;
    }

    // se estiver dormindo, rebaixa o corpo
    if(sleeping) {
        draw_y += 2;
    }
    
    // Corpo
    int body_w;
    int body_h;
    int radius;

    if(form == MinigotchiFormStage0) {
        body_w = 24;
        body_h = 15;
        radius = 7;
    } else if(form == MinigotchiFormStage1) {
        body_w = 28;
        body_h = 16;
        radius = 8;
    } else {
        // formas 2–6
        body_w = 32;
        body_h = 20;
        radius = 8;
    }

    int body_x = x - (body_w / 2);
    int body_y = draw_y - (body_h / 2);

    canvas_draw_rframe(canvas, body_x, body_y, body_w, body_h, radius);

    // Rabinho
    if(form >= MinigotchiFormStage2) {
        int tail_x = body_x + body_w + 1;
        int tail_y = draw_y + 1;
        canvas_draw_circle(canvas, tail_x, tail_y, 2);
    }

    // Orelhas 
    if(form >= MinigotchiFormStage3) {
        int ear_bottom_y = body_y;
        int ear_top_y;

        if(form == MinigotchiFormStage3) {
            ear_top_y = body_y - 4;   // curta
        } else if(form == MinigotchiFormStage4) {
            ear_top_y = body_y - 7;   // média
        } else {
            ear_top_y = body_y - 11;  // longa (5 e 6)
        }

        // orelha esquerda
        int lx1 = x - 6;
        int lx2 = x - 2;

        canvas_draw_line(canvas, lx1, ear_top_y, lx1, ear_bottom_y);
        canvas_draw_line(canvas, lx2, ear_top_y, lx2, ear_bottom_y);

        int top_y1 = ear_top_y - 1;
        int top_y2 = ear_top_y;
        canvas_draw_line(canvas, lx1 + 1, top_y1, lx2 - 1, top_y1);
        canvas_draw_line(canvas, lx1,     top_y2, lx2,     top_y2);

        // orelha direita
        int rx1 = x + 2;
        int rx2 = x + 6;

        canvas_draw_line(canvas, rx1, ear_top_y, rx1, ear_bottom_y);
        canvas_draw_line(canvas, rx2, ear_top_y, rx2, ear_bottom_y);
        canvas_draw_line(canvas, rx1 + 1, top_y1, rx2 - 1, top_y1);
        canvas_draw_line(canvas, rx1,     top_y2, rx2,     top_y2);
    }

    // Mimindo
    if(sleeping) {
        int eye_y = draw_y - 1;

        // olho esquerdo
        canvas_draw_box(canvas, x - 6, eye_y, 4, 1);
        // olho direito
        canvas_draw_box(canvas, x + 3, eye_y, 4, 1);
        // boca 
        canvas_draw_box(canvas, x + 0, 40, 2, 2);
        return;
    }

    // Olhos (acordado)
    int eye_y = draw_y - 3;

    if(form <= MinigotchiFormStage1) {
        // Forma 0-1: olho menor
        canvas_draw_box(canvas, x - 6, eye_y, 2, 2);
        canvas_draw_box(canvas, x + 4, eye_y, 2, 2);
    } else {
        // Formas 2–6: olhos gordinhos
        canvas_draw_box(canvas, x - 8, eye_y, 3, 4); // esquerdo
        canvas_draw_box(canvas, x + 5, eye_y, 3, 4); // direito
    }

    // Boca

    // Comer
    if(eating) {
        uint32_t t = furi_get_tick();
        bool open = ((t / 500) % 2) == 0;
        int mouth_y = draw_y + 3;

        if(open) {
            canvas_draw_box(canvas, x - 3, mouth_y, 7, 2);
            canvas_draw_line(canvas, x - 3, mouth_y + 2, x + 3, mouth_y + 2);
        } else {
            canvas_draw_line(canvas, x, mouth_y - 1, x, mouth_y + 3);
        }
        return;
    }

    // Carinha fomes/carente :-(
    if(hungry || lonely) {
        int mouth_y = draw_y + 4;
        canvas_draw_line(canvas, x - 6, mouth_y + 2, x - 3, mouth_y);
        canvas_draw_line(canvas, x - 3, mouth_y,     x + 3, mouth_y);
        canvas_draw_line(canvas, x + 3, mouth_y,     x + 6, mouth_y + 2);
        return;
    }

    // Carinha feliz (carinho): =D + coração
    if(happy) {
        if(form != MinigotchiFormStage0) {
            int mx = x;
            int top = draw_y + 2;
            canvas_draw_box(canvas, mx - 5, top, 11, 2);
            canvas_draw_line(canvas, mx - 5, top + 2, mx + 5, top + 2);
            canvas_draw_line(canvas, mx - 4, top + 3, mx + 4, top + 3);
            canvas_draw_line(canvas, mx - 3, top + 4, mx + 3, top + 4);
        }

        int heart_x = x - 25;
        int heart_y = draw_y - 18;
        draw_heart(canvas, heart_x, heart_y);

        return;
    }

    // Forma 0
    if(form == MinigotchiFormStage0) {
        return;
    }

    // Forma 1
    if(form == MinigotchiFormStage1) {
        int smile_y = draw_y + 3;
        canvas_draw_line(canvas, x - 6, smile_y,     x - 3, smile_y + 2);
        canvas_draw_line(canvas, x - 3, smile_y + 2, x + 3, smile_y + 2);
        canvas_draw_line(canvas, x + 3, smile_y + 2, x + 6, smile_y);
        return;
    }

    // Formas 2–6
    int mouth_top = draw_y + 2;
    draw_mouth_uwu(canvas, x, mouth_top);

    // Bigode (só na forma final)
    if(form == MinigotchiFormStage6) {
        int wy = draw_y + 4; // altura central da boca

        // lado esquerdo
        canvas_draw_line(canvas, x - 12, wy - 1, x - 6, wy);
        canvas_draw_line(canvas, x - 12, wy + 1, x - 6, wy);

        // lado direito
        canvas_draw_line(canvas, x + 6,  wy, x + 12, wy - 1);
        canvas_draw_line(canvas, x + 6,  wy, x + 12, wy + 1);
    }
}
