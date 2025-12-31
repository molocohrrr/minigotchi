// Minigotchi - UI Food (Implementation)
// Desenha os itens de comida do jogo,
// incluindo hambúrguer e refrigerante,
// posicionados próximos à boca do personagem.

#include "mg_ui_food.h"

static void draw_burger(Canvas* canvas, int x, int y) {
    // topo do pão
    canvas_draw_line(canvas, x + 1, y,     x + 7, y);     // borda superior
    canvas_draw_box(canvas,  x,     y + 1, 9, 2);         // pão de cima (9px)

    // carne saindo 1px pra cada lado
    // linha embaixo do pão
    canvas_draw_line(canvas, x - 1, y + 3, x + 9, y + 3); // carne mais larga (11px)
    // reforço da carne
    canvas_draw_line(canvas, x,     y + 4, x + 8, y + 4);
    // pão de baixo
    canvas_draw_box(canvas,  x,     y + 5, 9, 2);
}

// desenho interno da soda
static void draw_soda(Canvas* canvas, int x, int y) {
    // canudo
    canvas_draw_line(canvas, x + 4, y - 3, x + 6, y - 3);
    canvas_draw_line(canvas, x + 4, y - 3, x + 4, y);

    // borda do copo
    canvas_draw_box(canvas, x,     y,     9, 2);

    // corpo do copo 
    canvas_draw_box(canvas, x + 1, y + 2, 7, 3);
    canvas_draw_box(canvas, x + 2, y + 5, 5, 2);
}

void mg_ui_food_draw(Canvas* canvas, int x, int y, MinigotchiFood food) {
    switch(food) {
    case MinigotchiFoodBurger:
        draw_burger(canvas, x, y);
        break;
    case MinigotchiFoodSoda:
        draw_soda(canvas, x, y);
        break;
    default:
        break;
    }
}

// versão pública pra usar no HUD (mesmo desenho da soda normal)
void mg_ui_food_draw_soda(Canvas* canvas, int x, int y) {
    draw_soda(canvas, x, y);
}
