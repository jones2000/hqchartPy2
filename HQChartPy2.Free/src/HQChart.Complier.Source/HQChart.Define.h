#pragma once


/////////////////////////////////////////////////////////////////////////////
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stddef.h>
#include	<math.h>
#include	<stdarg.h>
#include	<ctype.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<sys/stat.h>
#include	<sys/types.h>

#if (defined WIN32) || (defined WIN64)
	#include	<share.h>
	#include	<time.h>
	#include	<io.h>
	#include	<sys/utime.h>
	#include	<winsock2.h>
	#include	<windows.h>
	#include	<tchar.h>
#else
	#include	<wchar.h>
	#include	<unistd.h>
	#include	<pthread.h>
	#include	<dirent.h>
	#include	<fnmatch.h>
	#include	<zlib.h>
	#include	<netdb.h>
	#include	<signal.h>
	#include	<getopt.h>
	#include	<termios.h>
	#include	<sysexits.h>
	#include	<utime.h>
	#include	<syslog.h>

	#include	<netinet/in.h>
	#include	<arpa/inet.h>
	#include	<linux/fb.h>
	#include	<net/if.h>
	#include	<net/if_arp.h>

	#include	<sys/wait.h>
	#include	<sys/uio.h>
	#include	<sys/un.h>
	#include	<sys/socket.h>
	#include	<sys/ioctl.h>
	#include	<sys/mman.h>
	#include	<sys/io.h>
	#include	<sys/fcntl.h>
	#include	<sys/kd.h>
	#include	<sys/time.h>
	#include	<sys/param.h>
	#include	<sys/utsname.h>
	#include	<sys/resource.h>
	#include	<sys/termios.h>
#endif
#pragma pack(push,1)
/////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	typedef	unsigned char		XBYTE;
	typedef	unsigned short		XWORD;
	typedef	short				XINT16;
	typedef	long				XINT32;
	typedef	unsigned long		XDWORD;
	typedef	__int64				XINT64;
	typedef	unsigned __int64	XBIGINT;
	typedef long double			XLDOUBLE;  // long double win32下面8bytes， linux下面12bytes
	typedef unsigned long		XULONG;
#else
	typedef	unsigned char		XBYTE;
	typedef	unsigned short		XWORD;
	typedef	short				XINT16;
	typedef	int					XINT32;
	typedef	unsigned int		XDWORD;
	typedef	long long			XINT64;
	typedef	unsigned long long	XBIGINT;
	typedef double				XLDOUBLE;
	typedef unsigned long		XULONG;
#endif

#if (defined WIN32) || (defined WIN64)
	typedef	HANDLE	HTHREAD;
	typedef	HANDLE	HPIPE;

	typedef	int	socklen_t;
	#define	NOVTABLE __declspec(novtable)
	#ifdef TEST_SOLUTION
		#define PRIVATE   public
	#else
		#define PRIVATE   private
	#endif

	#ifdef TEST_SOLUTION
		#define PROTECTED   public
	#else
		#define PROTECTED   protected
	#endif
#else	//Global Define(like win)
	typedef	pthread_t	HTHREAD;
	typedef	int		HPIPE[2];
	typedef XDWORD			WPARAM;
	typedef XINT32			LPARAM;
	typedef const void		*LPCVOID;
	typedef const char		*LPCSTR;
	typedef char			*LPSTR;
	typedef const wchar_t	*LPCWSTR;
	typedef wchar_t			*LPWSTR;
	typedef	__U32_TYPE		__time32_t;
	typedef unsigned char	*LPBYTE;
	typedef XDWORD			COLORREF;
	typedef void			*LPVOID;
	typedef void			*HANDLE;
	typedef void			*HINSTANCE;
	typedef XINT32			BOOL;
	typedef XINT32			LRESULT;
	typedef int				SOCKET;
	typedef void*			HWND;
	typedef short			SHORT;
	typedef char			TCHAR;
	typedef const char* LPCTSTR;
	typedef char* LPTSTR;
	const XINT32 MAX_PATH=260;

	#define	FILE_BEGIN		0
	#define FILE_CURRENT	1
	#define FILE_END		2

	#define INVALID_SOCKET  (SOCKET)-1
	#define	WINAPI
	#define	NOVTABLE

	#define MAKEWORD(a,b)	((XWORD)(((XBYTE)(a))|((XWORD)((XBYTE)(b)))<<8))
	#define MAKELONG(a,b)	((XINT32)(((XWORD)(a))|((XDWORD)((XWORD)(b)))<<16))
	#define LOWORD(l)		((XWORD)(l))
	#define HIWORD(l)		((XWORD)(((XDWORD)(l)>>16)&0xFFFF))
	#define LOBYTE(w)		((XBYTE)(w))
	#define HIBYTE(w)		((XBYTE)(((XWORD)(w)>>8)&0xFF))
	#define RGB(r,g,b)		((COLORREF)(((XBYTE)(r)|((XWORD)((XBYTE)(g))<<8))|(((XDWORD)(XBYTE)(b))<<16)))

