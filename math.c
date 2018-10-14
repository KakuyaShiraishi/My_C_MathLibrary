#include <SIMDx86/consts.h>

#include <math.h>

float SIMDx86_sqrtf(float value)
{
	#if defined(USE_SSE)

		float dummy;

		#ifdef HIPREC

		asm (
		"movss %1, %%xmm0\n"
		"sqrtss %%xmm0, %%xmm0\n"
		"movss %%xmm0, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);
		#else

		asm (
		"movss %1, %%xmm0\n"
		"rsqrtss %%xmm0, %%xmm0\n"
		"rcpss %%xmm0, %%xmm0\n"
		"movss %%xmm0, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);

		#endif

		return dummy;

	#elif defined(USE_3DNOW)

		float dummy;

		#ifdef 	HIPREC

        asm(
		"movd %1, %%mm0\n"		/* mm0 = ? | value */
		"pfrsqrt %%mm0, %%mm1\n"	/* mm1 = 1.0f / sqrt(value) | 1.0f / sqrt(value) (approx) */
		"movq %%mm1, %%mm2\n"		/* mm2 = mm1 = x0 */
		"pfmul %%mm1, %%mm1\n"		/* mm1 = x0*x0 | x0*x0 */
		"punpckldq %%mm0, %%mm0\n"	/* mm0 = value | value */
		"pfrsqit1 %%mm0, %%mm1\n"	/* mm1 = intermediate value (Newton-Raphson) */
		"pfrcpit2 %%mm1, %%mm2\n"	/* mm2 = 1.0 / sqrt(value) (exact) */
		"pfmul %%mm2, %%mm0\n"		/* mm0 = sqrt(value) */
		"movd %%mm0, -4(%%esp)\n"
		"femms\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);

		#else

		asm(
		"movd %1, %%mm0\n"		/* mm0 = value */
		"pfrsqrt %%mm0, %%mm1\n"	/* mm1 = 1.0f / sqrt(value) (approx) */
		"pfmul %%mm0, %%mm1\n"		/* mm1 = sqrtf(value) (approx) */
		"movd %%mm0, -4(%%esp)\n"
		"femms\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);

		#endif

		return dummy;

	#else
		return sqrtf(value);
	#endif

}

float SIMDx86_rsqrtf(float value)
{
	#if defined(USE_SSE)

		float dummy;

		#ifdef HIPREC

		asm(
		"movss %1, %%xmm0\n"
		"movss _SIMDx86_float_one, %%xmm1\n"
		"sqrtss %%xmm0, %%xmm0\n"
		"divss %%xmm0, %%xmm1\n"
		"movss %%xmm1, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);

		#else
		asm(
		"movss %1, %%xmm0\n"
		"rsqrtss %%xmm0, %%xmm0\n"
		"movss %%xmm0, -4(%%esp)\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);
		#endif

		return dummy;


	#elif defined(USE_3DNOW)

		float dummy;

		#ifdef HIPREC
        asm(
		"movd %1, %%mm0\n"		/* mm0 = ? | value */
		"pfrsqrt %%mm0, %%mm1\n"	/* mm1 = 1.0f / sqrt(value) | 1.0f / sqrt(value) (approx) */
		"movq %%mm1, %%mm2\n"		/* mm2 = mm1 = x0 */
		"pfmul %%mm1, %%mm1\n"		/* mm1 = x0*x0 | x0*x0 */
		"punpckldq %%mm0, %%mm0\n"	/* mm0 = value | value */
		"pfrsqit1 %%mm0, %%mm1\n"	/* mm1 = intermediate value (Newton-Raphson) */
		"pfrcpit2 %%mm2, %%mm2\n"	/* mm2 = 1.0 / sqrt(value) (exact) */
		"movd %%mm2, -4(%%esp)\n"
		"femms\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);

		#else

		asm(
		"movd %1, %%mm0\n"		/* mm0 = ? | value */
		"pfrsqrt %%mm0, %%mm0\n"	/* mm1 = 1.0f / sqrt(value) | 1.0f / sqrt(value) (approx) */
		"movd %%mm0, -4(%%esp)\n"
		"femms\n"
		"flds -4(%%esp)\n"
		: "=t" (dummy)
		: "m" (value)
		);

		#endif


		return dummy;

	#else
		return 1.0f / sqrtf(value);
	#endif
}


double SIMDx86_sqrt(double value)
{
	#if defined(USE_SSE) && (USE_SSE >= 2)

	double dummy;
	asm(
	"movsd %1, %%xmm0\n"
	"sqrtsd %%xmm0, %%xmm0\n"
	"movsd %%xmm0, -8(%%esp)\n"
	"fldl -8(%%esp)\n"
	: "=t" (dummy)
	: "m" (value)
	);
	return dummy;

	#else
	return sqrt(value);
	#endif

}

double SIMDx86_rsqrt(double value)
{
	#if defined(USE_SSE) && (USE_SSE >= 2)

	double dummy;
	asm(
	"movsd %1, %%xmm0\n"		/* xmm0 = value*/
	"movsd _SIMDx86_double_one, %%xmm1\n"		/* xmm1 = 1.0 */
	"sqrtsd %%xmm0, %%xmm0\n"	/* xmm0 = sqrt(value) */
	"divsd %%xmm0, %%xmm1\n"	/* xmm1 = 1.0 / sqrt(value) */
	"movsd %%xmm0, -8(%%esp)\n"
	"fldl -8(%%esp)\n"
	: "=t" (dummy) 
	: "m" (value)
	);
	return dummy;

	#else

	return 1.0 / sqrt(value);
	#endif
}
