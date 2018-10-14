%include "SIMDx86asm.inc"

EXPORT_SYMBOL(SIMDx86_sqrtf_387)
EXPORT_SYMBOL(SIMDx86_rsqrtf_387)
EXPORT_SYMBOL(SIMDx86_sqrt_387)
EXPORT_SYMBOL(SIMDx86_rsqrt_387)


singlefp_one:
	dd 1.0
doublefp_one:
	dq 1.0

BEGIN_SYMBOL(SIMDx86_sqrtf_387)
	fld dword [esp+4]
	fsqrt
	ret
END_SYMBOL(SIMDx86_sqrtf_3DNow)


BEGIN_SYMBOL(SIMDx86_rsqrtf_387)
	fld dword [esp+4]
	fsqrt
	fdivr dword [singlefp_one]
	ret
END_SYMBOL(SIMDx86_rsqrtf_387)

BEGIN_SYMBOL(SIMDx86_sqrt_387)
	fld qword [esp+4]
	fsqrt			
	ret
END_SYMBOL(SIMDx86_sqrt_387)


BEGIN_SYMBOL(SIMDx86_rsqrt_387)
	fld qword [esp+4]
	fsqrt
	fdivr qword [doublefp_one]
	ret
END_SYMBOL(SIMDx86_rsqrt_387)