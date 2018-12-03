/*
 * ksm - a really simple and fast x64 hypervisor
 * Copyright (C) 2016, 2017 Ahmed Samy <asamy@protonmail.com>
 *
 * Define UM before including this file to eliminate unneeded 
 * definitions.
 *
 * Public domain.
*/
#ifndef __COMPILER_H
#define __COMPILER_H

#ifdef __NetBSD__
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stdint.h>
#endif

#include "inttypes.h"

#ifdef DBG
#if defined(__linux__) || defined(__NetBSD__)
#define BREAK()	(void)0		//__asm __volatile("int $3")
#else
#define BREAK() do {		\
	if (KD_DEBUGGER_ENABLED && !KD_DEBUGGER_NOT_PRESENT)	\
		__debugbreak();		\
} while (0)
#endif
#else
#define BREAK()	(void)0
#endif

#define BREAK_ON(cond)	do {	\
	if (!!(cond))		\
		BREAK();	\
} while (0)

#if !defined(__linux__)
/* Windows definitions  */
#define DIV_ROUND_UP(n, d) 	(((n) + (d) - 1) / (d))

/* Sync with Linux  */
#define BUG_ON(cond)	do {	\
	if (!!(cond))		\
		__ud2();	\
} while (0)

#define WARN_ON(cond)	do {	\
	if (!!(cond))		\
		KSM_DEBUG("Badness in %s at %s:%d\n", __func__, __FILE__, __LINE__);	\
} while (0)

/* Don't expose this to the assembler, it won't understand it.  */
#ifndef __ASSEMBLY__
/* Long names  */
#ifndef __NetBSD__
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned long long uint64_t;
#endif

typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed long int sint32_t;
typedef signed long long sint64_t;

/* Short names  */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef sint8_t s8;
typedef sint16_t s16;
typedef sint32_t s32;
typedef sint64_t s64;

#ifndef __NetBSD__
#ifndef _UINTPTR_T_DEFINED
typedef unsigned long long uintptr_t;
#define _UINTPTR_T_DEFINED
#endif

#ifndef _INTPTR_T_DEFINED
typedef signed long long intptr_t;
#define _INTPTR_T_DEFINED
#endif

#ifndef _bool_true_false_are_defined
typedef _Bool bool;
#define true	1
#define false	0
#define _bool_true_false_are_defined
#endif
#endif

#ifdef __NetBSD__
#define	container_of(PTR, TYPE, FIELD)					\
     ((TYPE *)(((char *)(PTR)) - offsetof(TYPE, FIELD))			\
     + __validate_container_of(PTR, TYPE, FIELD))
#else
/* Avoid NT retardism  */
#define container_of(address, type, field)	CONTAINING_RECORD(address, type, field)
#endif
/* OTOH - MSVC does not have typeof.  Hack it.  */
#define container_off_var(var, member)			\
	((char *)&(var)->member - (char *)(var))
#define container_of_var(ptr, var, member)	\
	((char *)ptr - container_off_var(var, member))

#ifndef UM
#include "list.h"

#ifndef __NetBSD__
typedef KSPIN_LOCK spinlock_t;
#define spin_lock_init		KeInitializeSpinLock
#define spin_lock(s) \
	KLOCK_QUEUE_HANDLE q;	\
	KeAcquireInStackQueuedSpinLock((s), &q)
#define spin_unlock(s)	\
	KeReleaseInStackQueuedSpinLock(&(q))
#define spin_lock_irqsave(s,f)		spin_lock((s));		(void)f
#define spin_unlock_irqrestore(s,f)	spin_unlock((s));	(void)f

NTKERNELAPI UCHAR *NTAPI PsGetProcessImageFileName(PEPROCESS process);
#endif
#endif
#endif

#ifdef _MSC_VER
/* MSVC specific definitions  */
/* Disable annoying warnings  */
#pragma warning(disable:4115)		/* 'type' : named type definition in parentheses  */
#pragma warning(disable:4201)		/* nonstandard extension used : nameless struct/union  */
#pragma warning(disable:4214)		/* nonstandard extension used : bit field types other than int  */
#pragma warning(disable:4702)		/* unreachable code  */
#pragma warning(disable:4100)		/* 'identifier' : unreferenced formal parameter  */
#pragma warning(disable:4200)		/* nonstandard extension used : zero-sized array in struct/union  */
#pragma warning(disable:4098)		/* 'function' : void function returning a value  */
#pragma warning(disable:4204)		/* nonstandard extension used : non-constant aggregate initializer  */
#pragma warning(disable:4189)		/* 'identifier' : local variable is initialized but not referenced  */
#pragma warning(disable:4706)		/* assignment within conditional expression  */
#pragma warning(disable:4221)		/* nonstandard extension used : 'identifier' : cannot be initialized using address of automatic variable  */
#pragma warning(disable:4054)		/*  'conversion' : from function pointer 'type1' to data pointer 'type2'  */

#define __align(alignment)	__declspec(align(alignment))
#define __packed
#else
/* GCC (Windows) specific definitions  */
#define _In_
#define _In_opt_
#define __align(alignment)	__attribute__((__aligned__(alignment)))
#ifndef __forceinline
#define __forceinline		__attribute__((always_inline)) inline
#endif
#define __packed		__attribute__((__packed__))
#ifndef __NetBSD__
#include <ntstatus.h>
#endif

#define STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR	0xC035003C
#define STATUS_HV_NOT_ALLOWED_WITH_NESTED_VIRT_ACTIVE	0xC0350071
#define STATUS_HV_FEATURE_UNAVAILABLE			0xC035001E
#define STATUS_HV_ACCESS_DENIED				0xC0350006
#define STATUS_HV_NOT_PRESENT				0xC0351000
#endif

#if defined(ENABLE_DBGPRINT) || defined(ENABLE_FILEPRINT)
#define ENABLE_PRINT
#endif
#define ERR_NOTH 		STATUS_HV_NOT_PRESENT
#define ERR_CPUID		STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR
#define ERR_BUSY		STATUS_DEVICE_BUSY
#define ERR_FEAT		STATUS_HV_FEATURE_UNAVAILABLE
#define ERR_UNSUP		STATUS_NOT_SUPPORTED
#define ERR_DENIED		STATUS_HV_ACCESS_DENIED
#define ERR_NOMEM		STATUS_NO_MEMORY
#define ERR_EXCEPT		GetExceptionCode()
#define ERR_RANGE		STATUS_BUFFER_OVERFLOW
#define ERR_INVAL		STATUS_INVALID_PARAMETER_1
#define ERR_EXIST		STATUS_ADDRESS_ALREADY_EXISTS
#else
#ifndef __NetBSD__
/* Linux definitions  */
#ifndef __ASSEMBLY__
#include <stdbool.h>
#endif
#include <asm-generic/errno-base.h>
#endif

#define __align(alignment)	__attribute__((__aligned__(alignment)))

#define ERR_NOTH 		-ENOENT
#define ERR_CPUID		-EOPNOTSUPP
#define ERR_BUSY		-EBUSY
#define ERR_FEAT		-ENOENT
#define ERR_UNSUP		-EOPNOTSUPP
#define ERR_DENIED		-EACCES
#define ERR_NOMEM		-ENOMEM
#define ERR_EXCEPT		-EACCES
#define ERR_RANGE		-ERANGE
#define ERR_INVAL		-EINVAL
#define ERR_EXIST		-EEXIST
#endif

#ifndef __func__
#define __func__ __FUNCTION__
#endif

#endif
