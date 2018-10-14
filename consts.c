#include <SIMDx86/align.h>

#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)

double ALIGNED SIMDx86_double_one[4] = { 1.0, 1.0, 1.0, 1.0 };
float ALIGNED SIMDx86_float_one[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float ALIGNED SIMDx86_float_one_w[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float ALIGNED SIMDx86_float_one_y[4] = { 0.0f, 1.0f, 0.0f, 0.0f };

unsigned int ALIGNED SIMDx86_float_POSNEG[2] = { 0x00000000, 0x80000000 };	/* XOR  + - */
unsigned int ALIGNED SIMDx86_float_NEGPOS[2] = { 0x80000000, 0x00000000 };	/* XOR  - + */
unsigned int ALIGNED SIMDx86_float_NEGNEG[2] = { 0x80000000, 0x80000000 };	/* XOR  - - */
unsigned int ALIGNED SIMDx86_float_NEGPOSPOSPOS[4] = { 0x80000000, 0x00000000, 0x00000000, 0x00000000 };
unsigned int ALIGNED SIMDx86_float_POSPOSPOSNEG[4] = { 0x00000000, 0x00000000, 0x00000000, 0x80000000 };
unsigned int ALIGNED SIMDx86_float_SSE_NO_W_MASK[4] = { 0xFFFFFFFF,  0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
unsigned int ALIGNED SIMDx86_float_SSE_NO_XYZ_MASK[4] = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF };
unsigned int ALIGNED SIMDx86_float_3DNOW_NO_W_MASK[2] = { 0xFFFFFFFF, 0x00000000 };
unsigned int ALIGNED SIMDx86_float_ABS[4] = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,0x7FFFFFFF };

#else

double ALIGNED _SIMDx86_double_one[4] = { 1.0, 1.0, 1.0, 1.0 };
float ALIGNED _SIMDx86_float_one[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float ALIGNED _SIMDx86_float_one_w[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float ALIGNED _SIMDx86_float_one_y[4] = { 0.0f, 1.0f, 0.0f, 0.0f };

unsigned int ALIGNED _SIMDx86_float_POSNEG[2] = { 0x00000000, 0x80000000 };	/* XOR + - */
unsigned int ALIGNED _SIMDx86_float_NEGPOS[2] = { 0x80000000, 0x00000000 };	/* XOR - + */
unsigned int ALIGNED _SIMDx86_float_NEGNEG[2] = { 0x80000000, 0x80000000 };	/* XOR - - */
unsigned int ALIGNED _SIMDx86_float_NEGPOSPOSPOS[4] = { 0x80000000, 0x00000000, 0x00000000, 0x00000000 };
unsigned int ALIGNED _SIMDx86_float_POSPOSPOSNEG[4] = { 0x00000000, 0x00000000, 0x00000000, 0x80000000 };
unsigned int ALIGNED _SIMDx86_float_SSE_NO_W_MASK[4] = { 0xFFFFFFFF,  0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
unsigned int ALIGNED _SIMDx86_float_SSE_NO_XYZ_MASK[4] = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF };
unsigned int ALIGNED _SIMDx86_float_3DNOW_NO_W_MASK[2] = { 0xFFFFFFFF, 0x00000000 };
unsigned int ALIGNED _SIMDx86_float_ABS[4] = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF,0x7FFFFFFF };


#endif
