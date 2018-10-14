#include <SIMDx86/version.h>

#include <stdio.h> 
#if defined(USE_SSE)
	#if USE_SSE == 1
		#define BASESTRING	"Intel SSE"
	#elif USE_SSE == 2
		#define BASESTRING	"Intel SSE2"
	#elif USE_SSE == 3
		#define	BASESTRING	"Intel SSE3"
	#elif USE_SSE == 4
		#defube BASESTRING	"Intel SSE4"
	#else
		#define BASESTRING	"? Intel SSE"
	#endif
#elif defined(USE_3DNOW)

	#if USE_3DNOW == 1
		#define BASESTRING	"AMD 3DNow!"
	#elif USE_3DNOW == 2
		#define BASESTRING	"AMD 3DNow!+/MMX+"
	#else
		#define BASESTRING	"? AMD 3DNow!"
	#endif
#elif defined(USE_MMX)

	#if USE_MMX == 1
		#define BASESTRING	"Intel MMX"
	#else
		#define BASESTRING	"? Intel MMX"
	#endif

#else
	#define BASESTRING	"C Implementation or Generic x87 FPU"
#endif

#if defined(NO_EMMS)
	static int noemms = 1;
#else
	static int noemms = 0;
#endif

#if defined(HIPREC)
	static int hiprec = 1;
#else
	static int hiprec = 0;
#endif


int SIMDx86_GetMajorVersion()
{
	return SIMDX86_VERSION_MAJOR;
}

int SIMDx86_GetMinorVersion()
{
	return SIMDX86_VERSION_MINOR;
}


int SIMDx86_GetRevisionVersion()
{
	return SIMDX86_VERSION_REVISION;
}

const char* SIMDx86_GetBuildString()
{
	static char build_string[1024];

	sprintf(build_string, "%s%s%s",
	BASESTRING,
	hiprec ? " (High Precision)" : "",
	noemms ? " (No F/EMMS)" : "");


	return build_string;
}

