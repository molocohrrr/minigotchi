// Minigotchi - UI Screen (Implementation)
// Responsável por desenhar a tela completa do jogo.
// Coordena o corpo, a mão de carinho, a comida e o texto da interface.
// Não altera estado do jogo; apenas lê e desenha.

#include "mg_ui_screen.h"
#include "mg_ui_body.h"
#include "mg_ui_hand.h"
#include "mg_ui_food.h"

void mg_ui_screen_draw(Canvas* canvas, const MinigotchiState* state) {
    canvas_clear(canvas);

    // título em negrito
    const char* title = "M i n i G o t c h i";

    canvas_draw_str_aligned(
        canvas,
        64,
        0,
        AlignCenter,
        AlignTop,
        title
    );

    // desloca 1px pra “engrossar”
    canvas_draw_str_aligned(
        canvas,
        65,
        0,
        AlignCenter,
        AlignTop,
        title
    );

    int x_positions[3] = {20, 60, 100};

    // flags derivadas do estado
    bool happy  = state->petting;      // carinho
    bool eating = state->eating;       // comendo
    bool hungry = state->hungry;       // fominha!
    bool sleeping = state->sleeping;   // mimindo!

    int index = (happy || eating || hungry) ? 1 : state->position_index;

    int x = x_positions[index];
    int y = 32;

    // corpo / boca / coração / boca mastigando / cara triste    
    mg_ui_body_draw(canvas, x, y, state->form, happy, eating, hungry, sleeping);

    // mão de carinho
    if(state->petting) {
        mg_ui_hand_draw(canvas, x, y);
    }

    // comida (hambúrguer ou refrigerante)
    if(eating && state->current_food != MinigotchiFoodNone) {
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

    // menu inferior
    canvas_draw_str(canvas, 2, 62, "OK: Love     L: Food     R: Drink");
    //canvas_draw_str(canvas, 2, 62, "BACK: Sair");
}
