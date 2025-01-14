#include "libc/x/x.h"

static _Atomic(void *) __cp950ext_encmap_ptr;
static const unsigned char __cp950ext_encmap_rodata[] = {
  0xed, 0x93, 0x49, 0x0e, 0x82, 0x40, 0x14, 0x44, 0xaf, 0x26, 0x46, 0x71, 0xe7,
  0x1d, 0xbb, 0xbd, 0x89, 0x87, 0x00, 0x1c, 0x01, 0x1b, 0xd0, 0x05, 0x38, 0xe1,
  0xf4, 0x36, 0x40, 0x87, 0xf4, 0x05, 0x48, 0x9a, 0x95, 0x5d, 0x8b, 0x5a, 0xfc,
  0xd4, 0xa2, 0x52, 0x79, 0x7f, 0x2d, 0xe6, 0xa2, 0x75, 0xea, 0x95, 0xae, 0x3c,
  0xf9, 0xdf, 0x0b, 0x3c, 0x0d, 0x0b, 0xb5, 0xf8, 0x51, 0x70, 0xe3, 0xc0, 0x85,
  0x07, 0x31, 0x25, 0x6f, 0x14, 0x57, 0xbe, 0xe4, 0xe8, 0x14, 0x89, 0xf6, 0x17,
  0x27, 0xce, 0xdc, 0x39, 0x52, 0xf1, 0x21, 0xd3, 0x97, 0x9a, 0x14, 0x9b, 0x6d,
  0x1a, 0x36, 0xec, 0xd9, 0x12, 0xb0, 0x23, 0x22, 0x64, 0x39, 0x88, 0xd4, 0xa9,
  0x6c, 0x46, 0x23, 0x7b, 0x62, 0x81, 0x13, 0xe5, 0xfe, 0xae, 0xf5, 0xe5, 0xc2,
  0x2c, 0x39, 0x93, 0x1d,
};

optimizesize void *__cp950ext_encmap(void) {
  return xload(&__cp950ext_encmap_ptr,
               __cp950ext_encmap_rodata,
               121, 1162); /* 10.4131% profit */
}
