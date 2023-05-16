#ifndef _IMMINTRIN_H_INCLUDED
#error "Never use <avx512ifmaintrin.h> directly; include <immintrin.h> instead."
#endif

#ifndef _AVX512IFMAINTRIN_H_INCLUDED
#define _AVX512IFMAINTRIN_H_INCLUDED

#ifndef __AVX512IFMA__
#pragma GCC push_options
#pragma GCC target("avx512ifma")
#define __DISABLE_AVX512IFMA__
#endif /* __AVX512IFMA__ */

__funline __m512i _mm512_madd52lo_epu64(__m512i __X, __m512i __Y, __m512i __Z) {
  return (__m512i)__builtin_ia32_vpmadd52luq512_mask((__v8di)__X, (__v8di)__Y,
                                                     (__v8di)__Z, (__mmask8)-1);
}

__funline __m512i _mm512_madd52hi_epu64(__m512i __X, __m512i __Y, __m512i __Z) {
  return (__m512i)__builtin_ia32_vpmadd52huq512_mask((__v8di)__X, (__v8di)__Y,
                                                     (__v8di)__Z, (__mmask8)-1);
}

__funline __m512i _mm512_mask_madd52lo_epu64(__m512i __W, __mmask8 __M,
                                           __m512i __X, __m512i __Y) {
  return (__m512i)__builtin_ia32_vpmadd52luq512_mask(
      (__v8di)__W, (__v8di)__X, (__v8di)__Y, (__mmask8)__M);
}

__funline __m512i _mm512_mask_madd52hi_epu64(__m512i __W, __mmask8 __M,
                                           __m512i __X, __m512i __Y) {
  return (__m512i)__builtin_ia32_vpmadd52huq512_mask(
      (__v8di)__W, (__v8di)__X, (__v8di)__Y, (__mmask8)__M);
}

__funline __m512i _mm512_maskz_madd52lo_epu64(__mmask8 __M, __m512i __X,
                                            __m512i __Y, __m512i __Z) {
  return (__m512i)__builtin_ia32_vpmadd52luq512_maskz(
      (__v8di)__X, (__v8di)__Y, (__v8di)__Z, (__mmask8)__M);
}

__funline __m512i _mm512_maskz_madd52hi_epu64(__mmask8 __M, __m512i __X,
                                            __m512i __Y, __m512i __Z) {
  return (__m512i)__builtin_ia32_vpmadd52huq512_maskz(
      (__v8di)__X, (__v8di)__Y, (__v8di)__Z, (__mmask8)__M);
}

#ifdef __DISABLE_AVX512IFMA__
#undef __DISABLE_AVX512IFMA__
#pragma GCC pop_options
#endif /* __DISABLE_AVX512IFMA__ */

#endif /* _AVX512IFMAINTRIN_H_INCLUDED */
