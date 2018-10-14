

#define OVERLAY( funcname, simdname) memcpy( funcname, simdname, simdname#size)


static void OverlayAMD_MMX()
{
}

static void OverlayAMD_3DNow()
{
}

static void OverlayAMD_3DNowPlus()
{
}

static void OverlayAMD_SSE()
{
}

static void OverlayAMD_SSE2()
{
}

static void OverlayAMD_SSE3()
{
}

static void OverlayAMD_SSSE3()
{
	OverlayAMD_SSE3();
}

static void OverlayIntel_MMX()
{
}

static void OverlayIntel_3DNow()
{
}

static void OverlayIntel_3DNowPlus()
{
}

static void OverlayIntel_SSE()
{
}

static void OverlayIntel_SSE2()
{
}

static void OverlayIntel_SSE3()
{
}

static void OverlayIntel_SSSE3()
{
	OverlayIntel_SSE3();
}
