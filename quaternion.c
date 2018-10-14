#include <SIMDx86/quaternion.h>
#include <SIMDx86/consts.h>
#include <float.h> 
#include <math.h> /* sinf(), cosf(), sqrtf(), acosf() */

#if defined(WIN32)
	unsigned int ALIGNED POSNEGPOSNEG[4] = { 0x00000000, 0x80000000, 0x00000000, 0x80000000 };
	unsigned int ALIGNED POSPOSNEGNEG[4] = { 0x00000000, 0x00000000, 0x80000000, 0x80000000 };
	unsigned int ALIGNED NEGPOSPOSNEG[4] = { 0x80000000, 0x00000000, 0x00000000, 0x80000000 };
#else
	unsigned int ALIGNED _POSNEGPOSNEG[4] = { 0x00000000, 0x80000000, 0x00000000, 0x80000000 };
	unsigned int ALIGNED _POSPOSNEGNEG[4] = { 0x00000000, 0x00000000, 0x80000000, 0x80000000 };
	unsigned int ALIGNED _NEGPOSPOSNEG[4] = { 0x80000000, 0x00000000, 0x00000000, 0x80000000 };
#endif


void SIMDx86Quaternion_Normalize(SIMDx86Quaternion* pQuat)
{
	#if defined(USE_SSE)

	#if (USE_SSE >= 3)
		asm(
		"movups (%0), %%xmm0\n"			/* xmm0 = w | z | y | x  */
		"movaps %%xmm0, %%xmm1\n"		/* xmm1 = xmm0 */
		"mulps %%xmm0, %%xmm0\n"		/* xmm0 = w*w | z*z | y*y | x*x */
		"haddps %%xmm0, %%xmm0\n"		/* xmm0 = */
		"haddps %%xmm0, %%xmm0\n"		/* xmm0 = magsq | magsq | magsq | magsq */
		"rsqrtps %%xmm0, %%xmm0\n"		/* xmm0 = rcp(mag) | rcp(mag) | rcp(mag) | rcp(mag) */
		"mulps %%xmm0, %%xmm1\n"
		"movups %%xmm1, (%0)\n"
		:
		: "r" (pQuat)
		);
	#else

		asm(
		"movups (%0), %%xmm0\n"		/* xmm0 = w | z | y | x  */
		"movaps %%xmm0, %%xmm1\n"	/* xmm1 = w | z | y | x */
		"mulps %%xmm0, %%xmm0\n"	/* xmm0 = w*w | z*z | y*y | x*x */
		"movhlps %%xmm0, %%xmm2\n"	/* xmm2 = ? | ? | w*w | z*z */
		"addps %%xmm0, %%xmm2\n"	/* xmm2 = ? | ? | w*w+y*y | z*z+x*x */
		"movss %%xmm2, %%xmm0\n"	/* xmm0 = ? | ? | ? | z*z+x*x */
		"shufps $0x55, %%xmm2, %%xmm2\n"	/* xmm2 = w*w+y*y |  w*w+y*y |  w*w+y*y | w*w+y*y */
		"addss %%xmm0, %%xmm2\n"
		#ifdef HIPREC
		"sqrtss %%xmm2, %%xmm2\n"
		"shufps $0x00, %%xmm2, %%xmm2\n"
		"divps %%xmm2, %%xmm1\n"
		"movups %%xmm1, (%0)\n"
		#else
		"rsqrtss %%xmm2, %%xmm2\n"
		"shufps $0x00, %%xmm2, %%xmm2\n"
		"mulps %%xmm2, %%xmm1\n"
		"movups %%xmm1, (%0)\n"
		#endif

		:
		: "r" (pQuat)
		);
	#endif


	#elif defined(USE_3DNOW)

	asm(
	"movq  (%0), %%mm0\n"		/* mm0 = y | x */
	"movq 8(%0), %%mm1\n"		/* mm1 = w | z */
	"movq %%mm0, %%mm2\n"		/* mm2 = y | x */
	"movq %%mm1, %%mm3\n"		/* mm3 = w | z */
	"pfmul %%mm0, %%mm0\n"		/* y*y | x*x */
	"pfmul %%mm1, %%mm1\n"		/* w*w | z*z */
	"pfadd %%mm1, %%mm0\n"		/* y*y + w*w | z*z + x*x */
	"pfacc %%mm0, %%mm0\n"		/* magsq | magsq */
	"pfrsqrt %%mm0, %%mm1\n"	/* mm1 = rcp(mag) | rcp(mag) (approx) */

	#ifdef HIPREC
	"movq %%mm1, %%mm4\n"		/* mm4 = mm1 = x0 = rcp(mag) */
	"pfmul %%mm1, %%mm1\n"		/* mm1 = x1 = x0*x0 */
	"pfrsqit1 %%mm1, %%mm0\n"	/* mm1 = x2 = pfrsqit1(val, x1) */
	"pfrcpit2 %%mm4, %%mm0\n"	/* mm2 = 1/sqrt(mag*mag) = pfrcpit2(x2, x0)*/
	"pfmul %%mm0, %%mm2\n"
	"pfmul %%mm0, %%mm3\n"

	#else
	"pfmul %%mm1, %%mm2\n"
	"pfmul %%mm1, %%mm3\n"
	#endif
	"movq %%mm2,  (%0)\n"
	"movq %%mm3, 8(%0)\n"
	:
	: "r" (pQuat)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif

	#else

	float inv_len = 1.0f / (sqrtf((pQuat->x*pQuat->x) + (pQuat->y*pQuat->y) + (pQuat->z*pQuat->z) + (pQuat->w*pQuat->w)) + FLT_EPSILON);

	pQuat->x *= inv_len;
	pQuat->y *= inv_len;
	pQuat->z *= inv_len;
	pQuat->w *= inv_len;
	#endif
}

void SIMDx86Quaternion_NormalizeOf(SIMDx86Quaternion* pOut, const SIMDx86Quaternion* pQuat)
{
	#if defined(USE_SSE)

	#if (USE_SSE >= 3)
		asm(
		"movups (%0), %%xmm0\n"			/* xmm0 = w | z | y | x  */
		"movaps %%xmm0, %%xmm1\n"		/* xmm1 = xmm0 */
		"mulps %%xmm0, %%xmm0\n"		/* xmm0 = w*w | z*z | y*y | x*x */
		"haddps %%xmm0, %%xmm0\n"		/* xmm0 = */
		"haddps %%xmm0, %%xmm0\n"		/* xmm0 = magsq | magsq | magsq | magsq */
		"rsqrtps %%xmm0, %%xmm0\n"		/* xmm0 = rcp(mag) | rcp(mag) | rcp(mag) | rcp(mag) */
		"mulps %%xmm0, %%xmm1\n"
		"movups %%xmm1, (%1)\n"
		:
		: "r" (pQuat), "r" (pOut)
		);
	#else
		asm(
		"movups (%0), %%xmm0\n"		/* xmm0 = w | z | y | x  */
		"movaps %%xmm0, %%xmm1\n"	/* xmm1 = w | z | y | x */
		"mulps %%xmm0, %%xmm0\n"	/* xmm0 = w*w | z*z | y*y | x*x */
		"movhlps %%xmm0, %%xmm2\n"	/* xmm2 = ? | ? | w*w | z*z */
		"addps %%xmm0, %%xmm2\n"	/* xmm2 = ? | ? | w*w+y*y | z*z+x*x */
		"movss %%xmm2, %%xmm0\n"	/* xmm0 = ? | ? | ? | z*z+x*x */
		"shufps $0x55, %%xmm2, %%xmm2\n"	/* xmm2 = w*w+y*y |  w*w+y*y |  w*w+y*y | w*w+y*y */
		"addss %%xmm0, %%xmm2\n"
		#ifdef HIPREC
		
		"sqrtss %%xmm2, %%xmm2\n"			/* xmm2 = ??? | ??? | ??? | mag */
		"shufps $0x00, %%xmm2, %%xmm2\n"	/* xmm2 = mag | mag | mag | mag */
		"divps %%xmm2, %%xmm1\n"			/* xmm1 = w/mag | z/mag | y/mag | x/mag */
		"movups %%xmm1, (%1)\n"
		#else
		"rsqrtss %%xmm2, %%xmm2\n"
		"shufps $0x00, %%xmm2, %%xmm2\n"
		"mulps %%xmm2, %%xmm1\n"
		"movups %%xmm1, (%1)\n"
		#endif

		:
		: "r" (pQuat), "r" (pOut)
		);
	#endif


	#elif defined(USE_3DNOW)

	asm(
	"movq  (%0), %%mm0\n"		/* mm0 = y | x */
	"movq 8(%0), %%mm1\n"		/* mm1 = w | z */
	"movq %%mm0, %%mm2\n"		/* mm2 = y | x */
	"movq %%mm1, %%mm3\n"		/* mm3 = w | z */
	"pfmul %%mm0, %%mm0\n"		/* y*y | x*x */
	"pfmul %%mm1, %%mm1\n"		/* w*w | z*z */
	"pfadd %%mm1, %%mm0\n"		/* y*y + w*w | z*z + x*x */
	"pfacc %%mm0, %%mm0\n"		/* magsq | magsq */
	"pfrsqrt %%mm0, %%mm1\n"	/* mm1 = rcp(mag) | rcp(mag) (approx) */

	#ifdef HIPREC
	"movq %%mm1, %%mm4\n"		/* mm4 = mm1 = x0 = rcp(mag) */
	"pfmul %%mm1, %%mm1\n"		/* mm1 = x1 = x0*x0 */
	"pfrsqit1 %%mm1, %%mm0\n"	/* mm1 = x2 = pfrsqit1(val, x1) */
	"pfrcpit2 %%mm4, %%mm0\n"	/* mm2 = 1/sqrt(mag*mag) = pfrcpit2(x2, x0)*/
	"pfmul %%mm0, %%mm2\n"
	"pfmul %%mm0, %%mm3\n"

	#else
	"pfmul %%mm1, %%mm2\n"
	"pfmul %%mm1, %%mm3\n"
	#endif
	"movq %%mm2,  (%1)\n"
	"movq %%mm3, 8(%1)\n"
	:
	: "r" (pQuat), "r" (pOut)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif

	#else

	float inv_len = 1.0f / (sqrtf((pQuat->x*pQuat->x) + (pQuat->y*pQuat->y) + (pQuat->z*pQuat->z) + (pQuat->w*pQuat->w)) + FLT_EPSILON);

	pOut->x = pQuat->x * inv_len;
	pOut->y = pQuat->y * inv_len;
	pOut->z = pQuat->z * inv_len;
	pOut->w = pQuat->w * inv_len;

	#endif

}

float SIMDx86Quaternion_Length(const SIMDx86Quaternion* pQuat)
{

	return sqrtf((pQuat->x*pQuat->x) + (pQuat->y*pQuat->y) + (pQuat->z*pQuat->z) + (pQuat->w*pQuat->w));
}

float SIMDx86Quaternion_LengthSq(const SIMDx86Quaternion* pQuat)
{

	return (pQuat->x*pQuat->x) + (pQuat->y*pQuat->y) + (pQuat->z*pQuat->z) + (pQuat->w*pQuat->w);
}

void SIMDx86Quaternion_Multiply(SIMDx86Quaternion* pLeft, const SIMDx86Quaternion* pRight)
{

	#if defined(USE_3DNOW) && (USE_3DNOW >= 2)
	asm(
	"pswapd  (%0), %%mm1\n"		/* mm1 = Rx | Ry */
	"pswapd 8(%0), %%mm0\n"		/* mm0 = Rz | Rw */

	"movq  (%1), %%mm2\n"		/* mm2 = Ly | Lx */
	"movq 8(%1), %%mm7\n"		/* mm7 = Lw | Lz */

    "pswapd %%mm0, %%mm4\n"		/* mm4 = Rw | Rz */
	"pswapd %%mm1, %%mm5\n"		/* mm5 = Ry | Rx */

    "movq %%mm0, %%mm6\n"

	"movq %%mm2, %%mm3\n"		/* mm3 = mm2 = Ly | Lx */
	"punpckldq %%mm2, %%mm2\n"	/* mm2 = Lx | Lx */
	"punpckhdq %%mm3, %%mm3\n"	/* mm3 = Ly | Ly */

	"pfmul %%mm2, %%mm0\n"
	"pfmul %%mm2, %%mm1\n"


	"pxor _SIMDx86_float_POSNEG, %%mm0\n"
	"pxor _SIMDx86_float_POSNEG, %%mm1\n"

	"pswapd %%mm5, %%mm2\n"


	"pfmul %%mm3, %%mm4\n"
	"pfmul %%mm3, %%mm5\n"

	"pfadd %%mm4, %%mm0\n"
	"pfsub %%mm5, %%mm1\n"

	/*
		========================
		mm0 = C2+C3
		mm1 = C2+C3
		mm2 = Rx | Ry
		mm3 = free
		mm4 = free
		mm5 = free
		mm6 = Rz | Rw
		mm7 = Lw | Lz

		-------------------
		Column 4 = mm2 | mm6
		
		--------------
		Column 1 = mm3{swap(mm2)} | mm4{swap(mm6)}
		mm5 = Lz | Lz
		mm7 = Lw | Lw

	*/

	"pswapd %%mm2, %%mm3\n"		/* mm3 = Ry | Rx */
	"movq %%mm7, %%mm5\n"		/* mm5 = Lw | Lz */
	"pswapd %%mm6, %%mm4\n"		/* mm4 = Rw | Rz */
	"punpckhdq %%mm7, %%mm7\n"	/* mm7 = Lw | Lw */
	"punpckldq %%mm5, %%mm5\n"	/* mm5 = Lz | Lz */


	"pfmul %%mm7, %%mm3\n"
	"pfmul %%mm7, %%mm4\n"

	"pfmul %%mm5, %%mm2\n"
	"pfmul %%mm5, %%mm6\n"

	"pfadd %%mm3, %%mm0\n"
	"pfadd %%mm4, %%mm1\n"

	"pxor _SIMDx86_float_NEGPOS, %%mm2\n"
	"pxor _SIMDx86_float_POSNEG, %%mm6\n"

    "pfadd %%mm2, %%mm0\n"
	"pfadd %%mm6, %%mm1\n"

    "movq %%mm0,  (%1)\n"
	"movq %%mm1, 8(%1)\n"


	:
	: "r" (pRight), "r" (pLeft)
	);

	#ifndef NO_EMMS
	asm("femms\n");
	#endif

	#elif defined(USE_SSE)
	asm(
	"movups (%0), %%xmm0\n" 	/* xmm0 = Rw | Rz | Ry | Rx */
	"movups (%1), %%xmm4\n" 	/* xmm1 = Lw | Lz | Ly | Lx */


	"movaps %%xmm0, %%xmm1\n"	/* xmm1 = xmm0 */
	"movaps %%xmm0, %%xmm2\n"	/* xmm2 = xmm0 */
	"movaps %%xmm0, %%xmm3\n"	/* xmm3 = xmm0 */

	"movaps %%xmm4, %%xmm5\n"	/* xmm5 = xmm4 */
	"movaps %%xmm4, %%xmm6\n"	/* xmm6 = xmm4 */
	"movaps %%xmm4, %%xmm7\n"	/* xmm7 = xmm4 */

	/*
        0000 0000 = xxxx = 0x00
	    0101 0101 = yyyy = 0x55
		1010 1010 = zzzz = 0xAA
		1111 1111 = wwww = 0xFF
	*/
	"shufps $0x00, %%xmm4, %%xmm4\n"		/* xmm4 = Rx | Rx | Rx | Rx */
	"shufps $0x55, %%xmm5, %%xmm5\n"		/* xmm5 = Ry | Ry | Ry | Ry */
	"shufps $0xAA, %%xmm6, %%xmm6\n"		/* xmm6 = Rz | Rz | Rz | Rz */
	"shufps $0xFF, %%xmm7, %%xmm7\n"		/* xmm7 = Rw | Rw | Rw | Rw */

	/*
		C1 = w | z | y | x = 1110 0100 =
		C2 = x | y | z | w = 0001 1011 = 0x1B
		C3 = y | x | w | z = 0100 1110 = 0x4E
		C4 = z | w | x | y = 1011 0001 = 0xB1
	*/

	"shufps $0x1B, %%xmm1, %%xmm1\n"
	"shufps $0x4E, %%xmm2, %%xmm2\n"
	"shufps $0xB1, %%xmm3, %%xmm3\n"

    "mulps %%xmm0, %%xmm7\n"		/* C1 *= Lw */
	"mulps %%xmm1, %%xmm4\n"		/* C2 *= Lx */
	"mulps %%xmm2, %%xmm5\n"		/* C3 *= Ly */
	"mulps %%xmm3, %%xmm6\n"		/* C4 *= Lz */

	"xorps _POSNEGPOSNEG, %%xmm4\n"		/* C2 = { + - + - } */
	"xorps _POSPOSNEGNEG, %%xmm5\n"		/* C3 = { + + - - } */
	"xorps _NEGPOSPOSNEG, %%xmm6\n"		/* C4 = { - + + - } */

	"addps %%xmm4, %%xmm5\n"		/* C2 += C1 */
	"addps %%xmm6, %%xmm7\n"		/* C4 += C3 */
	"addps %%xmm5, %%xmm7\n"		/* C4 += C2 */

	"movups %%xmm7, (%1)\n"
	:
	: "r" (pRight), "r" (pLeft)
	);


	#else

	float quat[4];

	quat[0] = pLeft->x;
	quat[1] = pLeft->y;
	quat[2] = pLeft->z;
	quat[3] = pLeft->w;

	pLeft->x = (quat[3] * pRight->x) + (quat[0] * pRight->w) + (quat[1] * pRight->z) - (quat[2] * pRight->y);
	pLeft->y = (quat[3] * pRight->y) - (quat[0] * pRight->z) + (quat[1] * pRight->w) + (quat[2] * pRight->x);
	pLeft->z = (quat[3] * pRight->z) + (quat[0] * pRight->y) - (quat[1] * pRight->x) + (quat[2] * pRight->w);
	pLeft->w = (quat[3] * pRight->w) - (quat[0] * pRight->x) - (quat[1] * pRight->y) - (quat[2] * pRight->z);

	#endif
}

void SIMDx86Quaternion_MultiplyOf(SIMDx86Quaternion* pOut, const SIMDx86Quaternion* pLeft, const SIMDx86Quaternion* pRight)
{
	#if defined(USE_3DNOW) && (USE_3DNOW >= 2)
	asm(

	"pswapd  (%0), %%mm1\n"		/* mm1 = Rx | Ry */
	"pswapd 8(%0), %%mm0\n"		/* mm0 = Rz | Rw */

        
	"movq  (%1), %%mm2\n"		/* mm2 = Ly | Lx */
	"movq 8(%1), %%mm7\n"		/* mm7 = Lw | Lz */

    "pswapd %%mm0, %%mm4\n"		/* mm4 = Rw | Rz */
	"pswapd %%mm1, %%mm5\n"		/* mm5 = Ry | Rx */


	"movq %%mm0, %%mm6\n"

	
	"movq %%mm2, %%mm3\n"		/* mm3 = mm2 = Ly | Lx */
	"punpckldq %%mm2, %%mm2\n"	/* mm2 = Lx | Lx */
	"punpckhdq %%mm3, %%mm3\n"	/* mm3 = Ly | Ly */


	"pfmul %%mm2, %%mm0\n"
	"pfmul %%mm2, %%mm1\n"


	"pxor _SIMDx86_float_POSNEG, %%mm0\n"
	"pxor _SIMDx86_float_POSNEG, %%mm1\n"


	"pswapd %%mm5, %%mm2\n"


	"pfmul %%mm3, %%mm4\n"
	"pfmul %%mm3, %%mm5\n"

    "pfadd %%mm4, %%mm0\n"
	"pfsub %%mm5, %%mm1\n"

	/*
	
		mm0 = C2+C3
		mm1 = C2+C3
		mm2 = Rx | Ry
		mm3 = free
		mm4 = free
		mm5 = free
		mm6 = Rz | Rw
		mm7 = Lw | Lz

		Column 4 = mm2 | mm6

        Column 1 = mm3{swap(mm2)} | mm4{swap(mm6)}
		mm5 = Lz | Lz
		mm7 = Lw | Lw

		*/

	"pswapd %%mm2, %%mm3\n"		/* mm3 = Ry | Rx */
	"movq %%mm7, %%mm5\n"		/* mm5 = Lw | Lz */
	"pswapd %%mm6, %%mm4\n"		/* mm4 = Rw | Rz */
	"punpckhdq %%mm7, %%mm7\n"	/* mm7 = Lw | Lw */
	"punpckldq %%mm5, %%mm5\n"	/* mm5 = Lz | Lz */



	"pfmul %%mm7, %%mm3\n"
	"pfmul %%mm7, %%mm4\n"

    "pfmul %%mm5, %%mm2\n"
	"pfmul %%mm5, %%mm6\n"

	"pfadd %%mm3, %%mm0\n"
	"pfadd %%mm4, %%mm1\n"

	"pxor _SIMDx86_float_NEGPOS, %%mm2\n"
	"pxor _SIMDx86_float_POSNEG, %%mm6\n"

	"pfadd %%mm2, %%mm0\n"
	"pfadd %%mm6, %%mm1\n"

    "movq %%mm0,  (%2)\n"
	"movq %%mm1, 8(%2)\n"

	:
	: "r" (pRight), "r" (pLeft), "r" (pOut)
	);


	#ifndef NO_EMMS
	asm("femms\n");
	#endif


	#elif defined(USE_SSE)

	asm(
	"movups (%0), %%xmm0\n" 	/* xmm0 = Rw | Rz | Ry | Rx */
	"movups (%1), %%xmm4\n" 	/* xmm1 = Lw | Lz | Ly | Lx */

	"movaps %%xmm0, %%xmm1\n"	/* xmm1 = xmm0 */
	"movaps %%xmm0, %%xmm2\n"	/* xmm2 = xmm0 */
	"movaps %%xmm0, %%xmm3\n"	/* xmm3 = xmm0 */

	"movaps %%xmm4, %%xmm5\n"	/* xmm5 = xmm4 */
	"movaps %%xmm4, %%xmm6\n"	/* xmm6 = xmm4 */
	"movaps %%xmm4, %%xmm7\n"	/* xmm7 = xmm4 */

	/*
	
		0000 0000 = xxxx = 0x00
	    0101 0101 = yyyy = 0x55
		1010 1010 = zzzz = 0xAA
		1111 1111 = wwww = 0xFF
	*/
	"shufps $0x00, %%xmm4, %%xmm4\n"		/* xmm4 = Rx | Rx | Rx | Rx */
	"shufps $0x55, %%xmm5, %%xmm5\n"		/* xmm5 = Ry | Ry | Ry | Ry */
	"shufps $0xAA, %%xmm6, %%xmm6\n"		/* xmm6 = Rz | Rz | Rz | Rz */
	"shufps $0xFF, %%xmm7, %%xmm7\n"		/* xmm7 = Rw | Rw | Rw | Rw */

	/*
		C1 = w | z | y | x = 1110 0100 =
		C2 = x | y | z | w = 0001 1011 = 0x1B
		C3 = y | x | w | z = 0100 1110 = 0x4E
		C4 = z | w | x | y = 1011 0001 = 0xB1
	*/

	"shufps $0x1B, %%xmm1, %%xmm1\n"
	"shufps $0x4E, %%xmm2, %%xmm2\n"
	"shufps $0xB1, %%xmm3, %%xmm3\n"

	"mulps %%xmm0, %%xmm7\n"		/* C1 *= Lw */
	"mulps %%xmm1, %%xmm4\n"		/* C2 *= Lx */
	"mulps %%xmm2, %%xmm5\n"		/* C3 *= Ly */
	"mulps %%xmm3, %%xmm6\n"		/* C4 *= Lz */

	"xorps _POSNEGPOSNEG, %%xmm4\n"		/* C2 = { + - + - } */
	"xorps _POSPOSNEGNEG, %%xmm5\n"		/* C3 = { + + - - } */
	"xorps _NEGPOSPOSNEG, %%xmm6\n"		/* C4 = { - + + - } */

	"addps %%xmm4, %%xmm5\n"		/* C2 += C1 */
	"addps %%xmm6, %%xmm7\n"		/* C4 += C3 */
	"addps %%xmm5, %%xmm7\n"		/* C4 += C2 */

	"movups %%xmm7, (%2)\n"
	:
	: "r" (pRight), "r" (pLeft), "r" (pOut)
	);


	#else


	pOut->x = (pLeft->w * pRight->x) + (pLeft->x * pRight->w) + (pLeft->y * pRight->z) - (pLeft->z * pRight->y);
	pOut->y = (pLeft->w * pRight->y) - (pLeft->x * pRight->z) + (pLeft->y * pRight->w) + (pLeft->z * pRight->x);
	pOut->z = (pLeft->w * pRight->z) + (pLeft->x * pRight->y) - (pLeft->y * pRight->x) + (pLeft->z * pRight->w);
	pOut->w = (pLeft->w * pRight->w) - (pLeft->x * pRight->x) - (pLeft->y * pRight->y) - (pLeft->z * pRight->z);

	#endif
}

void SIMDx86Quaternion_Conjugate(SIMDx86Quaternion* pQuat)
{
	pQuat->x = -pQuat->x;
	pQuat->y = -pQuat->y;
	pQuat->z = -pQuat->z;
}

void SIMDx86Quaternion_ConjugateOf(SIMDx86Quaternion* pOut, const SIMDx86Quaternion* pSrc)
{

	pOut->x = -pSrc->x;
	pOut->y = -pSrc->y;
	pOut->z = -pSrc->z;
	pOut->w = pSrc->w;
}

void SIMDx86Quaternion_ToMatrix(SIMDx86Matrix* pMat, const SIMDx86Quaternion* pQuat)
{
	float x_sq = pQuat->x * pQuat->x;
	float y_sq = pQuat->y * pQuat->y;
	float z_sq = pQuat->z * pQuat->z;

	float x_w = pQuat->x * pQuat->w;
	float y_w = pQuat->y * pQuat->w;
	float z_w = pQuat->z * pQuat->w;

	float x_y = pQuat->x * pQuat->y;
	float x_z = pQuat->x * pQuat->z;
	float y_z = pQuat->y * pQuat->z;

	pMat->m[0]  = 1.0f - 2.0f * (y_sq + z_sq);
	pMat->m[1]  = 2.0f * (x_y + z_w);
	pMat->m[2]  = 2.0f * (x_z - y_w);
	pMat->m[3]  = 0.0f;

	pMat->m[4]  = 2.0f * (x_y - z_w);
	pMat->m[5]  = 1.0f - 2.0f * (x_sq + z_sq);
	pMat->m[6]  = 2.0f * (y_z + x_w);
	pMat->m[7]  = 0.0f;

	pMat->m[8]  = 2.0f * (x_z + y_w);
	pMat->m[9]  = 2.0f * (y_z - x_w);
	pMat->m[10] = 1.0f - 2.0f * (x_sq + y_sq);
	pMat->m[11] = 0.0f;

	pMat->m[12] = 0;
	pMat->m[13] = 0;
	pMat->m[14] = 0;
	pMat->m[15] = 1.0f;

}

void SIMDx86Quaternion_Rotate(SIMDx86Quaternion* pOut, float rads, float x, float y, float z)
{
	float sine;
	float mag = 1.0f / (sqrtf(x*x + y*y + z*z) + FLT_EPSILON);

	x *= mag;
	y *= mag;
	z *= mag;

	rads *= 0.5f;
	sine = sinf(rads);
	pOut->x = x * sine;
	pOut->y = y * sine;
	pOut->z = z * sine;
	pOut->w = cosf(rads);

	SIMDx86Quaternion_Normalize(pOut);
}

void SIMDx86Quaternion_Slerp(SIMDx86Quaternion* pOut, const SIMDx86Quaternion* pSrc1, const SIMDx86Quaternion* pSrc2, float scalar)
{
	float sine, cosine;
	float beta = 1.0f - scalar;
	float quat[4];

	quat[0] = pSrc2->x;
	quat[1] = pSrc2->y;
	quat[2] = pSrc2->z;
	quat[3] = pSrc2->w;

	cosine = (pSrc1->x*pSrc2->x) + (pSrc1->y*pSrc2->y) + (pSrc1->z*pSrc2->z) + (pSrc1->w*pSrc2->w);

	if(cosine < 0.0f)
	{
 		quat[0] = -quat[0];
		quat[1] = -quat[1];
		quat[2] = -quat[2];
		quat[3] = -quat[3];
		cosine = -cosine;
	}

	if(1.0f - cosine > 0.0001f)
	{
		cosine = acosf(cosine);

		sine = 1.0f / sinf(cosine);

		beta = sinf(cosine * beta) * sine;
		scalar = sinf(cosine * scalar) * sine;
	}

	pOut->x = (beta * pSrc1->x) + (scalar * quat[0]);
	pOut->y = (beta * pSrc1->y) + (scalar * quat[1]);
	pOut->z = (beta * pSrc1->z) + (scalar * quat[2]);
	pOut->w = (beta * pSrc1->w) + (scalar * quat[3]);

}

