// Minigotchi - UI Screen (Implementation)
// Responsável por desenhar a tela completa do jogo.
// Coordena o corpo, a mão de carinho, a comida e o texto da interface.
// Não altera estado do jogo; apenas lê e desenha.

#include <furi.h>
#include "mg_ui_screen.h"
#include "mg_ui_body.h"
#include "mg_ui_hand.h"
#include "mg_ui_food.h"
#include "mg_ui_symbols.h"

// Barra fominha
static void mg_ui_draw_hunger(Canvas* canvas, const MinigotchiState* state) {
    const int max_bars   = 5;
    const int bar_w      = 5;
    const int bar_h      = 5;
    const int start_x    = 2;   // lado esquerdo
    const int start_y    = 14;  // começando abaixo do título
    const int spacing    = 2;

    // soda pequena acima das barrinhas
    int soda_x = start_x - 2;
    int soda_y = start_y - 11;
    mg_ui_food_draw_soda(canvas, soda_x, soda_y);

    // desenha barrinhas
    for(int i = 0; i < max_bars; i++) {
        int y = start_y + (max_bars - 1 - i) * (bar_h + spacing);
        if(i < state->hunger_level) {
            canvas_draw_box(canvas, start_x, y, bar_w, bar_h);
        } else {
            canvas_draw_frame(canvas, start_x, y, bar_w, bar_h);
        }
    }
}

// Barra carência
static void mg_ui_draw_affection(Canvas* canvas, const MinigotchiState* state) {
    const int max_bars   = 5;
    const int bar_w      = 5;
    const int bar_h      = 5;
    const int start_x    = 120; // lado direito
    const int start_y    = 14;
    const int spacing    = 2;

    // coração pequeno acima das barrinhas
    int heart_x = start_x - 2;
    int heart_y = start_y - 12;
    mg_ui_draw_small_heart(canvas, heart_x, heart_y);

    // desenha barrinhas
    for(int i = 0; i < max_bars; i++) {
        int y = start_y + (max_bars - 1 - i) * (bar_h + spacing);
        if(i < state->affection_level) {
            canvas_draw_box(canvas, start_x, y, bar_w, bar_h);
        } else {
            canvas_draw_frame(canvas, start_x, y, bar_w, bar_h);
        }
    }
}

// Título do jogo
void mg_ui_screen_draw(Canvas* canvas, const MinigotchiState* state) {
    canvas_clear(canvas);

    const char* title = "M i n i G o t c h i";

    canvas_draw_str_aligned(
        canvas,
        64,
        0,
        AlignCenter,
        AlignTop,
        title
    );

    canvas_draw_str_aligned(
        canvas,
        65,
        0,
        AlignCenter,
        AlignTop,
        title
    );

    // barra de fome à esquerda
    if(state->form != MinigotchiFormStage7) {
       mg_ui_draw_hunger(canvas, state);
    }
    
    // barra de carinho à direita
    if(state->form != MinigotchiFormStage7) {
        mg_ui_draw_affection(canvas, state);    
    }
    
    // Posições de movimento do bichinho
    int x_positions[3] = {30, 60, 95};

    // flags derivadas do estado
    bool happy    = state->petting;    // carinho
    bool eating   = state->eating;     // comendo
    bool hungry   = state->hungry;     // fominha!
    bool sleeping = state->sleeping;   // mimindo!
    bool lonely = state->lonely;   // carente

    // posição parada
    int index;
    if(state->form == MinigotchiFormStage7 || sleeping) {
        index = 1;
    } else if(happy || eating || hungry || lonely) {
        index = 1;
    } else {
        index = state->position_index;
    }

    if(index < 0) index = 0;
    if(index > 2) index = 2;

    int x = x_positions[index]; // altura central, aumenta pra direita
    int y = 32; // largura central, aumenta pra baixo 

    // corpo / boca / coração / boca mastigando / cara triste
    mg_ui_body_draw(canvas, x, y, state->form, happy, eating, hungry, lonely, sleeping);

    // mão de carinho (só acordado)
    if(state->petting && !sleeping) {
        mg_ui_hand_draw(canvas, x, y);
    }

    // comida (hambúrguer ou refrigerante) – só acordado
    if(eating && !sleeping && state->current_food != MinigotchiFoodNone) {
        int food_x = x - 8;   // posição da comida
        int food_y = y + 10;
        mg_ui_food_draw(canvas, food_x, food_y, state->current_food);
    }

    // ZZZ de sono 
    if(sleeping) {
        int base_x = x + 10;
        int base_y = y - 6;

        uint32_t t = furi_get_tick();
        int phase = (t / 500) % 2; // velocidade da animação

        int offset_x = 0;
        int offset_y = 0;

        if(phase == 1) {
            offset_x = 2;
            offset_y = -2;
        }

        int z_x = base_x + offset_x;
        int z_y = base_y + offset_y;

        canvas_draw_str(canvas, z_x,         z_y,         "Z");
        canvas_draw_str(canvas, z_x + 6,     z_y - 4,     "Z");
        canvas_draw_str(canvas, z_x + 12,    z_y - 8,     "Z");
    }

    // Cruz de morto
    if(state->form == MinigotchiFormStage7) {
        int crux_x = x + 20;
        int crux_y = y - 15;
        mg_ui_draw_crux(canvas, crux_x, crux_y);
    }

    // menu inferior
    if(state->form == MinigotchiFormStage7) {
        canvas_draw_str(canvas, 35, 62, "OK: New game");
    } else {
        canvas_draw_str(canvas, 2, 62, "OK: Love      L: Food      R: Drink");
    }
}
