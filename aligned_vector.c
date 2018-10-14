#include <SIMDx86/vector.h>
#include <SIMDx86/consts.h>
#include <float.h>
#include <math.h>

void SIMDx86Vector_AlignedSum(SIMDx86Vector* pOut, const SIMDx86Vector* pIn)
{
	#if defined(USE_SSE)

	asm(
	"movaps (%0), %%xmm0\n"
	"addps (%1), %%xmm0\n"
	"movaps %%xmm0, (%0)\n"
	:
	: "r" (pOut), "r" (pIn)
	);
	#elif defined(USE_3DNOW)

	asm(
	"movq  (%0), %%mm0\n"
	"movq 8(%0), %%mm1\n"
	"pfadd  (%1), %%mm0\n"
	"pfadd 8(%1), %%mm1\n"
	"movq %%mm0,  (%0)\n"
	"movq %%mm1, 8(%0)\n"
	:
	: "r" (pOut), "r" (pIn)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif


	#else
	pOut->x += pIn->x;
	pOut->y += pIn->y;
	pOut->z += pIn->z;
	#endif
}

void SIMDx86Vector_AlignedSumOf(SIMDx86Vector* pOut, const SIMDx86Vector* pIn1, const SIMDx86Vector* pIn2)
{
	#if defined(USE_SSE)

	asm(
	"movaps (%0), %%xmm0\n"
	"addps (%1), %%xmm0\n"
	"movaps %%xmm0, (%2)\n"
	:
	: "r" (pIn1), "r" (pIn2), "r" (pOut)
	);

	#elif defined(USE_3DNOW)

	asm(
	"movq  (%0), %%mm0\n"
	"movq 8(%0), %%mm1\n"
	"pfadd  (%1), %%mm0\n"
	"pfadd 8(%1), %%mm1\n"
	"movq %%mm0,  (%2)\n"
	"movq %%mm1, 8(%2)\n"
	:
	: "r" (pIn1), "r" (pIn2), "r" (pOut)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif


	#else
	pOut->x = pIn1->x + pIn2->x;
	pOut->y = pIn1->y + pIn2->y;
	pOut->z = pIn1->z + pIn2->z;
	#endif
}


void SIMDx86Vector_AlignedDiff(SIMDx86Vector* pLeft, SIMDx86Vector* pRight)
{

	#if defined(USE_SSE)

	asm(
	"movaps (%0), %%xmm0\n"		/* xmm0 = pLeft */
	"subps (%1), %%xmm0\n"	/* xmm0 = pLeft - pRight */
	"movaps %%xmm0, (%0)\n"		/* store */
	:
	: "r" (pLeft), "r" (pRight)
	);

	#elif defined(USE_3DNOW)

	asm(
	"movq  (%0), %%mm0\n"
	"movq 8(%0), %%mm1\n"
	"pfsub  (%1), %%mm0\n"
	"pfsub 8(%1), %%mm1\n"
	"movq %%mm0,  (%0)\n"
	"movq %%mm1, 8(%0)\n"
	:
	: "r" (pLeft), "r" (pRight)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif


	#else
	pLeft->x -= pRight->x;
	pLeft->y -= pRight->y;
	pLeft->z -= pRight->z;
	#endif
}

void SIMDx86Vector_AlignedDiffOf(SIMDx86Vector* pOut, const SIMDx86Vector* pLeft, const SIMDx86Vector* pRight)
{
	#if defined(USE_SSE)

	asm(
	"movaps (%0), %%xmm0\n"		/* xmm0 = pLeft */
	"subps (%1), %%xmm0\n"	/* xmm0 = pLeft - pRight */
	"movaps %%xmm0, (%2)\n"		/* store */
	:
	: "r" (pLeft), "r" (pRight), "r" (pOut)
	);


	#elif defined(USE_3DNOW)

	asm(
	"movq  (%0), %%mm0\n"
	"movq 8(%0), %%mm1\n"
	"pfsub  (%1), %%mm0\n"
	"pfsub 8(%1), %%mm1\n"
	"movq %%mm0,  (%2)\n"
	"movq %%mm1, 8(%2)\n"
	:
	: "r" (pLeft), "r" (pRight), "r" (pOut)

	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif


	#else

	pOut->x = pLeft->x - pRight->x;
	pOut->y = pLeft->y - pRight->y;
	pOut->z = pLeft->z - pRight->z;
	#endif
}

void SIMDx86Vector_AlignedScale(SIMDx86Vector* pOut, float scalar)
{
	#if defined(USE_SSE)

	asm(
	"movss %1, %%xmm0\n"
	"shufps $0x00, %%xmm0, %%xmm0\n"
	"mulps (%0), %%xmm0\n"
	"movaps %%xmm0, (%0)\n"
	:
	: "r" (pOut), "m" (scalar)
	);

	#elif defined(USE_3DNOW)

	asm(
	"movd %1, %%mm0\n"
	"punpckldq %%mm0, %%mm0\n"
	"movq %%mm0, %%mm1\n"
	"pfmul  (%0), %%mm0\n"
	"pfmul 8(%0), %%mm1\n"
	"movq %%mm0,  (%0)\n"
	"movq %%mm1, 8(%0)\n"
	:
	: "r" (pOut), "m" (scalar)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif

	#else

	pOut->x *= scalar;
	pOut->y *= scalar;
	pOut->z *= scalar;

	#endif
}

void SIMDx86Vector_AlignedScaleOf(SIMDx86Vector* pOut, const SIMDx86Vector* pIn, float scalar)
{
	#if defined(USE_SSE)

	asm(
	"movss %2, %%xmm0\n"
	"shufps $0x00, %%xmm0, %%xmm0\n"
	"mulps (%1), %%xmm0\n"
	"movaps %%xmm0, (%0)\n"
	:
	: "r" (pOut), "r" (pIn),  "m" (scalar)
	);

	#elif defined(USE_3DNOW)

	asm(
	"movd %2, %%mm0\n"
	"punpckldq %%mm0, %%mm0\n"
	"movq %%mm0, %%mm1\n"
	"pfmul  (%1), %%mm0\n"
	"pfmul 8(%1), %%mm1\n"
	"movq %%mm0,  (%0)\n"
	"movq %%mm1, 8(%0)\n"
	:
	: "r" (pOut), "r" (pIn), "m" (scalar)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif


	#else

	pOut->x = pIn->x * scalar;
	pOut->y = pIn->y * scalar;
	pOut->z = pIn->z * scalar;

	#endif
}

float SIMDx86Vector_AlignedDot(const SIMDx86Vector* pSrc1, const SIMDx86Vector* pSrc2)
{
	#if defined(USE_SSE)

		#if USE_SSE >= 3

		asm(

		/*
			Do so with AND

			x & 0xFFFFFFFF -> x
			y & 0xFFFFFFFF -> y
			z & 0xFFFFFFFF -> z
			w & 0x00000000 -> 0.0f

		*/

		/* Load the vectors */
		"movaps (%0), %%xmm0\n"
		"mulps (%1), %%xmm0\n"

		/* Remove w component from one with AND mask */
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"

		/* Sum components */
		"haddps %%xmm0, %%xmm0\n"
		"haddps %%xmm0, %%xmm0\n"

		"movss %%xmm0, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		:
		: "r" (pSrc1), "r" (pSrc2)
		);


		#else
		float dummy;
		asm(

		"movaps (%1), %%xmm0\n"
		"mulps (%2), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"

        "movhlps %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   |  z's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   | z's + x's */
		"shufps $0x55, %%xmm0, %%xmm0\n"/* xmm0 = y's | y's | y's | y's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | ?   | x's+y's+z's */


		"movss %%xmm1, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "r" (pSrc1), "r" (pSrc2)
		);
		return dummy;
		#endif

	#elif defined(USE_3DNOW)
	float dummy;
	asm(

	"movq  (%2), %%mm0\n"   /* mm0 = Y1 | X1*/
	"movq 8(%2), %%mm1\n"   /* mm1 = ?? | Z1 */

	"pfmul  (%1), %%mm0\n"  /* mm0 = Y1*Y2 | X1*X2 */
	"pand _SIMDx86_float_3DNOW_NO_W_MASK, %%mm1\n" /* mm1 = ?? & 0 | Z1 & 0xFFFFFFFF */
	"pfmul 8(%1), %%mm1\n"  /* mm1 =   0.0 | Z1*Z2 */

	"pfadd %%mm0, %%mm1\n"  /* mm1 = Y1*Y2 | X1*X2 + Z1*Z2 */
	"pfacc %%mm1, %%mm1\n"  /* mm0 = ? | X1*X2 + Y1*Y2 + Z1*Z2 */

	"movd %%mm1, -4(%%esp)\n"

	"femms\n"
	"flds -4(%%esp)\n"
	: "=t" (dummy)
	: "r" (pSrc1), "r" (pSrc2)
	);
	return dummy;
	#else

	return (pSrc1->x * pSrc2->x) + (pSrc1->y * pSrc2->y) + (pSrc1->z * pSrc2->z);

	#endif
}

float SIMDx86Vector_AlignedDot4(const float* pSrc4D1, const float* pSrc4D2)
{
	#if defined(USE_3DNOW)

	float dummy;
	asm(
	"movq  (%1), %%mm0\n"	/* mm0 = y1 | x1 */
	"movq 8(%1), %%mm1\n"	/* mm1 = w1 | z1 */
	"pfmul  (%2), %%mm0\n"	/* mm0 = y1*y2 | x1*x2 */
	"pfmul 8(%2), %%mm1\n"	/* mm1 = w1*w2 | z1*z2 */
	"pfadd %%mm0, %%mm1\n"	/* mm1 = y1*y2+w1*w2 | x1*x2+z1*z2 */
	"pfacc %%mm1, %%mm1\n"			/* mm1 = dot4 | dot4 */
	"movd %%mm1, -4(%%esp)\n"
	"femms\n"
	"flds -4(%%esp)\n"
	: "=t" (dummy)
	: "r" (pSrc4D1), "r" (pSrc4D2)
	);
	return dummy;

	#elif defined(USE_SSE)

	#if USE_SSE >= 3

	float dummy;
	asm(
	"movaps (%1), %%xmm0\n"
	"mulps (%2), %%xmm0\n"
	"haddps %%xmm0, %%xmm0\n"
	"haddps %%xmm0, %%xmm0\n"
	"movss %%xmm0, -4(%%esp)\n"
	"flds -4(%%esp)\n"
	: "=t" (dummy)
	: "r" (pSrc4D1), "r" (pSrc4D2)
	);
	return dummy;
	#else
	float dummy;
	asm(

	"movaps (%1), %%xmm0\n"
	"mulps (%2), %%xmm0\n"				/* xmm0 = w1*w2 | z1*z2 | y1*y2 | x1*x2 */
	"movaps %%xmm0, %%xmm1\n"			/* xmm1 = xmm0 */
	"shufps $0x1B, %%xmm1, %%xmm1\n"	/* xmm1 = x | y | z | w */
	"addps %%xmm0, %%xmm1\n"			/* xmm1 = w+x | y+z | y+z | w+x */
	"movss %%xmm1, %%xmm2\n"			/* xmm2 = ??? | ??? | ??? | w+x */
	"shufps $0x01, %%xmm1, %%xmm1\n"	/* xmm2 = ??? | ??? | ??? | y+z */
	"addss %%xmm1, %%xmm2\n"			/* xmm2 = ??? | ??? | ??? | dot4 */
	"movss %%xmm2, -4(%%esp)\n"
	"flds -4(%%esp)\n"
	: "=t" (dummy)
	: "r" (pSrc4D1), "r" (pSrc4D2)
	);
	return dummy;
	#endif

	#else
	return (pSrc4D1[0]*pSrc4D2[0]) + (pSrc4D1[1]*pSrc4D2[1]) + (pSrc4D1[2]*pSrc4D2[2]) + (pSrc4D1[3]*pSrc4D2[3]);
	#endif
}

float SIMDx86Vector_AlignedLengthSq(const SIMDx86Vector* pVec)
{
	#if defined(USE_3DNOW)
	float dummy;
	asm(
	"movq  (%1), %%mm0\n"
	"movd 8(%1), %%mm1\n"

	"pfmul %%mm0, %%mm0\n"
	"pfmul %%mm1, %%mm1\n"

	"pfacc %%mm0, %%mm0\n"
	"pfadd %%mm1, %%mm0\n"

	"movd %%mm0, -4(%%esp)\n"
	"femms\n"
	"flds -4(%%esp)\n"
	: "=t" (dummy)
	: "r" (pVec)
	);
	return dummy;
	#elif defined(USE_SSE)

		float dummy;

		#if (USE_SSE >= 3)

		asm(
		"movaps (%1), %%xmm0\n"             /* xmm0 = x | y | z | w*/
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"mulps %%xmm0, %%xmm0\n"            /* xmm0 = x*x | y*y | z*z | 0.0f */
		"haddps %%xmm0, %%xmm0\n"           
		"haddps %%xmm0, %%xmm0\n"           /* xmm0 = lensq | lensq | lensq | lensq */
            
		"movss %%xmm0, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "r" (pVec)
		);

		#else

		asm(
		"movaps (%1), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"mulps %%xmm0, %%xmm0\n"


        "movhlps %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   |  z's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   | z's + x's */
		"shufps $0x55, %%xmm0, %%xmm0\n"/* xmm0 = y's | y's | y's | y's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | ?   | x's+y's+z's */


		"movss %%xmm1, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "r" (pVec)
		);


		#endif /* USE_SSE == 1 ||  USE_SSE == 2 */

		return dummy;

	#else

	return (pVec->x*pVec->x) + (pVec->y*pVec->y) + (pVec->z*pVec->z);
	#endif
}

float SIMDx86Vector_AlignedLength(const SIMDx86Vector* pVec)
{

	#if defined(USE_3DNOW)
	float dummy;
	asm(
	"movq  (%1), %%mm0\n"
	"movd 8(%1), %%mm1\n"

	"pfmul %%mm0, %%mm0\n"
	"pfmul %%mm1, %%mm1\n"

	"pfacc %%mm0, %%mm0\n"
	"pfadd %%mm1, %%mm0\n"

	#if defined(HIPREC)
	"movq %%mm0, %%mm3\n"
	"pfrsqrt %%mm0, %%mm1\n"        /* mm1 = x0 = rsqrt(mag*mag) */
	"movq %%mm1, %%mm2\n"           /* mm2 = x0 */
	"pfmul %%mm1, %%mm1\n"          /* mm1 = x1 = x0*x0 */
	"pfrsqit1 %%mm1, %%mm0\n"       /* mm0 = x2 = pfrsqit1(val, x1)*/
	"pfrcpit2 %%mm2, %%mm0\n"       /* mm2 = 1/sqrt(val) = pfrcpit2(x2, x0)*/
	"pfmul %%mm0, %%mm3\n"
	"movd %%mm3, -4(%%esp)\n"
	"femms\n"
	"flds -4(%%esp)\n"
	#else
	"pfrsqrt %%mm0, %%mm1\n"
	"pfmul %%mm0, %%mm1\n"
	"movd %%mm1, -4(%%esp)\n"
	"femms\n"
	"flds -4(%%esp)\n"

	#endif
	
	: "=t" (dummy)
	: "r" (pVec)
	);

	return dummy;

	#elif defined(USE_SSE)

		#if USE_SSE >= 3
		float dummy;

		asm(
		"movaps (%1), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"mulps %%xmm0, %%xmm0\n"
		"haddps %%xmm0, %%xmm0\n"
		"haddps %%xmm0, %%xmm0\n"

		#ifdef HIPREC
		"sqrtss %%xmm0, %%xmm0\n"
		#else
		"rsqrtss %%xmm0, %%xmm0\n"
		"rcpss %%xmm0, %%xmm0\n"
		#endif

		"movss %%xmm0, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "r" (pVec)
		);

		return dummy;
		#else

		float dummy;
		asm(
		"movaps (%1), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"mulps %%xmm0, %%xmm0\n"

		"movhlps %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   |  z's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   | z's + x's */
		"shufps $0x55, %%xmm0, %%xmm0\n"/* xmm0 = y's | y's | y's | y's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | ?   | x's+y's+z's */


		#ifdef HIPREC
		"sqrtss %%xmm1, %%xmm1\n"
		#else
		"rsqrtss %%xmm1, %%xmm1\n"
		"rcpss %%xmm1, %%xmm1\n"
		#endif

		"movss %%xmm1, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "r" (pVec)
		);

		return dummy;
		#endif
	#else


	return sqrtf((pVec->x*pVec->x) + (pVec->y*pVec->y) + (pVec->z*pVec->z));

	#endif
}

void SIMDx86Vector_AlignedCross(SIMDx86Vector* pLeft, const SIMDx86Vector* pRight)
{
	#if defined(USE_SSE)
	asm(
	"movaps (%0), %%xmm0\n"		/* xmm0 = pLeft */
	"movaps (%1), %%xmm1\n"		/* xmm1 = pRight */
	"movaps %%xmm0, %%xmm2\n"	/* xmm2 = pLeft */
	"movaps %%xmm1, %%xmm3\n"	/* xmm3 = pRight */

	/*
		Shuffles
		w | x | z | y  == 1100 1001 = 0xC9
		w | y | x | z  == 1101 0010 = 0xD2
	*/
	"shufps $0xC9, %%xmm0, %%xmm0\n"
	"shufps $0xD2, %%xmm1, %%xmm1\n"
	"shufps $0xD2, %%xmm2, %%xmm2\n"
	"shufps $0xC9, %%xmm3, %%xmm3\n"

	"mulps %%xmm0, %%xmm1\n"
	"mulps %%xmm2, %%xmm3\n"

	"subps %%xmm3, %%xmm1\n"

	"movaps %%xmm1, (%0)\n"
	:
	: "r" (pLeft), "r" (pRight)
	);

	#elif defined(USE_3DNOW)

	asm(
	"movq 4(%1), %%mm0\n"		/* mm0 = Rz | Ry */
	"movd  (%1), %%mm1\n"		/* mm1 =  ? | Rz */
	"movd 4(%1), %%mm5\n"		/* mm5 =  ? | Ry */
	"movd 8(%1), %%mm4\n"		/* mm4 =  ? | Rz */
	"punpckldq (%1), %%mm4\n"	/* mm4 = Rx | Rz */
	"movd 4(%0), %%mm3\n"		/* mm3 =  ? | Ly */
	"movd 8(%0), %%mm2\n"		/* mm2 =  ? | Lz */
	"punpckldq (%0), %%mm2\n"	/* mm2 = Lx | Lz */
	"movq 4(%0), %%mm6\n"		/* mm6 = Lz | Ly */
	"movd  (%0), %%mm7\n"		/* mm7 =  ? | Lx */


/*
	pLeft->x = (tmp.y * pRight->z) - (tmp.z * pRight->y);
	pLeft->y = (tmp.z * pRight->x) - (tmp.x * pRight->z);
	pLeft->z = (tmp.x * pRight->y) - (tmp.y * pRight->x);
*/

	/* Multiply Column 1 & 2 */
	"pfmul %%mm4, %%mm6\n"
	"pfmul %%mm5, %%mm7\n"

	/* Multiply Column 3 & 4 */
	"pfmul %%mm0, %%mm2\n"
	"pfmul %%mm1, %%mm3\n"

	/* Sutract... (1*2) - (3*4) */
	"pfsub %%mm2, %%mm6\n"
	"pfsub %%mm3, %%mm7\n"

	/* And store */
	"movq %%mm6,  (%0)\n"
	"movd %%mm7, 8(%0)\n"
	:
	: "r" (pLeft), "r" (pRight)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif

	#else

	SIMDx86Vector tmp;

	tmp.x = pLeft->x;
	tmp.y = pLeft->y;
	tmp.z = pLeft->z;

	pLeft->x = (tmp.y * pRight->z) - (tmp.z * pRight->y);
	pLeft->y = (tmp.z * pRight->x) - (tmp.x * pRight->z);
	pLeft->z = (tmp.x * pRight->y) - (tmp.y * pRight->x);

	#endif
}

void SIMDx86Vector_AlignedCrossOf(SIMDx86Vector* pOut, const SIMDx86Vector* pLeft, const SIMDx86Vector* pRight)
{
	#if defined(USE_SSE)

	asm(
	"movaps (%1), %%xmm0\n"		/* xmm0 = pLeft */
	"movaps (%2), %%xmm1\n"		/* xmm1 = pRight */
	"movaps %%xmm0, %%xmm2\n"	/* xmm2 = pLeft */
	"movaps %%xmm1, %%xmm3\n"	/* xmm3 = pRight */

	/*
		Shuffles
		w | x | z | y  == 1100 1001 = 0xC9
		w | y | x | z  == 1101 0010 = 0xD2
	*/
	"shufps $0xC9, %%xmm0, %%xmm0\n"
	"shufps $0xD2, %%xmm1, %%xmm1\n"
	"shufps $0xD2, %%xmm2, %%xmm2\n"
	"shufps $0xC9, %%xmm3, %%xmm3\n"

    "mulps %%xmm0, %%xmm1\n"
	"mulps %%xmm2, %%xmm3\n"

        
	"subps %%xmm3, %%xmm1\n"
        
	"movaps %%xmm1, (%0)\n"
	:
	: "r" (pOut), "r" (pLeft), "r" (pRight)
	);

	#elif defined(USE_3DNOW)


    
	asm(
	"movq 4(%2), %%mm0\n"		/* mm0 = Rz | Ry */
	"movd  (%2), %%mm1\n"		/* mm1 =  ? | Rz */
	"movd 4(%2), %%mm5\n"		/* mm5 =  ? | Ry */
	"movd 8(%2), %%mm4\n"		/* mm4 =  ? | Rz */
	"punpckldq (%2), %%mm4\n"	/* mm4 = Rx | Rz */
	"movd 4(%1), %%mm3\n"		/* mm3 =  ? | Ly */
	"movd 8(%1), %%mm2\n"		/* mm2 =  ? | Lz */
	"punpckldq (%1), %%mm2\n"	/* mm2 = Lx | Lz */
	"movq 4(%1), %%mm6\n"		/* mm6 = Lz | Ly */
	"movd  (%1), %%mm7\n"		/* mm7 =  ? | Lx */


/*
		pOut->x = (pLeft->y * pRight->z) - (pLeft->z * pRight->y);
		pOut->y = (pLeft->z * pRight->x) - (pLeft->x * pRight->z);
		pOut->z = (pLeft->x * pRight->y) - (pLeft->y * pRight->x);
*/
	
	"pfmul %%mm4, %%mm6\n"	/* mm4 = Lz*Rx | Ly*Rz */
	"pfmul %%mm5, %%mm7\n"	/* mm5 = ????? | Lx*Ry */


	"pfmul %%mm0, %%mm2\n"
	"pfmul %%mm1, %%mm3\n"


	"pfsub %%mm2, %%mm6\n"
	"pfsub %%mm3, %%mm7\n"


	"movq %%mm6,  (%0)\n"
	"movd %%mm7, 8(%0)\n"
	:
	: "r" (pOut), "r" (pLeft), "r" (pRight)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif

	#else

	pOut->x = (pLeft->y * pRight->z) - (pLeft->z * pRight->y);
	pOut->y = (pLeft->z * pRight->x) - (pLeft->x * pRight->z);
	pOut->z = (pLeft->x * pRight->y) - (pLeft->y * pRight->x);

	#endif
}
void SIMDx86Vector_AlignedNormalize(SIMDx86Vector* pVec)
{
	#if defined(USE_SSE)

		#if USE_SSE >= 3

		asm(
		"movaps (%0), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"movaps %%xmm0, %%xmm2\n"
		"mulps %%xmm0, %%xmm0\n"
		"haddps %%xmm0, %%xmm0\n"
		"haddps %%xmm0, %%xmm0\n"


		#ifdef HIPREC

			"sqrtps %%xmm0, %%xmm0\n"				/* xmm0 = mag(pVec) */
			"divps %%xmm0, %%xmm2\n"				/* xmm2 = ? | z / mag | y / mag | x / mag */

		#else

			"rsqrtps %%xmm0, %%xmm1\n"				/* xmm1 = rcp(pVec) | rcp(pVec) | rcp(pVec) | rcp(pVec) */
			"mulps %%xmm1, %%xmm2\n"				/* xmm2 = norm(pVecv) */

		#endif
		"movaps %%xmm2, (%0)\n"

		:
		: "r" (pVec)
		);
		#else

		asm(
		"movaps (%0), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"movaps %%xmm0, %%xmm7\n"			/* Save for the division by length */
		"mulps %%xmm0, %%xmm0\n"


		"movhlps %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   |  z's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   | z's + x's */
		"shufps $0x55, %%xmm0, %%xmm0\n"/* xmm0 = y's | y's | y's | y's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | ?   | x's+y's+z's */

		#ifdef HIPREC

			"sqrtss %%xmm1, %%xmm1\n"			/* xmm3 = ? | ? | ? | mag(pVec) */
			"shufps $0x00, %%xmm1, %%xmm1\n"	/* xmm3 = mag(pVec) | mag(pVec) | mag(pVec) | mag(pVec) */
			"divps %%xmm1, %%xmm7\n"			/* xmm1 = ? | z/mag | y/mag | x/mag */

		#else

			"rsqrtss %%xmm1 ,%%xmm1\n"			/* xmm3 = ? | ? | ? | rcp(mag) */
			"shufps $0x00, %%xmm1, %%xmm1\n"	/* xmm3 = rcp(mag) | rcp(mag) | rcp(mag) | rcp(mag) */
			"mulps %%xmm1, %%xmm7\n"			/* xmm1 = ? | z/mag | y/mag | x/mag */

		#endif

		"movaps %%xmm7, (%0)\n"
				:
		: "r" (pVec)
		);


		#endif

	#elif defined(USE_3DNOW)

	asm (

	"movq  (%0), %%mm0\n"
	"movq 8(%0), %%mm1\n"
	"pand _SIMDx86_float_3DNOW_NO_W_MASK, %%mm1\n"    /* mm1 = 0.0 | z */

	"movq %%mm0, %%mm6\n"
	"movq %%mm1, %%mm7\n"

	"pfmul %%mm0, %%mm0\n"	/* y*y | x*x */
	"pfmul %%mm1, %%mm1\n"	/* 0   | z*z */

	"pfadd %%mm0, %%mm1\n"	/* 0+y*y | z*z+x*x */
	"pfacc %%mm1, %%mm1\n"	/* mag*mag | mag*mag */

	#ifdef HIPREC
         
	"movq %%mm1, %%mm3\n"
	"pfrsqrt %%mm1, %%mm0\n"        /* mm1 = x0 = rsqrt(mag*mag) */
	"movq %%mm0, %%mm2\n"           /* mm2 = x0 */
	"pfmul %%mm0, %%mm0\n"          /* mm1 = x1 = x0*x0 */
	"pfrsqit1 %%mm0, %%mm1\n"       /* mm0 = x2 = pfrsqit1(val, x1)*/
	"pfrcpit2 %%mm2, %%mm1\n"       /* mm2 = 1/sqrt(val) = pfrcpit2(x2, x0)*/
	"pfmul %%mm1, %%mm6\n"
	"pfmul %%mm1, %%mm7\n"

	#else

	"pfrsqrt %%mm1, %%mm1\n"	/* mm1 = rcp(mag) | rcp(mag) (approx) */
	"pfmul %%mm1, %%mm6\n"		/* mm2 = y/mag | x/mag */
	"pfmul %%mm1, %%mm7\n"		/* mm3 = 0 | z/mag */
	#endif

	"movq %%mm6,  (%0)\n"
	"movq %%mm7, 8(%0)\n"
	:
	: "r" (pVec)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif

	#else

	float inv_mag = 1.0f / (sqrtf((pVec->x*pVec->x) + (pVec->y*pVec->y) + (pVec->z*pVec->z))+FLT_EPSILON);

	pVec->x *= inv_mag;
	pVec->y *= inv_mag;
	pVec->z *= inv_mag;
	#endif
}
void SIMDx86Vector_AlignedNormalizeOf(SIMDx86Vector* pOut, const SIMDx86Vector* pVec)
{
	#if defined(USE_SSE)

		#if USE_SSE >= 3

		asm(
		"movaps (%0), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"movaps %%xmm0, %%xmm2\n"
		"mulps %%xmm0, %%xmm0\n"
		"haddps %%xmm0, %%xmm0\n"
		"haddps %%xmm0, %%xmm0\n"

		#ifdef HIPREC

			"sqrtps %%xmm0, %%xmm0\n"				/* xmm0 = mag(pVec) */
			"divps %%xmm0, %%xmm2\n"				/* xmm2 = ? | z / mag | y / mag | x / mag */

		#else

			"rsqrtps %%xmm0, %%xmm1\n"				/* xmm1 = rcp(pVec) | rcp(pVec) | rcp(pVec) | rcp(pVec) */
			"mulps %%xmm1, %%xmm2\n"				/* xmm2 = norm(pVecv) */

		#endif

		"movaps %%xmm2, (%1)\n"

		:
		: "r" (pVec), "r" (pOut)
		);
		#else
		asm(
		"movaps (%0), %%xmm0\n"
		"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"    /* xmm0 = x | y | z | 0.0f */
		"movaps %%xmm0, %%xmm7\n"			/* Save for the division by length */
		"mulps %%xmm0, %%xmm0\n"

		"movhlps %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   |  z's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | 0   | z's + x's */
		"shufps $0x55, %%xmm0, %%xmm0\n"/* xmm0 = y's | y's | y's | y's */
		"addss %%xmm0, %%xmm1\n"		/* xmm1 = ?   | ?   | ?   | x's+y's+z's */

		#ifdef HIPREC

			"sqrtss %%xmm1, %%xmm1\n"			/* xmm3 = ? | ? | ? | mag(pVec) */
			"shufps $0x00, %%xmm1, %%xmm1\n"	/* xmm3 = mag(pVec) | mag(pVec) | mag(pVec) | mag(pVec) */
			"divps %%xmm1, %%xmm7\n"			/* xmm1 = ? | z/mag | y/mag | x/mag */

		#else

			"rsqrtss %%xmm1 ,%%xmm1\n"			/* xmm3 = ? | ? | ? | rcp(mag) */
			"shufps $0x00, %%xmm1, %%xmm1\n"	/* xmm3 = rcp(mag) | rcp(mag) | rcp(mag) | rcp(mag) */
			"mulps %%xmm1, %%xmm7\n"			/* xmm1 = ? | z/mag | y/mag | x/mag */

		#endif
		"movaps %%xmm7, (%1)\n"
				:
		: "r" (pVec), "r" (pOut)
		);

	#endif /* USE_SSE == 1 || USE_SSE == 2 */

	#elif defined(USE_3DNOW)

	asm (

	
	"movq  (%0), %%mm0\n"
	"movq 8(%0), %%mm1\n"
	"pand _SIMDx86_float_3DNOW_NO_W_MASK, %%mm1\n"    /* mm1 = 0.0 | z */
	"movq %%mm0, %%mm6\n"
	"movq %%mm1, %%mm7\n"

	"pfmul %%mm0, %%mm0\n"	/* y*y | x*x */
	"pfmul %%mm1, %%mm1\n"	/* 0   | z*z */


	"pfadd %%mm0, %%mm1\n"	/* 0+y*y | z*z+x*x */
	"pfacc %%mm1, %%mm1\n"	/* mag*mag | mag*mag */

	#ifdef HIPREC

	"movq %%mm1, %%mm3\n"
	"pfrsqrt %%mm1, %%mm0\n"        /* mm1 = x0 = rsqrt(mag*mag) */
	"movq %%mm0, %%mm2\n"           /* mm2 = x0 */
	"pfmul %%mm0, %%mm0\n"          /* mm1 = x1 = x0*x0 */
	"pfrsqit1 %%mm0, %%mm1\n"       /* mm0 = x2 = pfrsqit1(val, x1)*/
	"pfrcpit2 %%mm2, %%mm1\n"       /* mm2 = 1/sqrt(val) = pfrcpit2(x2, x0)*/
	"pfmul %%mm1, %%mm6\n"
	"pfmul %%mm1, %%mm7\n"

	#else

	"pfrsqrt %%mm1, %%mm1\n"	/* mm1 = rcp(mag) | rcp(mag) (approx) */
	"pfmul %%mm1, %%mm6\n"		/* mm2 = y/mag | x/mag */
	"pfmul %%mm1, %%mm7\n"		/* mm3 = 0 | z/mag */
	#endif

	"movq %%mm6,  (%1)\n"
	"movq %%mm7, 8(%1)\n"
	:
	: "r" (pVec), "r" (pOut)
	);
	#ifndef NO_EMMS
	asm("femms\n");
	#endif


	#else
	float inv_mag = 1.0f / (sqrtf((pVec->x*pVec->x) + (pVec->y*pVec->y) + (pVec->z*pVec->z))+FLT_EPSILON);

	pOut->x = pVec->x * inv_mag;
	pOut->y = pVec->y * inv_mag;
	pOut->z = pVec->z * inv_mag;
	#endif
}

float SIMDx86Vector_AlignedDistance(const SIMDx86Vector* pVec1, const SIMDx86Vector* pVec2)
{
#if 0
	#if defined(USE_SSE)

	#if USE_SSE == 3
		float dummy;
		asm(
			"movaps (%1), %%xmm0\n"
			"subps (%2), %%xmm0\n"
			"andps _SIMDx86_float_SSE_NO_W_MASK, %%xmm0\n"
			"haddps %%xmm0, %%xmm0\n"
			"haddps %%xmm0, %%xmm0\n"
			"subl $4, %%esp\n"
			"movss %%xmm0, (%%esp)\n"
			"flds (%%esp)\n"
			"addl $4, %%esp\n"
			: "=t" (dummy)
			: "r" (pVec1), "r" (pVec2)
			);
		return dummy;
	#else
	#endif
	#endif

#endif

	SIMDx86Vector diff;
	SIMDx86Vector_DiffOf(&diff, pVec1, pVec2);
	return SIMDx86Vector_Length(&diff);
	
}

