%include "SIMDx86asm.inc"

EXPORT_SYMBOL(SIMDx86_sqrtf_HIPREC_3DNow)
EXPORT_SYMBOL(SIMDx86_sqrtf_3DNow)
EXPORT_SYMBOL(SIMDx86_rsqrtf_HIPREC_3DNow)
EXPORT_SYMBOL(SIMDx86_rsqrtf_3DNow)

BEGIN_SYMBOL(SIMDx86_sqrtf_HIPREC_3DNow)
	movd mm0, [esp+4]
	pfrsqrt mm1, mm0
	movq mm1, mm2
	pfmul mm1, mm1
punpckldq mm0, mm0
	pfrsqit1 mm1, mm0
	pfrcpit2 mm2, mm1
	pfmul mm0, mm2
movd [esp-4], mm0
	femms
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_sqrtf_HIPREC_3DNow)

BEGIN_SYMBOL(SIMDx86_sqrtf_3DNow)
	movd mm0, [esp+4]
	pfrsqrt mm1, mm0
	pfmul mm1, mm0
	movd [esp-4], mm0
	femms
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_sqrtf_3DNow)

BEGIN_SYMBOL(SIMDx86_rsqrtf_HIPREC_3DNow)
	movd mm0, [esp+4]
	pfrsqrt mm1, mm0
	movq mm2, mm1
	pfmul mm1, mm1
	punpckldq mm0, mm0
	pfrsqit1 mm1, mm0
	pfrcpit2 mm2, mm2
	movd [esp-4], mm2
	femms
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_rsqrtf_HIPREC_3DNow)

BEGIN_SYMBOL(SIMDx86_rsqrtf_3DNow)
	movd mm0, [esp+4]
	pfrsqrt mm0, mm0
	movd [esp-4], mm0
	femms
	fld dword [esp-4]
	ret
END_SYMBOL(SIMDx86_rsqrtf_3DNow)