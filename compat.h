#ifndef _COMPAT_H_
#define _COMPAT_H_

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
/* Don't redefine EAGAIN/EINTR since they're already defined by errno.h */
/* We'll use the native Windows values instead */
#define HAVE_WINSOCK2
#define sleep(secs) Sleep((secs) * 1000)

enum {
	PRIO_PROCESS		= 0,
};
#endif

#ifndef WIN32
static inline int setpriority(int which, int who, int prio)
{
	return 0;
}
#else
/* Windows setpriority implementation */
static inline int setpriority(int which, int who, int prio)
{
	return -!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
}
#endif

#endif /* _COMPAT_H_ */
