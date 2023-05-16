#if !defined _X86INTRIN_H_INCLUDED && !defined _IMMINTRIN_H_INCLUDED
#error "Never use <bmi2intrin.h> directly; include <x86intrin.h> instead."
#endif

#ifndef _BMI2INTRIN_H_INCLUDED
#define _BMI2INTRIN_H_INCLUDED

#ifndef __BMI2__
#pragma GCC push_options
#pragma GCC target("bmi2")
#define __DISABLE_BMI2__
#endif /* __BMI2__ */

__funline unsigned int _bzhi_u32(unsigned int __X, unsigned int __Y) {
  return __builtin_ia32_bzhi_si(__X, __Y);
}

__funline unsigned int _pdep_u32(unsigned int __X, unsigned int __Y) {
  return __builtin_ia32_pdep_si(__X, __Y);
}

__funline unsigned int _pext_u32(unsigned int __X, unsigned int __Y) {
  return __builtin_ia32_pext_si(__X, __Y);
}

#ifdef __x86_64__

__funline unsigned long long _bzhi_u64(unsigned long long __X,
                                     unsigned long long __Y) {
  return __builtin_ia32_bzhi_di(__X, __Y);
}

__funline unsigned long long _pdep_u64(unsigned long long __X,
                                     unsigned long long __Y) {
  return __builtin_ia32_pdep_di(__X, __Y);
}

__funline unsigned long long _pext_u64(unsigned long long __X,
                                     unsigned long long __Y) {
  return __builtin_ia32_pext_di(__X, __Y);
}

__funline unsigned long long _mulx_u64(unsigned long long __X,
                                     unsigned long long __Y,
                                     unsigned long long *__P) {
  unsigned __int128 __res = (unsigned __int128)__X * __Y;
  *__P = (unsigned long long)(__res >> 64);
  return (unsigned long long)__res;
}

#else /* !__x86_64__ */

__funline unsigned int _mulx_u32(unsigned int __X, unsigned int __Y,
                               unsigned int *__P) {
  unsigned long long __res = (unsigned long long)__X * __Y;
  *__P = (unsigned int)(__res >> 32);
  return (unsigned int)__res;
}

#endif /* !__x86_64__  */

#ifdef __DISABLE_BMI2__
#undef __DISABLE_BMI2__
#pragma GCC pop_options
#endif /* __DISABLE_BMI2__ */

#endif /* _BMI2INTRIN_H_INCLUDED */
