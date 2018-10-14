%include "SIMDx86asm.inc"


EXPORT_SYMBOL(SIMDx86_sqrtf_HIPREC_SSE)
EXPORT_SYMBOL(SIMDx86_sqrtf_SSE)
EXPORT_SYMBOL(SIMDx86_rsqrtf_HIPREC_SSE)
EXPORT_SYMBOL(SIMDx86_rsqrtf_SSE)
EXPORT_SYMBOL(SIMDx86_sqrt_SSE2)
EXPORT_SYMBOL(SIMDx86_rsqrt_SSE2)


EXTERN _SIMDx86_double_one
EXTERN _SIMDx86_float_one


BEGIN_SYMBOL(SIMDx86_sqrtf_HIPREC_SSE)
	movlps xmm0, [esp+4]
    sqrtss xmm0, xmm0
	movss [esp-4],xmm0
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_sqrtf_HIPREC_SSE)
SIZE_SYMBOL(SIMDx86_sqrtf_HIPREC_SSE)


BEGIN_SYMBOL(SIMDx86_sqrtf_SSE)
	movlps xmm0, [esp+4]
	rsqrtss xmm0, xmm0
	rcpss xmm0, xmm0
	movss [esp-4], xmm0
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_sqrtf_SSE)
SIZE_SYMBOL(SIMDx86_sqrtf_SSE)


BEGIN_SYMBOL(SIMDx86_rsqrtf_HIPREC_SSE)
	movlps xmm0, [esp+4]
	movaps xmm1, [_SIMDx86_float_one]
	sqrtss xmm0, xmm0
	divss xmm1, xmm0
	movss [esp-4], xmm1
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_rsqrtf_HIPREC_SSE)
SIZE_SYMBOL(SIMDx86_rsqrtf_HIPREC_SSE)


BEGIN_SYMBOL(SIMDx86_rsqrtf_SSE)
	movlps xmm0, [esp+4]
	rsqrtss xmm0, xmm0
	movss [esp-4], xmm0
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_rsqrtf_SSE)
SIZE_SYMBOL(SIMDx86_rsqrtf_SSE)


BEGIN_SYMBOL(SIMDx86_sqrt_SSE2)
	movlpd xmm0, [esp+4]
	sqrtsd xmm0, xmm0
	movlpd [esp-4], xmm0
	fld dword [esp-4]
	ret
END_SYMBOL SIMDx86_sqrt_SSE2
SIZE_SYMBOL(SIMDx86_sqrt_SSE2)


BEGIN_SYMBOL(SIMDx86_rsqrt_SSE2)
	movlpd xmm0, [esp+4]
    movapd xmm1, [_SIMDx86_double_one]
	sqrtsd xmm0, xmm0
	divsd xmm1, xmm0						
	movlpd [esp-4], xmm1
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_rsqrt_SSE2)
SIZE_SYMBOL(SIMDx86_rsqrt_SSE2)