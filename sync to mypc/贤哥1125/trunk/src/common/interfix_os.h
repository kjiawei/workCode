#ifndef INTERFIX_OS_H
#define INTERFIX_OS_H

#ifdef OS_WIN32
#include <windows.h>
#define interfix_sleep(x)      Sleep( (x) * 1000 )

#endif

#ifdef OS_LINUX
#include <unistd.h>

#define interfix_sleep(x)      sleep(x)

#endif

#endif
