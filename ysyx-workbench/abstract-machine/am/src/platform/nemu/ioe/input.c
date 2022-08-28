#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000


/*static uint64_t read_keyboard() {
  uint32_t hi = inl(KBD_ADDR + 4);
  uint32_t lo = inl(KBD_ADDR + 0);
  uint64_t key_value = ((uint64_t)lo << 32) | hi;
  return key_value;
}*/

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = 0;
  kbd->keycode = AM_KEY_NONE;
}
