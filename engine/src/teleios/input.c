#include "teleios/teleios.h"
#include "teleios/messagingcodes.h"

static b8 key_curr[TL_KEY_MAXIMUM];
static b8 key_prev[TL_KEY_MAXIMUM];

static b8 mouse_curr[TL_MOUSE_MAXIMUM];
static b8 mouse_prev[TL_MOUSE_MAXIMUM];
static ivec2s mouse_pos;
static i8 mouse_scroll;

static TLMessageChain tl_input_handle(const u16 code, const TLMessage* message) {
    TLDIAGNOSTICS_PUSH;
    switch (code) {
        case TL_MESSAGE_INPUT_KEY_PRESSED: key_curr[message->u16[0]] = true; break;
        case TL_MESSAGE_INPUT_KEY_RELEASED: key_curr[message->u16[0]] = false; break;
        case TL_MESSAGE_INPUT_MOUSE_PRESSED: mouse_curr[message->u8[0]] = false; break;
        case TL_MESSAGE_INPUT_MOUSE_RELEASED: mouse_curr[message->u8[0]] = false; break;
        case TL_MESSAGE_INPUT_MOUSE_WHELLED: mouse_scroll = message->i8[0]; break;
        case TL_MESSAGE_INPUT_MOUSE_MOVED: mouse_pos.x = message->i32[0]; mouse_pos.y = message->i32[1]; break;
    }
    TLDIAGNOSTICS_POP;

    return TL_MESSAGE_AVALIABLE;
}

b8 tl_input_initialize(void) {
    TLDIAGNOSTICS_PUSH;
    tl_memory_zero((void*)key_curr, sizeof(key_curr));
    tl_memory_zero((void*)key_prev, sizeof(key_curr));

    tl_messaging_subscribe(TL_MESSAGE_INPUT_KEY_PRESSED, tl_input_handle);
    tl_messaging_subscribe(TL_MESSAGE_INPUT_KEY_RELEASED, tl_input_handle);

    tl_memory_zero((void*)mouse_curr, sizeof(mouse_curr));
    tl_memory_zero((void*)key_prev, sizeof(mouse_curr));

    tl_messaging_subscribe(TL_MESSAGE_INPUT_MOUSE_PRESSED, tl_input_handle);
    tl_messaging_subscribe(TL_MESSAGE_INPUT_MOUSE_RELEASED, tl_input_handle);
    tl_messaging_subscribe(TL_MESSAGE_INPUT_MOUSE_MOVED, tl_input_handle);
    tl_messaging_subscribe(TL_MESSAGE_INPUT_MOUSE_WHELLED, tl_input_handle);

    mouse_scroll = 0;
    tl_memory_zero((void*)&mouse_pos, sizeof(mouse_pos));

    TLDIAGNOSTICS_POP;
    return true;
}

void tl_input_update(void) {
    TLDIAGNOSTICS_PUSH;
    tl_memory_copy((void*)key_curr, (void*)key_prev, sizeof(key_curr));
    tl_memory_copy((void*)mouse_curr, (void*)mouse_prev, sizeof(mouse_curr));
    TLDIAGNOSTICS_POP;
}

b8 tl_input_key_active(const TLInputKey key) { return key_curr[key]; }
b8 tl_input_key_pressed(const TLInputKey key) { return !key_prev[key] && key_curr[key]; }
b8 tl_input_key_released(const TLInputKey key) { return key_prev[key] && !key_curr[key]; }

i8 tl_input_mouse_scrool(void) { return mouse_scroll; }
ivec2s tl_input_mouse_position(void) { return mouse_pos; }
b8 tl_input_mouse_active(const TLInputMouse button) { return mouse_curr[button]; }
b8 tl_input_mouse_pressed(const TLInputMouse button) { return !mouse_prev[button] && mouse_curr[button]; }
b8 tl_input_mouse_released(const TLInputMouse button) { return mouse_prev[button] && !mouse_curr[button]; }

b8 tl_input_terminate(void) {
    TLDIAGNOSTICS_PUSH;
    mouse_scroll = 0;
    tl_memory_zero((void*)key_curr, sizeof(key_curr));
    tl_memory_zero((void*)key_prev, sizeof(key_curr));
    tl_memory_zero((void*)mouse_curr, sizeof(mouse_curr));
    tl_memory_zero((void*)key_prev, sizeof(mouse_curr));
    tl_memory_zero((void*)&mouse_pos, sizeof(mouse_pos));
    TLDIAGNOSTICS_POP;
    return true;
}