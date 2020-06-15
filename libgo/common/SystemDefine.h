#ifndef _SYSTEMDEFINE_H_
#define _SYSTEMDEFINE_H_

#ifdef _WIN32
#define SYSTEM_MACRO_WINDOWS
#ifdef _WIN64
#define SYSTEM_MACRO_64BIT
#else
#define SYSTEM_MACRO_32BIT
#endif // _WIN64
#endif // _WIN32

#ifdef __linux__
#define SYSTEM_MACRO_LINUX
#ifdef __x86_64__ 
#define SYSTEM_MACRO_64BIT
#endif
#ifdef __i386__
#define SYSTEM_MACRO_32BIT
#endif
#endif // __linux__

#endif//_SYSTEMDEFINE_H_
