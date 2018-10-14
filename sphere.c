#include <SIMDx86/sphere.h>

int SIMDx86Sphere_SphereCollision(const SIMDx86Sphere* pIn1, const SIMDx86Sphere* pIn2)
{
	SIMDx86Vector dist;

	SIMDx86Vector_DiffOf(&dist, &pIn1->Center, &pIn2->Center);

	return ((pIn1->radius+pIn2->radius)*(pIn1->radius+pIn2->radius)) >= SIMDx86Vector_LengthSq(&dist);
}

int SIMDx86Sphere_ContainsPoint(const SIMDx86Sphere* pSphere, const SIMDx86Vector* pVec)
{
	SIMDx86Vector dist;

	SIMDx86Vector_DiffOf(&dist, &pSphere->Center, pVec);

	return SIMDx86Vector_LengthSq(&dist) <= (pSphere->radius*pSphere->radius);
}

int SIMDx86Sphere_ContainsPolygon(const SIMDx86Sphere* pSphere, const SIMDx86Polygon* pPoly)
{
	return SIMDx86Sphere_ContainsPoint(pSphere, &pPoly->a) && SIMDx86Sphere_ContainsPoint(pSphere, &pPoly->b) && SIMDx86Sphere_ContainsPoint(pSphere, &pPoly->c);
}

float SIMDx86Sphere_DistanceToPoint(const SIMDx86Sphere* pSphere, const SIMDx86Vector* pVec)
{
	return SIMDx86Vector_Distance(&pSphere->Center, pVec);
}

float SIMDx86Sphere_DistanceToPointSq(const SIMDx86Sphere* pSphere, const SIMDx86Vector* pVec)
{
	SIMDx86Vector v;

	SIMDx86Vector_DiffOf(&v, &pSphere->Center, pVec);

	return SIMDx86Vector_LengthSq(&v);
}

float SIMDx86Sphere_DistanceToSphere(const SIMDx86Sphere* pSphere1, const SIMDx86Sphere* pSphere2)
{
	SIMDx86Vector distvec;
	float dist;

	SIMDx86Vector_DiffOf(&distvec, &pSphere1->Center, &pSphere2->Center);

	dist = SIMDx86Vector_Length(&distvec);

	return dist - (pSphere1->radius + pSphere2->radius);
}

float SIMDx86Sphere_DistanceToSphereSq(const SIMDx86Sphere* pSphere1, const SIMDx86Sphere* pSphere2)
{
	SIMDx86Vector distvec;
	float dist;

	SIMDx86Vector_DiffOf(&distvec, &pSphere1->Center, &pSphere2->Center);

	dist = SIMDx86Vector_Length(&distvec) - (pSphere1->radius + pSphere2->radius);

	return dist * dist;
}
