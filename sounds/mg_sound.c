// Minigotchi - Sounds (Implementation)
// Rotinas de som + detecção código para easter egg

#include <furi.h>
#include <furi_hal_speaker.h>
#include <input/input.h>
#include "mg_sound.h"

typedef struct {
    uint16_t freq;
    uint16_t duration_ms;
} Tone;

// Música
static const Tone easter_egg_song[] = {
    {659, 240},
    {0,   30},
    {659, 240},
    {0,   30},
    {659, 240},
    {0,   120},
    {523, 210},
    {0,   30},
    {659, 240},
    {0,   30},
    {784, 310},
    {0,   200},
    {392, 450},
};

static void mg_play_easter_egg_song(void) {
    size_t len = sizeof(easter_egg_song) / sizeof(easter_egg_song[0]);
    if(!furi_hal_speaker_acquire(100)) {
        return;
    }

    for(size_t i = 0; i < len; i++) {
        uint16_t freq = easter_egg_song[i].freq;
        uint16_t dur  = easter_egg_song[i].duration_ms;

        if(freq > 0) {
            furi_hal_speaker_start(freq, 0.5f);
        } else {
            furi_hal_speaker_stop();
        }

        furi_delay_ms(dur);

        if(freq > 0 && i + 1 < len) {
            furi_hal_speaker_stop();
            furi_delay_ms(30);
        }
    }

    furi_hal_speaker_stop();
    furi_hal_speaker_release();
}

// Sequência
#define MARIO_LEN 5

static const InputKey mario_code[MARIO_LEN] = {
    InputKeyUp,
    InputKeyRight,
    InputKeyDown,
    InputKeyLeft,
    InputKeyOk,
};

static InputKey mario_buffer[MARIO_LEN];
static int mario_index = 0;
static bool mario_initialized = false;

static void mario_reset(void) {
    for(int i = 0; i < MARIO_LEN; i++) {
        mario_buffer[i] = InputKeyBack;
    }
    mario_index = 0;
    mario_initialized = true;
}

void mg_mario_push(InputKey key) {
    if(!mario_initialized) {
        mario_reset();
    }

    mario_buffer[mario_index] = key;
    mario_index = (mario_index + 1) % MARIO_LEN;

    int start = mario_index;
    bool match = true;

    for(int i = 0; i < MARIO_LEN; i++) {
        int idx = (start + i) % MARIO_LEN;
        if(mario_buffer[idx] != mario_code[i]) {
            match = false;
            break;
        }
    }

    if(match) {
        mg_play_easter_egg_song();
        mario_reset();
    }
}