//critical section macro:
	typedef XINT32 HRESULT;
	typedef	pthread_mutex_t CRITICAL_SECTION;
	#define	InitializeCriticalSection(hMutex)	pthread_mutex_init(hMutex,NULL)
	#define	DeleteCriticalSection(hMutex)		pthread_mutex_destroy(hMutex)
	#define	EnterCriticalSection(hMutex)		pthread_mutex_lock(hMutex)
	#define	LeaveCriticalSection(hMutex)		pthread_mutex_unlock(hMutex)
	#define	InterlockedIncrement(pInt)			__sync_add_and_fetch(pInt,1)
	#define	InterlockedDecrement(pInt)			__sync_sub_and_fetch(pInt,1)
//map win func
	#define TlsAlloc() pthread_key_create()
	#define _vstprintf vsprintf
	#define _snprintf snprintf
	#define _sntprintf snprintf
	#define	lstrlenW(str) wcslen(str)
	#define	lstrlen(str) strlen(str)
	#define	lstrlenA(str) strlen(str)
	#define wsprintf	sprintf
	#define wsprintfW   swprintf
	#define wsprintfA	sprintf
	#define _istalnum   isalnum
	#define _istalpha	isalpha
	#define _istdigit	isdigit
	#define _atoi64 atoll
	#define _wtof(str)  wcstof(str, NULL)
	#define _wtoi64(str)    wcstoll (str, NULL, 10)
	#define _tfopen  fopen
	#define _wtol(str)  wcstol(str, NULL, 10)
	#define _wtoi(str)  wcstol(str, NULL, 10)
	#define sprintf_s sprintf
	#define	lstrcpy(str1,str2) strcpy(str1,str2)
	#define	lstrcpyA(str1,str2) strcpy(str1,str2)
	#define	lstrcpyn(str1,str2,size) strncpy(str1,str2,size)
	#define	lstrcpynW(str1,str2,size) wcscpy(str1,str2)
	#define	lstrcpynA(str1,str2,size) strncpy(str1,str2,size)
	#define strcpy_s(str1, size, str2) strncpy(str1, str2,size)
	//#define strcpy_s(str1, str2) strcpy(str1, str2)
	#define	lstrcat(str1,str2) strcat(str1,str2)
	#define	lstrcatA(str1,str2) strcat(str1,str2)
	#define	lstrcmp(str1,str2) strcmp(str1,str2)
	#define	lstrcmpA(str1,str2) strcmp(str1,str2)
	#define	lstrcmpi(str1,str2) strcasecmp(str1,str2)
	#define	lstrcmpiA(str1,str2) strcasecmp(str1,str2)
	#define	_stricmp(str1,str2) strcasecmp(str1,str2)
	#define	strnicmp(str1,str2,n) strncasecmp(str1,str2,n)
	#define	closesocket(h)  ::close(h)
	#define	Sleep(lSeconds)	usleep(lSeconds*1000L)
	#define		ARRAYSIZE(a)  (sizeof(a)/sizeof(a[0]))
	#define  _T(x)	x
	#define TEXT(x) 	x
	typedef struct _LARGE_INTEGER
	{
		struct
		{
			XDWORD	LowPart;
			XINT32	HighPart;
		};
		XINT64	QuadPart;
	}LARGE_INTEGER;

#ifndef FILETIME
	typedef struct _FILETIME
	{
		XDWORD	dwLowDateTime;
		XDWORD	dwHighDateTime;
	}FILETIME,*PFILETIME,*LPFILETIME;
