#ifndef TELEIOS_MESSAGING_CODES
#define TELEIOS_MESSAGING_CODES

// ##################################################################################
//                                Application messages
// ##################################################################################
#define TL_MESSAGE_APPLICATION_QUIT 2500
#define TL_MESSAGE_APPLICATION_PAUSE 2501
#define TL_MESSAGE_APPLICATION_RESUME 2502

// ##################################################################################
//                                  Window messages
// ##################################################################################
#define TL_MESSAGE_WINDOW_ 2600
// width = u16[0], height = u16[1]
#define TL_MESSAGE_WINDOW_RESIZED 2601
// width = u16[0], height = u16[1]
#define TL_MESSAGE_WINDOW_MAXIMIZED 2602
// width = u16[0], height = u16[1]
#define TL_MESSAGE_WINDOW_MINIMIZED 2603
// width = u16[0], height = u16[1]
#define TL_MESSAGE_WINDOW_RESTORED 2604
#define TL_MESSAGE_WINDOW_FOCUS_GAINED 2605
#define TL_MESSAGE_WINDOW_FOCUS_LOST 2606
// xpos = u16[0], ypos = u16[1]
#define TL_MESSAGE_WINDOW_MOVED 2607
// ##################################################################################
//                                   Input messages
// ##################################################################################
#define TL_MESSAGE_INPUT_ 2700
// xpos = i32[0], ypos = i32[1]
#define TL_MESSAGE_INPUT_MOUSE_MOVED 2701
// ydelta = i8[0]
#define TL_MESSAGE_INPUT_MOUSE_WHELLED 2702
// xpos = i32[0], ypos = i32[1]
#define TL_MESSAGE_INPUT_MOUSE_ENTERED 2703
#define TL_MESSAGE_INPUT_MOUSE_LEFT 2704
// button u8[0]
#define TL_MESSAGE_INPUT_MOUSE_PRESSED 2705
// button u8[0]
#define TL_MESSAGE_INPUT_MOUSE_RELEASED 2706
// button u16[0]
#define TL_MESSAGE_INPUT_KEY_PRESSED 277
// button u16[0]
#define TL_MESSAGE_INPUT_KEY_RELEASED 2708


// ##################################################################################
//
// ##################################################################################
#define TL_MESSAGE_ALL_KNOWN 3000

#endif // TELEIOS_MESSAGING_CODES
