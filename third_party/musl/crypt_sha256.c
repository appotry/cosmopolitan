/*-*- mode:c;indent-tabs-mode:t;c-basic-offset:8;tab-width:8;coding:utf-8   -*-│
│vi: set et ft=c ts=8 tw=8 fenc=utf-8                                       :vi│
╚──────────────────────────────────────────────────────────────────────────────╝
│                                                                              │
│  Musl Libc                                                                   │
│  Copyright © 2005-2014 Rich Felker, et al.                                   │
│                                                                              │
│  Permission is hereby granted, free of charge, to any person obtaining       │
│  a copy of this software and associated documentation files (the             │
│  "Software"), to deal in the Software without restriction, including         │
│  without limitation the rights to use, copy, modify, merge, publish,         │
│  distribute, sublicense, and/or sell copies of the Software, and to          │
│  permit persons to whom the Software is furnished to do so, subject to       │
│  the following conditions:                                                   │
│                                                                              │
│  The above copyright notice and this permission notice shall be              │
│  included in all copies or substantial portions of the Software.             │
│                                                                              │
│  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,             │
│  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF          │
│  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.      │
│  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY        │
│  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,        │
│  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE           │
│  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                      │
│                                                                              │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/mem/alg.h"
#include "libc/calls/calls.h"
#include "libc/fmt/conv.h"
#include "libc/fmt/fmt.h"
#include "libc/limits.h"
#include "libc/literal.h"
#include "libc/mem/mem.h"
#include "libc/nexgen32e/nexgen32e.h"
#include "libc/stdio/rand.h"
#include "libc/runtime/runtime.h"
#include "libc/stdio/stdio.h"
#include "libc/stdio/temp.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/exit.h"
#include "third_party/gdtoa/gdtoa.h"
#include "third_party/musl/crypt.internal.h"

#pragma GCC diagnostic ignored "-Wmissing-braces"

asm(".ident\t\"\\n\\n\
Musl libc (MIT License)\\n\
Copyright 2005-2014 Rich Felker, et. al.\"");
asm(".include \"libc/disclaimer.inc\"");
// clang-format off

/*
 * public domain sha256 crypt implementation
 *
 * original sha crypt design: http://people.redhat.com/drepper/SHA-crypt.txt
 * in this implementation at least 32bit int is assumed,
 * key length is limited, the $5$ prefix is mandatory, '\n' and ':' is rejected
 * in the salt and rounds= setting must contain a valid iteration count,
 * on error "*" is returned.
 */

/* public domain sha256 implementation based on fips180-3 */

struct sha256 {
	uint64_t len;    /* processed message length */
	uint32_t h[8];   /* hash state */
	uint8_t buf[64]; /* message block buffer */
};

static uint32_t ror(uint32_t n, int k) { return (n >> k) | (n << (32-k)); }
#define Ch(x,y,z)  (z ^ (x & (y ^ z)))
#define Maj(x,y,z) ((x & y) | (z & (x | y)))
#define S0(x)      (ror(x,2) ^ ror(x,13) ^ ror(x,22))
#define S1(x)      (ror(x,6) ^ ror(x,11) ^ ror(x,25))
#define R0(x)      (ror(x,7) ^ ror(x,18) ^ (x>>3))
#define R1(x)      (ror(x,17) ^ ror(x,19) ^ (x>>10))

