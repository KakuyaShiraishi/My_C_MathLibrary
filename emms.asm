%include "SIMDx86asm.inc"

EXPORT_SYMBOL(SIMDx86_emms)
EXPORT_SYMBOL(SIMDx86_femms)

BEGIN_SYMBOL(SIMDx86_emms)
	emms
	ret
END_SYMBOL(SIMDx86_emms)

BEGIN_SYMBOL(SIMDx86_femms)
	femms
	ret
END_SYMBOL(SIMDx86_femms)
