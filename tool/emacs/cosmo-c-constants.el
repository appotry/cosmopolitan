
(defconst cosmo-c-constants-ansi
  '("EOF"
    "WEOF"
    "NDEBUG"
    "HUGE_VAL"
    "HUGE_VALF"
    "HUGE_VALL"
    "CLK_TCK"
    "FLT_ROUNDS"))

(defconst cosmo-c-constants-c11
  '("__func__"
    "__VA_ARGS__"
    "__STDC__"
    "__STDC_HOSTED__"
    "__STDC_VERSION__"
    "__TIME__"
    "__STDC_ISO_10646__"
    "__STDC_MB_MIGHT_NEQ_WC__"
    "__STDC_UTF_16__"
    "__STDC_UTF_32__"
    "__STDC_ANALYZABLE__"
    "__STDC_IEC_559_COMPLEX__"
    "__STDC_LIB_EXT1__"
    "__STDC_NO_ATOMICS__"
    "__STDC_NO_COMPLEX__"
    "__STDC_NO_THREADS__"
    "__STDC_NO_VLA__"
    "__STDC_WANT_LIB_EXT1__"))

(defconst cosmo-c-constants-limits
  '("IMAGE_BASE_VIRTUAL"
    "IMAGE_BASE_REAL"
    "IMAGE_BASE_PHYSICAL"
    "CHAR_MAX"
    "SCHAR_MAX"
    "UCHAR_MAX"
    "SHRT_MAX"
    "INT_MAX"
    "LONG_MAX"
    "LLONG_MAX"
    "LONG_LONG_MAX"
    "SIZE_MAX"
    "INT8_MAX"
    "INT16_MAX"
    "INT32_MAX"
    "INT64_MAX"
    "INT128_MAX"
    "WINT_MAX"
    "WCHAR_MAX"
    "INTPTR_MAX"
    "PTRDIFF_MAX"
    "SCHAR_MIN"
    "UCHAR_MIN"
    "SHRT_MIN"
    "UINT_MIN"
    "INT_MIN"
    "LONG_MIN"
    "LLONG_MIN"
    "LONG_LONG_MIN"
    "SIZE_MIN"
    "INT8_MIN"
    "INT16_MIN"
    "INT32_MIN"
    "INT64_MIN"
    "INT128_MIN"
    "INTMAX_MIN"
    "INTPTR_MIN"
    "WINT_MIN"
    "WCHAR_MIN"
    "PTRDIFF_MIN"
    "USHRT_MAX"
    "UINT_MAX"
    "ULONG_MAX"
    "ULLONG_MAX"
    "ULONG_LONG_MAX"
    "UINTPTR_MAX"
    "UINT8_MAX"
    "UINT16_MAX"
    "UINT32_MAX"
    "UINT64_MAX"
    "UINT128_MAX"
    "USHRT_MIN"
    "ULONG_MIN"
    "ULLONG_MIN"
    "ULONG_LONG_MIN"
    "UINT8_MIN"
    "UINT16_MIN"
    "UINT32_MIN"
    "UINT64_MIN"
    "UINT128_MIN"
    "UINTMAX_MIN"
    "UINTPTR_MIN"
    "MB_CUR_MAX"
    "MB_LEN_MAX"
    "INTMAX_MAX"
    "UINTMAX_MAX"
    "INTMAX_MAX"
    "UINTMAX_MAX"
    "DBL_MIN"
    "DBL_MAX"
    "FLT_MIN"
    "FLT_MAX"))

(defconst cosmo-c-constants-math
  '("NAN"
    "INFINITY"
    "M_E"
    "M_LOG2_10"
    "M_LOG10_2"
    "M_LOG2E"
    "M_LOG10E"
    "M_LN2"
    "M_LN10"
    "M_TAU"
    "M_PI"
    "M_PI_2"
    "M_PI_4"
    "M_1_PI"
    "M_2_PI"
    "M_2_SQRTPI"
    "M_SQRT2"
    "M_SQRT1_2"
    "DBL_DECIMAL_DIG"
    "DBL_DIG"
    "DBL_EPSILON"
    "DBL_MANT_DIG"
    "DBL_MANT_DIG"
    "DBL_MAX"
    "DBL_MAX_10_EXP"
    "DBL_MAX_EXP"
    "DBL_MIN"
    "DBL_MIN_10_EXP"
    "DBL_MIN_EXP"
    "DECIMAL_DIG"
    "FLT_DECIMAL_DIG"
    "FLT_RADIX"
    "FLT_DIG"
    "FLT_EPSILON"
    "FLT_MANT_DIG"
    "FLT_MANT_DIG"
    "FLT_MAX"
    "FLT_MAX_10_EXP"
    "FLT_MAX_EXP"
    "FLT_MIN"
    "FLT_MIN_10_EXP"
    "FLT_MIN_EXP"
    "HLF_MAX"
    "HLF_MIN"
    "LDBL_DECIMAL_DIG"
    "LDBL_DIG"
    "LDBL_EPSILON"
    "LDBL_MANT_DIG"
    "LDBL_MANT_DIG"
    "LDBL_MAX"
    "LDBL_MAX_10_EXP"
    "LDBL_MAX_EXP"
    "LDBL_MIN"
    "LDBL_MIN_10_EXP"
    "LDBL_MIN_EXP"
    "FP_NAN"
    "FP_INFINITE"
    "FP_ZERO"
    "FP_SUBNORMAL"
    "FP_NORMAL"))

(defconst cosmo-c-constants-regex
  (concat "\\_<"
          (regexp-opt (append cosmo-c-constants-ansi
                              cosmo-c-constants-c11
                              cosmo-c-constants-limits
                              cosmo-c-constants-math))
          "\\_>"))

(provide 'cosmo-c-constants)
