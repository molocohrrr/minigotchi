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
    canvas_draw_str(canvas, 2, 10, "M i n i G o t c h i");
    canvas_draw_str(canvas, 3, 10, "M i n i G o t c h i");

    int x_positions[3] = {20, 60, 100};

    // flags derivadas do estado
    bool happy  = state->petting;      // carinho
    bool eating = state->eating;       // comendo
    bool hungry = state->hungry;       // fominha!

    // se estiver feliz, comendo ou fominha, fica no centro
    int index = (happy || eating || hungry) ? 1 : state->position_index;

    int x = x_positions[index];
    int y = 32;

    // corpo / boca / coração / boca mastigando / cara triste
    mg_ui_body_draw(canvas, x, y, happy, eating, hungry);

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

    // menu inferior
    canvas_draw_str(canvas, 2, 62, "OK: Love     L: Food     R: Drink");
    //canvas_draw_str(canvas, 2, 62, "BACK: Sair");
}