static void processblock(struct sha256 *s, const uint8_t *buf)
{
	uint32_t W[64], t1, t2, a, b, c, d, e, f, g, h;
	int i;

	for (i = 0; i < 16; i++) {
		W[i] = (uint32_t)buf[4*i]<<24;
		W[i] |= (uint32_t)buf[4*i+1]<<16;
		W[i] |= (uint32_t)buf[4*i+2]<<8;
		W[i] |= buf[4*i+3];
	}
	for (; i < 64; i++)
		W[i] = R1(W[i-2]) + W[i-7] + R0(W[i-15]) + W[i-16];
	a = s->h[0];
	b = s->h[1];
	c = s->h[2];
	d = s->h[3];
	e = s->h[4];
	f = s->h[5];
	g = s->h[6];
	h = s->h[7];
	for (i = 0; i < 64; i++) {
		t1 = h + S1(e) + Ch(e,f,g) + kSha256[i] + W[i];
		t2 = S0(a) + Maj(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}
	s->h[0] += a;
	s->h[1] += b;
	s->h[2] += c;
	s->h[3] += d;
	s->h[4] += e;
	s->h[5] += f;
	s->h[6] += g;
	s->h[7] += h;
}

static void pad(struct sha256 *s)
{
	unsigned r = s->len % 64;

	s->buf[r++] = 0x80;
	if (r > 56) {
		memset(s->buf + r, 0, 64 - r);
		r = 0;
		processblock(s, s->buf);
	}
	memset(s->buf + r, 0, 56 - r);
	s->len *= 8;
	s->buf[56] = s->len >> 56;
	s->buf[57] = s->len >> 48;
	s->buf[58] = s->len >> 40;
	s->buf[59] = s->len >> 32;
	s->buf[60] = s->len >> 24;
	s->buf[61] = s->len >> 16;
	s->buf[62] = s->len >> 8;
	s->buf[63] = s->len;
	processblock(s, s->buf);
}

static void sha256_init(struct sha256 *s)
{
	s->len = 0;
	s->h[0] = 0x6a09e667;
	s->h[1] = 0xbb67ae85;
	s->h[2] = 0x3c6ef372;
	s->h[3] = 0xa54ff53a;
	s->h[4] = 0x510e527f;
	s->h[5] = 0x9b05688c;
	s->h[6] = 0x1f83d9ab;
	s->h[7] = 0x5be0cd19;
}

static void sha256_sum(struct sha256 *s, uint8_t *md)
{
	int i;

	pad(s);
	for (i = 0; i < 8; i++) {
		md[4*i] = s->h[i] >> 24;
		md[4*i+1] = s->h[i] >> 16;
		md[4*i+2] = s->h[i] >> 8;
		md[4*i+3] = s->h[i];
	}
}

static void sha256_update(struct sha256 *s, const void *m, unsigned long len)
{
	const uint8_t *p = m;
	unsigned r = s->len % 64;

	s->len += len;
	if (r) {
		if (len < 64 - r) {
			memcpy(s->buf + r, p, len);
			return;
		}
		memcpy(s->buf + r, p, 64 - r);
		len -= 64 - r;
		p += 64 - r;
		processblock(s, s->buf);
	}
	for (; len >= 64; len -= 64, p += 64)
		processblock(s, p);
	memcpy(s->buf, p, len);
}

static const unsigned char b64[] =
"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static char *to64(char *s, unsigned int u, int n)
{
	while (--n >= 0) {
		*s++ = b64[u % 64];
		u /= 64;
	}
	return s;
}

/* key limit is not part of the original design, added for DoS protection.
 * rounds limit has been lowered (versus the reference/spec), also for DoS
 * protection. runtime is O(klen^2 + klen*rounds) */
#define KEY_MAX 256
#define SALT_MAX 16
#define ROUNDS_DEFAULT 5000
#define ROUNDS_MIN 1000
#define ROUNDS_MAX 9999999

/* hash n bytes of the repeated md message digest */
static void hashmd(struct sha256 *s, unsigned int n, const void *md)
{
	unsigned int i;

	for (i = n; i > 32; i -= 32)
		sha256_update(s, md, 32);
	sha256_update(s, md, i);
}

static char *sha256crypt(const char *key, const char *setting, char *output)
{
	struct sha256 ctx;
	unsigned char md[32], kmd[32], smd[32];
	unsigned int i, r, klen, slen;
	char rounds[20] = "";
	const char *salt;
	char *p;

	/* reject large keys */
	klen = strnlen(key, KEY_MAX+1);
	if (klen > KEY_MAX)
		return 0;

	/* setting: $5$rounds=n$salt$ (rounds=n$ and closing $ are optional) */
	if (strncmp(setting, "$5$", 3) != 0)
		return 0;
	salt = setting + 3;

	r = ROUNDS_DEFAULT;
	if (strncmp(salt, "rounds=", sizeof "rounds=" - 1) == 0) {
		unsigned long u;
		char *end;

		/*
		 * this is a deviation from the reference:
		 * bad rounds setting is rejected if it is
		 * - empty
		 * - unterminated (missing '$')
		 * - begins with anything but a decimal digit
		 * the reference implementation treats these bad
		 * rounds as part of the salt or parse them with
		 * strtoul semantics which may cause problems
		 * including non-portable hashes that depend on
		 * the host's value of ULONG_MAX.
		 */
		salt += sizeof "rounds=" - 1;
		if (!isdigit(*salt))
			return 0;
		u = strtoul(salt, &end, 10);
		if (*end != '$')
			return 0;
		salt = end+1;
		if (u < ROUNDS_MIN)
			r = ROUNDS_MIN;
		else if (u > ROUNDS_MAX)
			return 0;
		else
			r = u;
		/* needed when rounds is zero prefixed or out of bounds */
		(sprintf)(rounds, "rounds=%u$", r);
	}

	for (i = 0; i < SALT_MAX && salt[i] && salt[i] != '$'; i++)
		/* reject characters that interfere with /etc/shadow parsing */
		if (salt[i] == '\n' || salt[i] == ':')
			return 0;
	slen = i;

	/* B = sha(key salt key) */
	sha256_init(&ctx);
	sha256_update(&ctx, key, klen);
	sha256_update(&ctx, salt, slen);
	sha256_update(&ctx, key, klen);
	sha256_sum(&ctx, md);

	/* A = sha(key salt repeat-B alternate-B-key) */
	sha256_init(&ctx);
	sha256_update(&ctx, key, klen);
	sha256_update(&ctx, salt, slen);
	hashmd(&ctx, klen, md);
	for (i = klen; i > 0; i >>= 1)
		if (i & 1)
			sha256_update(&ctx, md, sizeof md);
		else
			sha256_update(&ctx, key, klen);
	sha256_sum(&ctx, md);

	/* DP = sha(repeat-key), this step takes O(klen^2) time */
	sha256_init(&ctx);
	for (i = 0; i < klen; i++)
		sha256_update(&ctx, key, klen);
	sha256_sum(&ctx, kmd);

	/* DS = sha(repeat-salt) */
	sha256_init(&ctx);
	for (i = 0; i < 16 + md[0]; i++)
		sha256_update(&ctx, salt, slen);
	sha256_sum(&ctx, smd);

	/* iterate A = f(A,DP,DS), this step takes O(rounds*klen) time */
	for (i = 0; i < r; i++) {
		sha256_init(&ctx);
		if (i % 2)
			hashmd(&ctx, klen, kmd);
		else
			sha256_update(&ctx, md, sizeof md);
		if (i % 3)
			sha256_update(&ctx, smd, slen);
		if (i % 7)
			hashmd(&ctx, klen, kmd);
		if (i % 2)
			sha256_update(&ctx, md, sizeof md);
		else
			hashmd(&ctx, klen, kmd);
		sha256_sum(&ctx, md);
	}

	/* output is $5$rounds=n$salt$hash */
	p = output;
	p += (sprintf)(p, "$5$%s%.*s$", rounds, slen, salt);
	static const unsigned char perm[][3] = {
		0,10,20,21,1,11,12,22,2,3,13,23,24,4,14,
		15,25,5,6,16,26,27,7,17,18,28,8,9,19,29 };
	for (i=0; i<10; i++) p = to64(p,
		(md[perm[i][0]]<<16)|(md[perm[i][1]]<<8)|md[perm[i][2]], 4);
	p = to64(p, (md[31]<<8)|md[30], 3);
	*p = 0;
	return output;
}

char *__crypt_sha256(const char *key, const char *setting, char *output)
{
	static const char testkey[] = "Xy01@#\x01\x02\x80\x7f\xff\r\n\x81\t !";
	static const char testsetting[] = "$5$rounds=1234$abc0123456789$";
	static const char testhash[] = "$5$rounds=1234$abc0123456789$3VfDjPt05VHFn47C/ojFZ6KRPYrOjj1lLbH.dkF3bZ6";
	char testbuf[128];
	char *p, *q;

	p = sha256crypt(key, setting, output);
	/* self test and stack cleanup */
	q = sha256crypt(testkey, testsetting, testbuf);
	if (!p || q != testbuf || memcmp(testbuf, testhash, sizeof testhash))
		return "*";
	return p;
}