#endif

	typedef struct _SYSTEMTIME
	{
		XWORD	wYear;
		XWORD	wMonth;
		XWORD	wDayOfWeek;
		XWORD	wDay;
		XWORD	wHour;
		XWORD	wMinute;
		XWORD	wSecond;
		XWORD	wMilliseconds;
	}SYSTEMTIME,*PSYSTEMTIME,*LPSYSTEMTIME;
	typedef struct tagDBTIMESTAMP
    {
		short	year;
		XWORD	month;
		XWORD	day;
		XWORD	hour;
		XWORD	minute;
		XWORD	second;
		XDWORD	fraction;
    }DBTIMESTAMP;
#endif

#ifndef	TRUE
	#define	TRUE	1
#endif

#ifndef	FALSE
	#define	FALSE	0
#endif


#ifndef	DELETE_PTR
	#define	DELETE_PTR(p) if(p){delete p;p=NULL;}
#endif

#ifndef	DELETE_WNDPTR
	#define	DELETE_WNDPTR(p) if(p){if(p->m_hWnd)p->DestroyWindow();delete p;p=NULL;}
#endif

#ifndef	DELETE_PTRA
	#define	DELETE_PTRA(p) if(p){delete[] p;p=NULL;}
#endif

#ifndef	COPY_STRING
	#define	COPY_STRING(p1,p2)\
	lstrcpyn(p1,p2,sizeof(p1))
#endif

#ifndef	COPY_STRINGEX
	#define	COPY_STRINGEX(p1,p2)\
	memset(p1,0,sizeof(p1));\
	lstrcpyn(p1,p2,sizeof(p1));
#endif

#ifndef	WLSTAT_DEFINE
	#define	WLSTAT_DEFINE
	#ifdef	_WIN32
		typedef	struct _stati64 STAT_STRU;
		#define	STAT_PROC _stati64
		#define	FSTAT_PROC _fstati64
	#else
		typedef	struct stat64 STAT_STRU;
		#define	STAT_PROC stat64
		#define	FSTAT_PROC fstat64
	#endif
#endif

#if (defined WIN32) || (defined WIN64)
	#define	WSA_LAST_ERROR	WSAGetLastError()
	#define	LAST_ERROR	GetLastError()

	#ifndef	CLOSE_HANDLE
		#define	CLOSE_HANDLE(h) if(h){CloseHandle(h);h=NULL;}
	#endif

	#ifndef	CLOSE_FILE
		#define	CLOSE_FILE(h) if(h!=INVALID_HANDLE_VALUE){CloseHandle(h);h=INVALID_HANDLE_VALUE;}
	#endif

	#ifndef	CLOSE_SOCKET
		#define	CLOSE_SOCKET(h)	if(h!=INVALID_SOCKET){closesocket(h);h=INVALID_SOCKET;}
	#endif

	#ifndef	THREAD_CREATE
		#define	THREAD_CREATE(AfxProc,Class,Method)\
		DWORD WINAPI AfxProc(LPVOID lpParam)\
		{\
			CoInitialize(NULL);\
			try\
			{\
				((Class*)lpParam)->Method();\
			}\
			catch(...)\
			{\
			}\
			CoUninitialize();\
			return(0);\
		}
	#endif

	#ifndef	THREAD_SAFE_CLOSE
		#define	THREAD_SAFE_CLOSE(h,tm)\
		if(h)\
		{\
			if(WaitForSingleObject(h,tm)!=WAIT_OBJECT_0)\
			{\
				TerminateThread(h,0);\
			}\
			CloseHandle(h);\
			h = NULL;\
		}
	#endif

	#define	DECLARE_FILEOPEN(fp,filename,openmode)	FILE*fp=_fsopen(filename,openmode,SH_DENYNO)
#else	//Linux
	#define	WM_USER		0x0400

	#define	WSA_LAST_ERROR	errno
	#define	LAST_ERROR	errno

	#ifndef	CLOSE_FILE
		#define	CLOSE_FILE(h) if(h!=NULL){fclose(h);h=NULL;}
	#endif

	#ifndef	CLOSE_SOCKET
		#define	CLOSE_SOCKET(h)	if(h>0){closesocket(h);h=-1;}
	#endif

	#ifndef	THREAD_CREATE
		#define	THREAD_CREATE(AfxProc,Class,Method)\
		void*AfxProc(void*lpParam)\
		{\
			try\
			{\
				((Class*)lpParam)->Method();\
			}\
			catch(...)\
			{\
			}\
			return(NULL);\
		}
	#endif

	#ifndef	THREAD_SAFE_CLOSE
		#define	THREAD_SAFE_CLOSE(h,tm)\
		if(h)\
		{\
			pthread_join(h,NULL);\
			h = 0;\
		}
	#endif

	#define	DECLARE_FILEOPEN(fp,filename,openmode)	FILE*fp=fopen(filename,openmode)
