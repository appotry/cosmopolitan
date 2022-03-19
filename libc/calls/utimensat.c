/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
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
#include "libc/bits/weaken.h"
#include "libc/calls/calls.h"
#include "libc/calls/internal.h"
#include "libc/calls/strace.internal.h"
#include "libc/dce.h"
#include "libc/intrin/asan.internal.h"
#include "libc/sysv/errfuns.h"
#include "libc/zipos/zipos.internal.h"

/**
 * Sets atime/mtime on file, the modern way.
 *
 * @param ts is atime/mtime, or null for current time
 * @param flags can have AT_SYMLINK_NOFOLLOW
 * @note no xnu/rhel5 support if dirfd≠AT_FDCWD∨flags≠0
 * @asyncsignalsafe
 */
int utimensat(int dirfd, const char *path, const struct timespec ts[2],
              int flags) {
  int rc;
  if (IsAsan() && (!__asan_is_valid(path, 1) ||
                   (ts && !__asan_is_valid(ts, sizeof(struct timespec) * 2)))) {
    rc = efault();
  } else if (weaken(__zipos_notat) && (rc = __zipos_notat(dirfd, path)) == -1) {
    STRACE("zipos mkdirat not supported yet");
  } else if (!IsWindows()) {
    rc = sys_utimensat(dirfd, path, ts, flags);
  } else {
    rc = sys_utimensat_nt(dirfd, path, ts, flags);
  }
  if (ts) {
    STRACE("utimensat(%d, %#s, {{%,ld, %,ld}, {%,ld, %,ld}}, %#b) → %d% m",
           dirfd, path, ts[0].tv_sec, ts[0].tv_nsec, ts[1].tv_sec,
           ts[1].tv_nsec, flags, rc);
  } else {
    STRACE("utimensat(%d, %#s, 0, %#b) → %d% m", dirfd, path, flags, rc);
  }
  return rc;
}
