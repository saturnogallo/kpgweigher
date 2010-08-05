// DbgHelp.h
// 调试辅助头文件

#ifndef _DbgHelp_h_
#define _DbgHelp_h_

#ifdef _DEBUG
	#define DEBUG_EXP(e) ((void)(e))
#else
	#define DEBUG_EXP(e) ((void)0)
#endif	

#ifndef ASSERT
	#ifdef _DEBUG
		#include <assert.h>
		#define ASSERT(f) assert(f)
		#define VERIFY(f) assert(f)
	#else
		#define ASSERT(f) ((void)0)
		#define VERIFY(f) ((void)(f))
	#endif
#endif

#ifndef TRACE
	#ifdef _DEBUG
		#include <stdio.h>
		#define TRACE					printf
		#define TRACE0(sz)				printf(sz)
		#define TRACE1(sz, p1)			printf(sz, p1)
		#define TRACE2(sz, p1, p2)		printf(sz, p1, p2)
		#define TRACE3(sz, p1, p2, p3)	printf(sz, p1, p2, p3)
	#else
		#define TRACE					((void)0)
		#define TRACE0(sz)				((void)0)
		#define TRACE1(sz, p1)			((void)0)
		#define TRACE2(sz, p1, p2)		((void)0)
		#define TRACE3(sz, p1, p2, p3)	((void)0)
	#endif
#endif	

#endif //_DbgHelp_h_