#endif
/////////////////////////////////////////////////////////////////////////////
#ifndef	ZERO_STRING
	#define	ZERO_STRING(s) memset(s,0,sizeof(s))
#endif

#ifndef	ROUNDUP_SIZE
	#define ROUNDUP_SIZE(size,amount)	(((ULONG)(size)+((amount)-1))&~((amount)-1))
#endif

#ifndef	CHECK_STRING_ENDNULL
	#define	CHECK_STRING_ENDNULL(s) s[sizeof(s)-1] = 0;
#endif

#ifdef	_WIN32
	#ifdef	_UNICODE
		#define	__string_lwr__(s) _wcslwr(s)
		#define	__string_upr__(s) _wcsupr(s)
		#define	__bigint_to_string__(i,s,b) _i64tow(i,s,b)
		#define	__string_to_bigint__(s) _wtoi64(s)
		#define	__long_to_string__(i,s,b) _itow(i,s,b)
		#define	__string_to_long__(s) _wtol(s)
		#define	__string_to_double__(s) _wtof(s)
		#define	__string_search__(s,sub) wcsstr(s,sub)
		#define	__string_char__(s,c) wcschr(s,c)
		#define	__csprintf__ swprintf
	#else
		#define	__string_lwr__(s) _strlwr(s)
		#define	__string_upr__(s) _strupr(s)
		#define	__bigint_to_string__(i,s,b) _i64toa(i,s,b)
		#define	__string_to_bigint__(s) _atoi64(s)
		#define	__long_to_string__(i,s,b) _itoa(i,s,b)
		#define	__string_to_long__(s) atol(s)
		#define	__string_to_double__(s) atof(s)
		#define	__string_search__(s,sub) strstr(s,sub)
		#define	__string_char__(s,c) strchr(s,c)
		#define	__csprintf__ sprintf
	#endif
	#define	__stringA_to_bigint__(s) _atoi64(s)
	#define	__bigint_to_stringA__(i,s)	_i64toa(i,s,10)
	#define	ustrcmpiA(str1,str2) _stricmp(str1,str2)
	#define	__long_to_stringA__(i,s)	_itoa(i,s,10)
	#define	ustrcmp(str1,str2) _tcscmp(str1,str2)
	#define	ustrcmpi(str1,str2) _tcsicmp(str1,str2)
	#define	ustrlen(str) lstrlen(str)
	#define	ustrnicmp(str1,str2,n) _strnicmp(str1,str2,n)
#else
	#define	__string_lwr__(s)  _strlwr(s)
	#define	__string_upr__(s)  _strupr(s)
	#define	__bigint_to_string__(i,s,b)    CONVERT_BIGINT(s,i)
	#define	__string_to_bigint__(s)  atoll(s)
	#define	__long_to_string__(i,s,b)  sprintf(s, "%d", i) 
	#define	__string_to_long__(s) atol(s)
	#define	__string_to_double__(s) atof(s)
	#define	__string_search__(s,sub) strstr(s,sub)
	#define	__string_char__(s,c) strchr(s,c)
	#define	__csprintf__ sprintf
	#define	__stringA_to_bigint__(s) atoll(s)
	#define	__bigint_to_stringA__(i,s)	CONVERT_BIGINT(s,i)
	#define	ustrcmpiA(str1,str2) strcasecmp(str1,str2)
	#define	__long_to_stringA__(i,s)	sprintf(s, "%d", i) 
	#define	ustrcmp(str1,str2) strcmp(str1,str2)
	#define	ustrcmpi(str1,str2) strcasecmp(str1,str2)
	#define	ustrlen(str) strlen(str)
	#define	ustrnicmp(str1,str2,n) strnicmp(str1,str2,n)
#endif



#pragma pack(pop)
