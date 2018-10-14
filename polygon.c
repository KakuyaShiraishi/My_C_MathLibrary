#include <SIMDx86/polygon.h>

void SIMDx86Polygon_PlaneNormal(SIMDx86Vector* pNormal, const SIMDx86Polygon* pPoly)
{
	#if defined(USE_SSE)

	asm(
	"movups   (%0), %%xmm0\n"	/* pPoly->a */
	"movups 16(%0), %%xmm1\n"	/* pPoly->b */
	"movups 32(%0), %%xmm2\n"	/* pPoly->c */
	"subps %%xmm1, %%xmm0\n"
	"subps %%xmm1, %%xmm2\n"

	"movaps %%xmm0, %%xmm1\n"	/* xmm0 = xmm1 = Left */
	"movaps %%xmm2, %%xmm3\n"	/* xmm2 = xmm3 = Right */

	/*
		Shuffles
		w | x | z | y  == 1100 1001 = 0xC9
		w | y | x | z  == 1101 0010 = 0xD2
	*/
	"shufps $0xC9, %%xmm0, %%xmm0\n"	/* Left.yxz */
	"shufps $0xD2, %%xmm1, %%xmm1\n"	/* Left.xzy */
	"shufps $0xD2, %%xmm2, %%xmm2\n"	/* Right.xzy */
	"shufps $0xC9, %%xmm3, %%xmm3\n"	/* Right.yxz */

	"mulps %%xmm0, %%xmm2\n"
	"mulps %%xmm1, %%xmm3\n"

	"subps %%xmm3, %%xmm2\n"

	"movups %%xmm2, (%1)\n"
	:
	: "r" (pPoly), "r" (pNormal)
	);


	#else

	SIMDx86Vector d0, d1;

	SIMDx86Vector_DiffOf(&d0, &pPoly->a, &pPoly->b);
	SIMDx86Vector_DiffOf(&d1, &pPoly->c, &pPoly->b);

	SIMDx86Vector_CrossOf(pNormal, &d0, &d1);
	#endif
}
