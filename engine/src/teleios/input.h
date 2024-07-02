#ifndef TELEIOS_INPUT
#define TELEIOS_INPUT

#include "teleios/types.h"
#include "teleios/inputcodes.h"

b8 tl_input_initialize(void);
void tl_input_update(void);
b8 tl_input_terminate(void);

b8 tl_input_key_active(const TLInputKey key);
b8 tl_input_key_pressed(const TLInputKey key);
b8 tl_input_key_released(const TLInputKey key);

i8 tl_input_mouse_scrool(void);
ivec2s tl_input_mouse_position(void);
b8 tl_input_mouse_active(const TLInputMouse button);
b8 tl_input_mouse_pressed(const TLInputMouse button);
b8 tl_input_mouse_released(const TLInputMouse button);

#endif // TELEIOS_INPUT