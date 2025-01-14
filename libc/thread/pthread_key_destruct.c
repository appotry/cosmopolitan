/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2022 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/nexgen32e/bsr.h"
#include "libc/thread/internal.h"
#include "libc/thread/thread.h"

void _pthread_key_destruct(void *key[PTHREAD_KEYS_MAX]) {
  int i, j;
  uint64_t x;
  void *value;
  pthread_key_dtor dtor;
StartOver:
  for (i = 0; i < (PTHREAD_KEYS_MAX + 63) / 64; ++i) {
    x = _pthread_key_usage[i];
    while (x) {
      j = bsrl(x);
      if ((dtor = _pthread_key_dtor[i * 64 + j]) && (value = key[i * 64 + j])) {
        key[i * 64 + j] = 0;
        dtor(value);
        goto StartOver;
      }
      x &= ~(1ul << j);
    }
  }
}
