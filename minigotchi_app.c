// Minigotchi - Application Entry Point
// Integra o jogo com o Flipper Zero:
// configura GUI, fila de input, loop principal,
// delega lógica para core/ e desenho para ui/.

#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <storage/storage.h>
#include "core/mg_state.h"
#include "core/mg_game.h"
#include "ui/mg_ui_screen.h"

#define MINIGOTCHI_SAVE_DIR  "apps_data/minigotchi"
#define MINIGOTCHI_SAVE_FILE "apps_data/minigotchi/state.bin"
#define MINIGOTCHI_SAVE_PATH EXT_PATH(MINIGOTCHI_SAVE_FILE)

typedef struct {
    MinigotchiState state;
    Gui* gui;
    ViewPort* view_port;
    FuriMessageQueue* input_queue;
} MinigotchiApp;

static void minigotchi_draw_callback(Canvas* canvas, void* ctx) {
    MinigotchiApp* app = ctx;
    mg_ui_screen_draw(canvas, &app->state);
}

static void minigotchi_input_callback(InputEvent* event, void* ctx) {
    MinigotchiApp* app = ctx;
    furi_message_queue_put(app->input_queue, event, FuriWaitForever);
}

static bool minigotchi_load_state(MinigotchiState* state) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);

    bool ok = false;

    if(storage_file_open(file, MINIGOTCHI_SAVE_PATH, FSAM_READ, FSOM_OPEN_EXISTING)) {
        uint32_t read = storage_file_read(file, state, sizeof(MinigotchiState));
        if(read == sizeof(MinigotchiState)) {
            ok = true;
        }
        storage_file_close(file);
    }

    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);

    return ok;
}

static bool minigotchi_save_state(const MinigotchiState* state_in) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);

    bool ok = false;

    storage_common_mkdir(storage, EXT_PATH("apps_data"));
    storage_common_mkdir(storage, EXT_PATH(MINIGOTCHI_SAVE_DIR));
    MinigotchiState state = *state_in;

    // campos de sessão NÃO devem ser persistidos
    state.running = true;                      // sempre true no disco
    state.petting = false;
    state.eating = false;
    state.current_food = MinigotchiFoodNone;
    state.pet_end_tick = 0;
    state.eat_end_tick = 0;
    state.last_move_tick = 0;                 // vai ser ressincronizado no load

    if(storage_file_open(file, MINIGOTCHI_SAVE_PATH, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        uint32_t written = storage_file_write(file, &state, sizeof(MinigotchiState));
        if(written == sizeof(MinigotchiState)) {
            ok = true;
        }
        storage_file_close(file);
    }

    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);

    return ok;
}

static MinigotchiApp* minigotchi_app_alloc(void) {
    MinigotchiApp* app = malloc(sizeof(MinigotchiApp));

    bool loaded = minigotchi_load_state(&app->state);
    if(!loaded) {
        // 1ª vez: cria estado novo completo
        mg_init(&app->state);
        minigotchi_save_state(&app->state);
    } else {
        // veio do disco
        app->state.running = true;
        app->state.petting = false;
        app->state.eating = false;
        app->state.current_food = MinigotchiFoodNone;
        app->state.pet_end_tick = 0;
        app->state.eat_end_tick = 0;
        app->state.last_move_tick = furi_get_tick(); // ressincroniza animação
    }

    app->input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, minigotchi_draw_callback, app);
    view_port_input_callback_set(app->view_port, minigotchi_input_callback, app);

    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    return app;
}

static void minigotchi_app_free(MinigotchiApp* app) {
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);
    furi_message_queue_free(app->input_queue);

    free(app);
}

int32_t minigotchi_app(void* p) {
    UNUSED(p);

    MinigotchiApp* app = minigotchi_app_alloc();
    MinigotchiForm last_form = app->state.form;

    while(app->state.running) {
        InputEvent event;
        FuriStatus status = furi_message_queue_get(app->input_queue, &event, 50);

        uint32_t now = furi_get_tick();

        if(status == FuriStatusOk) {
            if(event.type == InputTypeShort) {
                if(event.key == InputKeyOk) {
                    mg_pet(&app->state, now);
                } else if(event.key == InputKeyLeft) {
                    mg_feed(&app->state, now, MinigotchiFoodBurger);
                    minigotchi_save_state(&app->state);
                } else if(event.key == InputKeyRight) {
                    mg_feed(&app->state, now, MinigotchiFoodSoda);
                    minigotchi_save_state(&app->state);
                } else if(event.key == InputKeyBack) {
                    minigotchi_save_state(&app->state);
                    app->state.running = false;
                }
            }
        }

        mg_update(&app->state, now);

        if(app->state.form != last_form) {
            minigotchi_save_state(&app->state);
            last_form = app->state.form;
        }

        view_port_update(app->view_port);
    }

    minigotchi_app_free(app);
    return 0;
}
