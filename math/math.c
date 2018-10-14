#include <math.h> 

float SIMDx86_sqrtf(float value)
{
		return sqrtf(value);
}

float SIMDx86_rsqrtf(float value)
{
		return 1.0f / sqrtf(value);
}


double SIMDx86_sqrt(double value)
{
	return sqrt(value);
}

double SIMDx86_rsqrt(double value)
{
	return 1.0 / sqrt(value);
}
