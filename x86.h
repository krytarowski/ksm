/*
 * Mostly MSR indices, APIC definitions, etc.
 * Hacked off the Linux Kernel.
 *
 * From:
 *	arch/x86/include/uapi/asm
 *
 * Also helpers for segmentation and inline assembly for some
 * "misisng" intrinsics.
 */
#ifndef __X86_H
#define __X86_H

#if !defined(_AMD64_) && !defined(__x86_64) && !defined(_M_AMD64) && !defined(__M_X64)
#error only 64-bit is supported
#endif

#include "compiler.h"
#include "mm.h"

#ifndef __linux__
#define bit(b) 			(1 << (b))
#define _BITUL(bit)		(1ul << (bit))

#define X86_EFLAGS_CF_BIT	0 /* Carry Flag */
#define X86_EFLAGS_CF		_BITUL(X86_EFLAGS_CF_BIT)
#define X86_EFLAGS_FIXED_BIT	1 /* Bit 1 - always on */
#define X86_EFLAGS_FIXED	_BITUL(X86_EFLAGS_FIXED_BIT)
#define X86_EFLAGS_PF_BIT	2 /* Parity Flag */
#define X86_EFLAGS_PF		_BITUL(X86_EFLAGS_PF_BIT)
#define X86_EFLAGS_AF_BIT	4 /* Auxiliary carry Flag */
#define X86_EFLAGS_AF		_BITUL(X86_EFLAGS_AF_BIT)
#define X86_EFLAGS_ZF_BIT	6 /* Zero Flag */
#define X86_EFLAGS_ZF		_BITUL(X86_EFLAGS_ZF_BIT)
#define X86_EFLAGS_SF_BIT	7 /* Sign Flag */
#define X86_EFLAGS_SF		_BITUL(X86_EFLAGS_SF_BIT)
#define X86_EFLAGS_TF_BIT	8 /* Trap Flag */
#define X86_EFLAGS_TF		_BITUL(X86_EFLAGS_TF_BIT)
#define X86_EFLAGS_IF_BIT	9 /* Interrupt Flag */
#define X86_EFLAGS_IF		_BITUL(X86_EFLAGS_IF_BIT)
#define X86_EFLAGS_DF_BIT	10 /* Direction Flag */
#define X86_EFLAGS_DF		_BITUL(X86_EFLAGS_DF_BIT)
#define X86_EFLAGS_OF_BIT	11 /* Overflow Flag */
#define X86_EFLAGS_OF		_BITUL(X86_EFLAGS_OF_BIT)
#define X86_EFLAGS_IOPL_BIT	12 /* I/O Privilege Level (2 bits) */
#define X86_EFLAGS_IOPL		(_AC(3,UL) << X86_EFLAGS_IOPL_BIT)
#define X86_EFLAGS_NT_BIT	14 /* Nested Task */
#define X86_EFLAGS_NT		_BITUL(X86_EFLAGS_NT_BIT)
#define X86_EFLAGS_RF_BIT	16 /* Resume Flag */
#define X86_EFLAGS_RF		_BITUL(X86_EFLAGS_RF_BIT)
#define X86_EFLAGS_VM_BIT	17 /* Virtual Mode */
#define X86_EFLAGS_VM		_BITUL(X86_EFLAGS_VM_BIT)
#define X86_EFLAGS_AC_BIT	18 /* Alignment Check/Access Control */
#define X86_EFLAGS_AC		_BITUL(X86_EFLAGS_AC_BIT)
#define X86_EFLAGS_VIF_BIT	19 /* Virtual Interrupt Flag */
#define X86_EFLAGS_VIF		_BITUL(X86_EFLAGS_VIF_BIT)
#define X86_EFLAGS_VIP_BIT	20 /* Virtual Interrupt Pending */
#define X86_EFLAGS_VIP		_BITUL(X86_EFLAGS_VIP_BIT)
#define X86_EFLAGS_ID_BIT	21 /* CPUID detection */
#define X86_EFLAGS_ID		_BITUL(X86_EFLAGS_ID_BIT)

 /*
 * Basic CPU control in CR0
 */
#define X86_CR0_PE_BIT		0 /* Protection Enable */
#define X86_CR0_PE		_BITUL(X86_CR0_PE_BIT)
#define X86_CR0_MP_BIT		1 /* Monitor Coprocessor */
#define X86_CR0_MP		_BITUL(X86_CR0_MP_BIT)
#define X86_CR0_EM_BIT		2 /* Emulation */
#define X86_CR0_EM		_BITUL(X86_CR0_EM_BIT)
#define X86_CR0_TS_BIT		3 /* Task Switched */
#define X86_CR0_TS		_BITUL(X86_CR0_TS_BIT)
#define X86_CR0_ET_BIT		4 /* Extension Type */
#define X86_CR0_ET		_BITUL(X86_CR0_ET_BIT)
#define X86_CR0_NE_BIT		5 /* Numeric Error */
#define X86_CR0_NE		_BITUL(X86_CR0_NE_BIT)
#define X86_CR0_WP_BIT		16 /* Write Protect */
#define X86_CR0_WP		_BITUL(X86_CR0_WP_BIT)
#define X86_CR0_AM_BIT		18 /* Alignment Mask */
#define X86_CR0_AM		_BITUL(X86_CR0_AM_BIT)
#define X86_CR0_NW_BIT		29 /* Not Write-through */
#define X86_CR0_NW		_BITUL(X86_CR0_NW_BIT)
#define X86_CR0_CD_BIT		30 /* Cache Disable */
#define X86_CR0_CD		_BITUL(X86_CR0_CD_BIT)
#define X86_CR0_PG_BIT		31 /* Paging */
#define X86_CR0_PG		_BITUL(X86_CR0_PG_BIT)

 /*
 * Paging options in CR3
 */
#define X86_CR3_PWT_BIT		3 /* Page Write Through */
#define X86_CR3_PWT		_BITUL(X86_CR3_PWT_BIT)
#define X86_CR3_PCD_BIT		4 /* Page Cache Disable */
#define X86_CR3_PCD		_BITUL(X86_CR3_PCD_BIT)
#define X86_CR3_PCID_MASK	_AC(0x00000fff,UL) /* PCID Mask */

 /*
 * Intel CPU features in CR4
 */
#define X86_CR4_VME_BIT		0 /* enable vm86 extensions */
#define X86_CR4_VME		_BITUL(X86_CR4_VME_BIT)
#define X86_CR4_PVI_BIT		1 /* virtual interrupts flag enable */
#define X86_CR4_PVI		_BITUL(X86_CR4_PVI_BIT)
#define X86_CR4_TSD_BIT		2 /* disable time stamp at ipl 3 */
#define X86_CR4_TSD		_BITUL(X86_CR4_TSD_BIT)
#define X86_CR4_DE_BIT		3 /* enable debugging extensions */
#define X86_CR4_DE		_BITUL(X86_CR4_DE_BIT)
#define X86_CR4_PSE_BIT		4 /* enable page size extensions */
#define X86_CR4_PSE		_BITUL(X86_CR4_PSE_BIT)
#define X86_CR4_PAE_BIT		5 /* enable physical address extensions */
#define X86_CR4_PAE		_BITUL(X86_CR4_PAE_BIT)
#define X86_CR4_MCE_BIT		6 /* Machine check enable */
#define X86_CR4_MCE		_BITUL(X86_CR4_MCE_BIT)
#define X86_CR4_PGE_BIT		7 /* enable global pages */
#define X86_CR4_PGE		_BITUL(X86_CR4_PGE_BIT)
#define X86_CR4_PCE_BIT		8 /* enable performance counters at ipl 3 */
#define X86_CR4_PCE		_BITUL(X86_CR4_PCE_BIT)
#define X86_CR4_OSFXSR_BIT	9 /* enable fast FPU save and restore */
#define X86_CR4_OSFXSR		_BITUL(X86_CR4_OSFXSR_BIT)
#define X86_CR4_OSXMMEXCPT_BIT	10 /* enable unmasked SSE exceptions */
#define X86_CR4_OSXMMEXCPT	_BITUL(X86_CR4_OSXMMEXCPT_BIT)
#define X86_CR4_VMXE_BIT	13 /* enable VMX virtualization */
#define X86_CR4_VMXE		_BITUL(X86_CR4_VMXE_BIT)
#define X86_CR4_SMXE_BIT	14 /* enable safer mode (TXT) */
#define X86_CR4_SMXE		_BITUL(X86_CR4_SMXE_BIT)
#define X86_CR4_FSGSBASE_BIT	16 /* enable RDWRFSGS support */
#define X86_CR4_FSGSBASE	_BITUL(X86_CR4_FSGSBASE_BIT)
#define X86_CR4_PCIDE_BIT	17 /* enable PCID support */
#define X86_CR4_PCIDE		_BITUL(X86_CR4_PCIDE_BIT)
#define X86_CR4_OSXSAVE_BIT	18 /* enable xsave and xrestore */
#define X86_CR4_OSXSAVE		_BITUL(X86_CR4_OSXSAVE_BIT)
#define X86_CR4_SMEP_BIT	20 /* enable SMEP support */
#define X86_CR4_SMEP		_BITUL(X86_CR4_SMEP_BIT)
#define X86_CR4_SMAP_BIT	21 /* enable SMAP support */
#define X86_CR4_SMAP		_BITUL(X86_CR4_SMAP_BIT)
#define X86_CR4_PKE_BIT		22 /* enable Protection Keys support */
#define X86_CR4_PKE		_BITUL(X86_CR4_PKE_BIT)
#endif

/* Interrupts/Exceptions */
#define X86_TRAP_DE			0	/*  0, Divide-by-zero */
#define X86_TRAP_DB			1	/*  1, Debug */
#define X86_TRAP_NMI			2	/*  2, Non-maskable Interrupt */
#define X86_TRAP_BP			3	/*  3, Breakpoint */
#define X86_TRAP_OF			4	/*  4, Overflow */
#define X86_TRAP_BR			5	/*  5, Bound Range Exceeded */
#define X86_TRAP_UD			6	/*  6, Invalid Opcode */
#define X86_TRAP_NM			7	/*  7, Device Not Available */
#define X86_TRAP_DF			8	/*  8, Double Fault */
#define X86_TRAP_OLD_MF			9	/*  9, Coprocessor Segment Overrun */
#define X86_TRAP_TS			10	/* 10, Invalid TSS */
#define X86_TRAP_NP			11	/* 11, Segment Not Present */
#define X86_TRAP_SS			12	/* 12, Stack Segment Fault */
#define X86_TRAP_GP			13	/* 13, General Protection Fault */
#define X86_TRAP_PF			14	/* 14, Page Fault */
#define X86_TRAP_SPURIOUS		15	/* 15, Spurious Interrupt */
#define X86_TRAP_MF			16	/* 16, x87 Floating-Point Exception */
#define X86_TRAP_AC			17	/* 17, Alignment Check */
#define X86_TRAP_MC			18	/* 18, Machine Check */
#define X86_TRAP_XF			19	/* 19, SIMD Floating-Point Exception */
#define X86_TRAP_VE			20	/* 20, Virtualization Exception  */
#define X86_TRAP_IRET			32	/* 32, IRET Exception */

#define GATE_INTERRUPT  	0xE
#define GATE_TRAP		0xF
#define GATE_CALL		0xC
#define GATE_TASK		0x5

#ifndef __linux__
/* Intel-defined CPU features, CPUID level 0x00000001 (edx), word 0 */
#define X86_FEATURE_FPU		( 0*32+ 0) /* Onboard FPU */
#define X86_FEATURE_VME		( 0*32+ 1) /* Virtual Mode Extensions */
#define X86_FEATURE_DE		( 0*32+ 2) /* Debugging Extensions */
#define X86_FEATURE_PSE		( 0*32+ 3) /* Page Size Extensions */
#define X86_FEATURE_TSC		( 0*32+ 4) /* Time Stamp Counter */
#define X86_FEATURE_MSR		( 0*32+ 5) /* Model-Specific Registers */
#define X86_FEATURE_PAE		( 0*32+ 6) /* Physical Address Extensions */
#define X86_FEATURE_MCE		( 0*32+ 7) /* Machine Check Exception */
#define X86_FEATURE_CX8		( 0*32+ 8) /* CMPXCHG8 instruction */
#define X86_FEATURE_APIC	( 0*32+ 9) /* Onboard APIC */
#define X86_FEATURE_SEP		( 0*32+11) /* SYSENTER/SYSEXIT */
#define X86_FEATURE_MTRR	( 0*32+12) /* Memory Type Range Registers */
#define X86_FEATURE_PGE		( 0*32+13) /* Page Global Enable */
#define X86_FEATURE_MCA		( 0*32+14) /* Machine Check Architecture */
#define X86_FEATURE_CMOV	( 0*32+15) /* CMOV instructions */
					  /* (plus FCMOVcc, FCOMI with FPU) */
#define X86_FEATURE_PAT		( 0*32+16) /* Page Attribute Table */
#define X86_FEATURE_PSE36	( 0*32+17) /* 36-bit PSEs */
#define X86_FEATURE_PN		( 0*32+18) /* Processor serial number */
#define X86_FEATURE_CLFLUSH	( 0*32+19) /* CLFLUSH instruction */
#define X86_FEATURE_DS		( 0*32+21) /* "dts" Debug Store */
#define X86_FEATURE_ACPI	( 0*32+22) /* ACPI via MSR */
#define X86_FEATURE_MMX		( 0*32+23) /* Multimedia Extensions */
#define X86_FEATURE_FXSR	( 0*32+24) /* FXSAVE/FXRSTOR, CR4.OSFXSR */
#define X86_FEATURE_XMM		( 0*32+25) /* "sse" */
#define X86_FEATURE_XMM2	( 0*32+26) /* "sse2" */
#define X86_FEATURE_SELFSNOOP	( 0*32+27) /* "ss" CPU self snoop */
#define X86_FEATURE_HT		( 0*32+28) /* Hyper-Threading */
#define X86_FEATURE_ACC		( 0*32+29) /* "tm" Automatic clock control */
#define X86_FEATURE_IA64	( 0*32+30) /* IA-64 processor */
#define X86_FEATURE_PBE		( 0*32+31) /* Pending Break Enable */

/* AMD-defined CPU features, CPUID level 0x80000001, word 1 */
/* Don't duplicate feature flags which are redundant with Intel! */
#define X86_FEATURE_SYSCALL	( 1*32+11) /* SYSCALL/SYSRET */
#define X86_FEATURE_MP		( 1*32+19) /* MP Capable. */
#define X86_FEATURE_NX		( 1*32+20) /* Execute Disable */
#define X86_FEATURE_MMXEXT	( 1*32+22) /* AMD MMX extensions */
#define X86_FEATURE_FXSR_OPT	( 1*32+25) /* FXSAVE/FXRSTOR optimizations */
#define X86_FEATURE_GBPAGES	( 1*32+26) /* "pdpe1gb" GB pages */
#define X86_FEATURE_RDTSCP	( 1*32+27) /* RDTSCP */
#define X86_FEATURE_LM		( 1*32+29) /* Long Mode (x86-64) */
#define X86_FEATURE_3DNOWEXT	( 1*32+30) /* AMD 3DNow! extensions */
#define X86_FEATURE_3DNOW	( 1*32+31) /* 3DNow! */

/* Transmeta-defined CPU features, CPUID level 0x80860001, word 2 */
#define X86_FEATURE_RECOVERY	( 2*32+ 0) /* CPU in recovery mode */
#define X86_FEATURE_LONGRUN	( 2*32+ 1) /* Longrun power control */
#define X86_FEATURE_LRTI	( 2*32+ 3) /* LongRun table interface */

/* Other features, Linux-defined mapping, word 3 */
/* This range is used for feature bits which conflict or are synthesized */
#define X86_FEATURE_CXMMX	( 3*32+ 0) /* Cyrix MMX extensions */
#define X86_FEATURE_K6_MTRR	( 3*32+ 1) /* AMD K6 nonstandard MTRRs */
#define X86_FEATURE_CYRIX_ARR	( 3*32+ 2) /* Cyrix ARRs (= MTRRs) */
#define X86_FEATURE_CENTAUR_MCR	( 3*32+ 3) /* Centaur MCRs (= MTRRs) */
/* cpu types for specific tunings: */
#define X86_FEATURE_K8		( 3*32+ 4) /* "" Opteron, Athlon64 */
#define X86_FEATURE_K7		( 3*32+ 5) /* "" Athlon */
#define X86_FEATURE_P3		( 3*32+ 6) /* "" P3 */
#define X86_FEATURE_P4		( 3*32+ 7) /* "" P4 */
#define X86_FEATURE_CONSTANT_TSC ( 3*32+ 8) /* TSC ticks at a constant rate */
#define X86_FEATURE_UP		( 3*32+ 9) /* smp kernel running on up */
#define X86_FEATURE_ART		( 3*32+10) /* Platform has always running timer (ART) */
#define X86_FEATURE_ARCH_PERFMON ( 3*32+11) /* Intel Architectural PerfMon */
#define X86_FEATURE_PEBS	( 3*32+12) /* Precise-Event Based Sampling */
#define X86_FEATURE_BTS		( 3*32+13) /* Branch Trace Store */
#define X86_FEATURE_SYSCALL32	( 3*32+14) /* "" syscall in ia32 userspace */
#define X86_FEATURE_SYSENTER32	( 3*32+15) /* "" sysenter in ia32 userspace */
#define X86_FEATURE_REP_GOOD	( 3*32+16) /* rep microcode works well */
#define X86_FEATURE_MFENCE_RDTSC ( 3*32+17) /* "" Mfence synchronizes RDTSC */
#define X86_FEATURE_LFENCE_RDTSC ( 3*32+18) /* "" Lfence synchronizes RDTSC */
#define X86_FEATURE_ACC_POWER	( 3*32+19) /* AMD Accumulated Power Mechanism */
#define X86_FEATURE_NOPL	( 3*32+20) /* The NOPL (0F 1F) instructions */
#define X86_FEATURE_ALWAYS	( 3*32+21) /* "" Always-present feature */
#define X86_FEATURE_XTOPOLOGY	( 3*32+22) /* cpu topology enum extensions */
#define X86_FEATURE_TSC_RELIABLE ( 3*32+23) /* TSC is known to be reliable */
#define X86_FEATURE_NONSTOP_TSC	( 3*32+24) /* TSC does not stop in C states */
/* free, was #define X86_FEATURE_CLFLUSH_MONITOR ( 3*32+25) * "" clflush reqd with monitor */
#define X86_FEATURE_EXTD_APICID	( 3*32+26) /* has extended APICID (8 bits) */
#define X86_FEATURE_AMD_DCM     ( 3*32+27) /* multi-node processor */
#define X86_FEATURE_APERFMPERF	( 3*32+28) /* APERFMPERF */
#define X86_FEATURE_EAGER_FPU	( 3*32+29) /* "eagerfpu" Non lazy FPU restore */
#define X86_FEATURE_NONSTOP_TSC_S3 ( 3*32+30) /* TSC doesn't stop in S3 state */
#define X86_FEATURE_MCE_RECOVERY ( 3*32+31) /* cpu has recoverable machine checks */

/* Intel-defined CPU features, CPUID level 0x00000001 (ecx), word 4 */
#define X86_FEATURE_XMM3	( 4*32+ 0) /* "pni" SSE-3 */
#define X86_FEATURE_PCLMULQDQ	( 4*32+ 1) /* PCLMULQDQ instruction */
#define X86_FEATURE_DTES64	( 4*32+ 2) /* 64-bit Debug Store */
#define X86_FEATURE_MWAIT	( 4*32+ 3) /* "monitor" Monitor/Mwait support */
#define X86_FEATURE_DSCPL	( 4*32+ 4) /* "ds_cpl" CPL Qual. Debug Store */
#define X86_FEATURE_VMX		( 4*32+ 5) /* Hardware virtualization */
#define X86_FEATURE_SMX		( 4*32+ 6) /* Safer mode */
#define X86_FEATURE_EST		( 4*32+ 7) /* Enhanced SpeedStep */
#define X86_FEATURE_TM2		( 4*32+ 8) /* Thermal Monitor 2 */
#define X86_FEATURE_SSSE3	( 4*32+ 9) /* Supplemental SSE-3 */
#define X86_FEATURE_CID		( 4*32+10) /* Context ID */
#define X86_FEATURE_SDBG	( 4*32+11) /* Silicon Debug */
#define X86_FEATURE_FMA		( 4*32+12) /* Fused multiply-add */
#define X86_FEATURE_CX16	( 4*32+13) /* CMPXCHG16B */
#define X86_FEATURE_XTPR	( 4*32+14) /* Send Task Priority Messages */
#define X86_FEATURE_PDCM	( 4*32+15) /* Performance Capabilities */
#define X86_FEATURE_PCID	( 4*32+17) /* Process Context Identifiers */
#define X86_FEATURE_DCA		( 4*32+18) /* Direct Cache Access */
#define X86_FEATURE_XMM4_1	( 4*32+19) /* "sse4_1" SSE-4.1 */
#define X86_FEATURE_XMM4_2	( 4*32+20) /* "sse4_2" SSE-4.2 */
#define X86_FEATURE_X2APIC	( 4*32+21) /* x2APIC */
#define X86_FEATURE_MOVBE	( 4*32+22) /* MOVBE instruction */
#define X86_FEATURE_POPCNT      ( 4*32+23) /* POPCNT instruction */
#define X86_FEATURE_TSC_DEADLINE_TIMER	( 4*32+24) /* Tsc deadline timer */
#define X86_FEATURE_AES		( 4*32+25) /* AES instructions */
#define X86_FEATURE_XSAVE	( 4*32+26) /* XSAVE/XRSTOR/XSETBV/XGETBV */
#define X86_FEATURE_OSXSAVE	( 4*32+27) /* "" XSAVE enabled in the OS */
#define X86_FEATURE_AVX		( 4*32+28) /* Advanced Vector Extensions */
#define X86_FEATURE_F16C	( 4*32+29) /* 16-bit fp conversions */
#define X86_FEATURE_RDRAND	( 4*32+30) /* The RDRAND instruction */
#define X86_FEATURE_HYPERVISOR	( 4*32+31) /* Running on a hypervisor */

/* More extended AMD flags: CPUID level 0x80000001, ecx, word 6 */
#define X86_FEATURE_LAHF_LM	( 6*32+ 0) /* LAHF/SAHF in long mode */
#define X86_FEATURE_CMP_LEGACY	( 6*32+ 1) /* If yes HyperThreading not valid */
#define X86_FEATURE_SVM		( 6*32+ 2) /* Secure virtual machine */
#define X86_FEATURE_EXTAPIC	( 6*32+ 3) /* Extended APIC space */
#define X86_FEATURE_CR8_LEGACY	( 6*32+ 4) /* CR8 in 32-bit mode */
#define X86_FEATURE_ABM		( 6*32+ 5) /* Advanced bit manipulation */
#define X86_FEATURE_SSE4A	( 6*32+ 6) /* SSE-4A */
#define X86_FEATURE_MISALIGNSSE ( 6*32+ 7) /* Misaligned SSE mode */
#define X86_FEATURE_3DNOWPREFETCH ( 6*32+ 8) /* 3DNow prefetch instructions */
#define X86_FEATURE_OSVW	( 6*32+ 9) /* OS Visible Workaround */
#define X86_FEATURE_IBS		( 6*32+10) /* Instruction Based Sampling */
#define X86_FEATURE_XOP		( 6*32+11) /* extended AVX instructions */
#define X86_FEATURE_SKINIT	( 6*32+12) /* SKINIT/STGI instructions */
#define X86_FEATURE_WDT		( 6*32+13) /* Watchdog timer */
#define X86_FEATURE_LWP		( 6*32+15) /* Light Weight Profiling */
#define X86_FEATURE_FMA4	( 6*32+16) /* 4 operands MAC instructions */
#define X86_FEATURE_TCE		( 6*32+17) /* translation cache extension */
#define X86_FEATURE_NODEID_MSR	( 6*32+19) /* NodeId MSR */
#define X86_FEATURE_TBM		( 6*32+21) /* trailing bit manipulations */
#define X86_FEATURE_TOPOEXT	( 6*32+22) /* topology extensions CPUID leafs */
#define X86_FEATURE_PERFCTR_CORE ( 6*32+23) /* core performance counter extensions */
#define X86_FEATURE_PERFCTR_NB  ( 6*32+24) /* NB performance counter extensions */
#define X86_FEATURE_BPEXT	(6*32+26) /* data breakpoint extension */
#define X86_FEATURE_PTSC	( 6*32+27) /* performance time-stamp counter */
#define X86_FEATURE_PERFCTR_L2	( 6*32+28) /* L2 performance counter extensions */
#define X86_FEATURE_MWAITX	( 6*32+29) /* MWAIT extension (MONITORX/MWAITX) */

/*
 * Auxiliary flags: Linux defined - For features scattered in various
 * CPUID levels like 0x6, 0xA etc, word 7.
 *
 * Reuse free bits when adding new feature flags!
 */
#define X86_FEATURE_CPB		( 7*32+ 2) /* AMD Core Performance Boost */
#define X86_FEATURE_EPB		( 7*32+ 3) /* IA32_ENERGY_PERF_BIAS support */

#define X86_FEATURE_HW_PSTATE	( 7*32+ 8) /* AMD HW-PState */
#define X86_FEATURE_PROC_FEEDBACK ( 7*32+ 9) /* AMD ProcFeedbackInterface */

#define X86_FEATURE_INTEL_PT	( 7*32+15) /* Intel Processor Trace */

 /* Intel-defined CPU features, CPUID level 0x00000007:0 (ebx), word 9 */
#define X86_FEATURE_FSGSBASE	( 9*32+ 0) /* {RD/WR}{FS/GS}BASE instructions*/
#define X86_FEATURE_TSC_ADJUST	( 9*32+ 1) /* TSC adjustment MSR 0x3b */
#define X86_FEATURE_BMI1	( 9*32+ 3) /* 1st group bit manipulation extensions */
#define X86_FEATURE_HLE		( 9*32+ 4) /* Hardware Lock Elision */
#define X86_FEATURE_AVX2	( 9*32+ 5) /* AVX2 instructions */
#define X86_FEATURE_SMEP	( 9*32+ 7) /* Supervisor Mode Execution Protection */
#define X86_FEATURE_BMI2	( 9*32+ 8) /* 2nd group bit manipulation extensions */
#define X86_FEATURE_ERMS	( 9*32+ 9) /* Enhanced REP MOVSB/STOSB */
#define X86_FEATURE_INVPCID	( 9*32+10) /* Invalidate Processor Context ID */
#define X86_FEATURE_RTM		( 9*32+11) /* Restricted Transactional Memory */
#define X86_FEATURE_CQM		( 9*32+12) /* Cache QoS Monitoring */
#define X86_FEATURE_MPX		( 9*32+14) /* Memory Protection Extension */
#define X86_FEATURE_AVX512F	( 9*32+16) /* AVX-512 Foundation */
#define X86_FEATURE_AVX512DQ	( 9*32+17) /* AVX-512 DQ (Double/Quad granular) Instructions */
#define X86_FEATURE_RDSEED	( 9*32+18) /* The RDSEED instruction */
#define X86_FEATURE_ADX		( 9*32+19) /* The ADCX and ADOX instructions */
#define X86_FEATURE_SMAP	( 9*32+20) /* Supervisor Mode Access Prevention */
#define X86_FEATURE_CLFLUSHOPT	( 9*32+23) /* CLFLUSHOPT instruction */
#define X86_FEATURE_CLWB	( 9*32+24) /* CLWB instruction */
#define X86_FEATURE_AVX512PF	( 9*32+26) /* AVX-512 Prefetch */
#define X86_FEATURE_AVX512ER	( 9*32+27) /* AVX-512 Exponential and Reciprocal */
#define X86_FEATURE_AVX512CD	( 9*32+28) /* AVX-512 Conflict Detection */
#define X86_FEATURE_SHA_NI	( 9*32+29) /* SHA1/SHA256 Instruction Extensions */
#define X86_FEATURE_AVX512BW	( 9*32+30) /* AVX-512 BW (Byte/Word granular) Instructions */
#define X86_FEATURE_AVX512VL	( 9*32+31) /* AVX-512 VL (128/256 Vector Length) Extensions */

/* Extended state features, CPUID level 0x0000000d:1 (eax), word 10 */
#define X86_FEATURE_XSAVEOPT	(10*32+ 0) /* XSAVEOPT */
#define X86_FEATURE_XSAVEC	(10*32+ 1) /* XSAVEC */
#define X86_FEATURE_XGETBV1	(10*32+ 2) /* XGETBV with ECX = 1 */
#define X86_FEATURE_XSAVES	(10*32+ 3) /* XSAVES/XRSTORS */

/* Intel-defined CPU QoS Sub-leaf, CPUID level 0x0000000F:0 (edx), word 11 */
#define X86_FEATURE_CQM_LLC	(11*32+ 1) /* LLC QoS if 1 */

/* Intel-defined CPU QoS Sub-leaf, CPUID level 0x0000000F:1 (edx), word 12 */
#define X86_FEATURE_CQM_OCCUP_LLC (12*32+ 0) /* LLC occupancy monitoring if 1 */
#define X86_FEATURE_CQM_MBM_TOTAL (12*32+ 1) /* LLC Total MBM monitoring */
#define X86_FEATURE_CQM_MBM_LOCAL (12*32+ 2) /* LLC Local MBM monitoring */

/* AMD-defined CPU features, CPUID level 0x80000008 (ebx), word 13 */
#define X86_FEATURE_CLZERO	(13*32+0) /* CLZERO instruction */
#define X86_FEATURE_IRPERF	(13*32+1) /* Instructions Retired Count */

/* Thermal and Power Management Leaf, CPUID level 0x00000006 (eax), word 14 */
#define X86_FEATURE_DTHERM	(14*32+ 0) /* Digital Thermal Sensor */
#define X86_FEATURE_IDA		(14*32+ 1) /* Intel Dynamic Acceleration */
#define X86_FEATURE_ARAT	(14*32+ 2) /* Always Running APIC Timer */
#define X86_FEATURE_PLN		(14*32+ 4) /* Intel Power Limit Notification */
#define X86_FEATURE_PTS		(14*32+ 6) /* Intel Package Thermal Status */
#define X86_FEATURE_HWP		(14*32+ 7) /* Intel Hardware P-states */
#define X86_FEATURE_HWP_NOTIFY	(14*32+ 8) /* HWP Notification */
#define X86_FEATURE_HWP_ACT_WINDOW (14*32+ 9) /* HWP Activity Window */
#define X86_FEATURE_HWP_EPP	(14*32+10) /* HWP Energy Perf. Preference */
#define X86_FEATURE_HWP_PKG_REQ (14*32+11) /* HWP Package Level Request */

/* AMD SVM Feature Identification, CPUID level 0x8000000a (edx), word 15 */
#define X86_FEATURE_NPT		(15*32+ 0) /* Nested Page Table support */
#define X86_FEATURE_LBRV	(15*32+ 1) /* LBR Virtualization support */
#define X86_FEATURE_SVML	(15*32+ 2) /* "svm_lock" SVM locking MSR */
#define X86_FEATURE_NRIPS	(15*32+ 3) /* "nrip_save" SVM next_rip save */
#define X86_FEATURE_TSCRATEMSR  (15*32+ 4) /* "tsc_scale" TSC scaling support */
#define X86_FEATURE_VMCBCLEAN   (15*32+ 5) /* "vmcb_clean" VMCB clean bits support */
#define X86_FEATURE_FLUSHBYASID (15*32+ 6) /* flush-by-ASID support */
#define X86_FEATURE_DECODEASSISTS (15*32+ 7) /* Decode Assists support */
#define X86_FEATURE_PAUSEFILTER (15*32+10) /* filtered pause intercept */
#define X86_FEATURE_PFTHRESHOLD (15*32+12) /* pause filter threshold */
#define X86_FEATURE_AVIC	(15*32+13) /* Virtual Interrupt Controller */

/* Intel-defined CPU features, CPUID level 0x00000007:0 (ecx), word 16 */
#define X86_FEATURE_PKU		(16*32+ 3) /* Protection Keys for Userspace */
#define X86_FEATURE_OSPKE	(16*32+ 4) /* OS Protection Keys Enable */

/* AMD-defined CPU features, CPUID level 0x80000007 (ebx), word 17 */
#define X86_FEATURE_OVERFLOW_RECOV (17*32+0) /* MCA overflow recovery support */
#define X86_FEATURE_SUCCOR	(17*32+1) /* Uncorrectable error containment and recovery */
#define X86_FEATURE_SMCA	(17*32+3) /* Scalable MCA */
#endif

/* x86-64 specific MSRs */
#define MSR_EFER		0xc0000080 /* extended feature register */
#define MSR_STAR		0xc0000081 /* legacy mode SYSCALL target */
#define MSR_LSTAR		0xc0000082 /* long mode SYSCALL target */
#define MSR_CSTAR		0xc0000083 /* compat mode SYSCALL target */
#define MSR_SYSCALL_MASK	0xc0000084 /* EFLAGS mask for syscall */
#define MSR_FS_BASE		0xc0000100 /* 64bit FS base */
#define MSR_GS_BASE		0xc0000101 /* 64bit GS base */
#define MSR_KERNEL_GS_BASE	0xc0000102 /* SwapGS GS shadow */
#define MSR_TSC_AUX		0xc0000103 /* Auxiliary TSC */

#ifndef __NetBSD__
/* EFER bits: */
#define _EFER_SCE		0  /* SYSCALL/SYSRET */
#define _EFER_LME		8  /* Long mode enable */
#define _EFER_LMA		10 /* Long mode active (read-only) */
#define _EFER_NX		11 /* No execute enable */
#define _EFER_SVME		12 /* Enable virtualization */
#define _EFER_LMSLE		13 /* Long Mode Segment Limit Enable */
#define _EFER_FFXSR		14 /* Enable Fast FXSAVE/FXRSTOR */

#define EFER_SCE		(1<<_EFER_SCE)
#define EFER_LME		(1<<_EFER_LME)
#define EFER_LMA		(1<<_EFER_LMA)
#define EFER_NX			(1<<_EFER_NX)
#define EFER_SVME		(1<<_EFER_SVME)
#define EFER_LMSLE		(1<<_EFER_LMSLE)
#define EFER_FFXSR		(1<<_EFER_FFXSR)
#endif

/* Intel MSRs. Some also available on other CPUs */
#define MSR_IA32_PERFCTR0		0x000000c1
#define MSR_IA32_PERFCTR1		0x000000c2

#ifndef MSR_FSB_FREQ
#define MSR_FSB_FREQ			0x000000cd
#endif

#define MSR_PLATFORM_INFO		0x000000ce

#ifndef MSR_MTRRcap
#define MSR_MTRRcap			0x000000fe
#endif

#define MSR_MTRR_PHYS_BASE		0x00000200
#define MSR_MTRR_PHYS_MASK		0x00000201
#define MSR_IA32_BBL_CR_CTL		0x00000119
#define MSR_IA32_BBL_CR_CTL3		0x0000011e

#define MSR_IA32_SYSENTER_CS		0x00000174
#define MSR_IA32_SYSENTER_ESP		0x00000175
#define MSR_IA32_SYSENTER_EIP		0x00000176

#define MSR_LBR_SELECT			0x000001c8
#define MSR_LBR_TOS			0x000001c9
#define MSR_LBR_NHM_FROM		0x00000680
#define MSR_LBR_NHM_TO			0x000006c0
#define MSR_LBR_CORE_FROM		0x00000040
#define MSR_LBR_CORE_TO			0x00000060

#define MSR_LBR_INFO_0			0x00000dc0 /* ... 0xddf for _31 */
#define LBR_INFO_MISPRED		BIT_ULL(63)
#define LBR_INFO_IN_TX			BIT_ULL(62)
#define LBR_INFO_ABORT			BIT_ULL(61)
#define LBR_INFO_CYCLES			0xffff

#ifndef __NetBSD__
#define MSR_MTRRfix64K_00000		0x00000250
#define MSR_MTRRfix16K_80000		0x00000258
#define MSR_MTRRfix16K_A0000		0x00000259
#define MSR_MTRRfix4K_C0000		0x00000268
#define MSR_MTRRfix4K_C8000		0x00000269
#define MSR_MTRRfix4K_D0000		0x0000026a
#define MSR_MTRRfix4K_D8000		0x0000026b
#define MSR_MTRRfix4K_E0000		0x0000026c
#define MSR_MTRRfix4K_E8000		0x0000026d
#define MSR_MTRRfix4K_F0000		0x0000026e
#define MSR_MTRRfix4K_F8000		0x0000026f
#define MSR_MTRRdefType			0x000002ff
#endif

#define MSR_IA32_CR_PAT			0x00000277

#define MSR_IA32_DEBUGCTLMSR		0x000001d9
#define MSR_IA32_LASTBRANCHFROMIP	0x000001db
#define MSR_IA32_LASTBRANCHTOIP		0x000001dc
#define MSR_IA32_LASTINTFROMIP		0x000001dd
#define MSR_IA32_LASTINTTOIP		0x000001de

/* DEBUGCTLMSR bits (others vary by model): */
#define DEBUGCTLMSR_LBR			(1UL <<  0) /* last branch recording */
#define DEBUGCTLMSR_BTF			(1UL <<  1) /* single-step on branches */
#define DEBUGCTLMSR_TR			(1UL <<  6)
#define DEBUGCTLMSR_BTS			(1UL <<  7)
#define DEBUGCTLMSR_BTINT		(1UL <<  8)
#define DEBUGCTLMSR_BTS_OFF_OS		(1UL <<  9)
#define DEBUGCTLMSR_BTS_OFF_USR		(1UL << 10)
#define DEBUGCTLMSR_FREEZE_LBRS_ON_PMI	(1UL << 11)

/* Hardware P state interface */
#define MSR_PPERF			0x0000064e
#define MSR_PERF_LIMIT_REASONS		0x0000064f
#define MSR_PM_ENABLE			0x00000770
#define MSR_HWP_CAPABILITIES		0x00000771
#define MSR_HWP_REQUEST_PKG		0x00000772
#define MSR_HWP_INTERRUPT		0x00000773
#define MSR_HWP_REQUEST 		0x00000774
#define MSR_HWP_STATUS			0x00000777

#define MSR_IA32_FS_BASE		0xC0000100
#define MSR_IA32_GS_BASE		0xC0000101
#define MSR_IA32_KERNEL_GS_BASE		0xC0000102
#define MSR_IA32_TSC_AUX		0xC0000103

/* Intel defined MSRs. */
#define MSR_IA32_TSC			0x00000010
#define MSR_SMI_COUNT			0x00000034
#define MSR_IA32_FEATURE_CONTROL        0x0000003a
#define MSR_IA32_TSC_ADJUST             0x0000003b
#define MSR_IA32_BNDCFGS		0x00000d90

#define MSR_IA32_XSS			0x00000da0

#define FEATURE_CONTROL_LOCKED				(1<<0)
#define FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX	(1<<1)
#define FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX	(1<<2)
#define FEATURE_CONTROL_LMCE				(1<<20)

#define MSR_IA32_APICBASE		0x0000001b
#define MSR_IA32_APICBASE_BSP		(1<<8)
#define MSR_IA32_APICBASE_X2APIC	(1<<10)
#define MSR_IA32_APICBASE_ENABLE	(1<<11)
#define MSR_IA32_APICBASE_BASE		(0xfffff<<12)
#define MSR_IA32_TSCDEADLINE		0x000006e0
#define MSR_IA32_SMM_MONITOR_CTL	0x0000009b
#define MSR_IA32_SMBASE			0x0000009e
#define MSR_IA32_TSC_DEADLINE		0x000006E0

/* Intel VT MSRs */
#define MSR_IA32_VMX_BASIC              0x00000480
#define MSR_IA32_VMX_PINBASED_CTLS      0x00000481
#define MSR_IA32_VMX_PROCBASED_CTLS     0x00000482
#define MSR_IA32_VMX_EXIT_CTLS          0x00000483
#define MSR_IA32_VMX_ENTRY_CTLS         0x00000484
#define MSR_IA32_VMX_MISC               0x00000485
#define MSR_IA32_VMX_CR0_FIXED0         0x00000486
#define MSR_IA32_VMX_CR0_FIXED1         0x00000487
#define MSR_IA32_VMX_CR4_FIXED0         0x00000488
#define MSR_IA32_VMX_CR4_FIXED1         0x00000489
#define MSR_IA32_VMX_VMCS_ENUM          0x0000048a
#define MSR_IA32_VMX_PROCBASED_CTLS2    0x0000048b
#define MSR_IA32_VMX_EPT_VPID_CAP       0x0000048c
#define MSR_IA32_VMX_TRUE_PINBASED_CTLS  0x0000048d
#define MSR_IA32_VMX_TRUE_PROCBASED_CTLS 0x0000048e
#define MSR_IA32_VMX_TRUE_EXIT_CTLS      0x0000048f
#define MSR_IA32_VMX_TRUE_ENTRY_CTLS     0x00000490
#define MSR_IA32_VMX_VMFUNC             0x00000491

/* VMX_BASIC bits and bitmasks */
#define VMX_BASIC_VMCS_SIZE_SHIFT	32
#define VMX_BASIC_TRUE_CTLS		(1ULL << 55)
#define VMX_BASIC_64		0x0001000000000000LLU
#define VMX_BASIC_MEM_TYPE_SHIFT	50
#define VMX_BASIC_MEM_TYPE_MASK	0x003c000000000000LLU
#define VMX_BASIC_MEM_TYPE_WB	6LLU
#define VMX_BASIC_INOUT		0x0040000000000000LLU

/* MSR_IA32_VMX_MISC bits */
#define MSR_IA32_VMX_MISC_VMWRITE_SHADOW_RO_FIELDS (1ULL << 29)
#define MSR_IA32_VMX_MISC_PREEMPTION_TIMER_SCALE   0x1F

/* AMD-V MSRs */
#define MSR_VM_CR                       0xc0010114
#define MSR_VM_IGNNE                    0xc0010115
#define MSR_VM_HSAVE_PA                 0xc0010117

#ifndef _MSC_VER
#ifndef __cli
#define __cli			__asm("cli" ::: "cc")
#endif
#ifndef __sti
#define __sti			__asm("sti" ::: "cc")
#endif
#define __return_addr()		__builtin_return_address(0)
#ifndef __linux__
#define cpu_relax()		__asm __volatile("pause\n\t" ::: "memory")
#define barrier()		__asm __volatile("" ::: "memory")
#define smp_mb()		__asm __volatile("mfence" ::: "memory")
#define smp_rmb()		__asm __volatile("lfence" ::: "memory")
#define smp_wmb()		__asm __volatile("sfence" ::: "memory")
#endif
#else
#define __cli()			_disable()
#define __sti()			_enable()
#define __return_addr()		_ReturnAddress()
#define cpu_relax()		_mm_pause()
#define barrier()		_ReadWriteBarrier()
#define smp_mb()		_mm_mfence()
#define smp_rmb()		_mm_lfence()
#define smp_wmb() 		_mm_sfence()
#endif

#ifndef _MSC_VER
#define __writedr(dr, val)				\
	__asm __volatile("movq %0, %%dr" #dr		\
			 :: "r" ((val)))

#define __readdr(dr) __extension__ ({			\
	unsigned long long val;				\
	__asm __volatile("movq %%dr" #dr ", %0"		\
			 : "=r" (val));			\
	val;						\
})

static inline void _xsetbv(u32 index, u64 value)
{
	u32 eax = value;
	u32 edx = value >> 32;

	__asm __volatile(".byte 0x0f,0x01,0xd1"
			 :: "a" (eax), "d" (edx), "c" (index));
}

static inline void __cpuidex(int *ret, int func, int subf)
{
	__asm __volatile("xchgq %%rbx, %%rdi\n\t"
			 "cpuid\n\t"
			 "xchgq %%rbx, %%rdi\n\t"
			 : "=a" (ret[0]), "=D" (ret[1]), "=c" (ret[2]), "=d"(ret[3])
			 : "a" (func), "c" (subf));
}

static inline u64 __lar(u64 sel)
{
	u64 ar;
	__asm __volatile("lar %1, %0"
			 : "=r" (ar)
			 : "r" (sel));
	return ar;
}

/* MinGW might not have it, and Linux most likely won't.  */
#ifndef __writecr2
#define __writecr2(cr2)		\
	__asm("mov %0, %%cr2" :: "r"(cr2))
#endif

#ifdef __linux__
static inline u64 __rdtsc(void)
{
	u32 eax, edx;
	__asm __volatile("rdtsc"
			 : "=a" (eax), "=d" (edx));
	return (u64)eax | (u64)edx << 32;
}

static inline u64 __rdtscp(u32 *cpu)
{
	u32 eax, edx;
	__asm __volatile("rdtscp"
			 : "=c" (*cpu), "=a" (eax), "=d" (edx));
	return (u64)eax | (u64)edx << 32;
}

static inline unsigned long long __readmsr(u32 msr)
{
	unsigned long long x;
	rdmsrl(msr, x);
	return x;
}

static inline void __writemsr(u32 msr, u64 val)
{
	wrmsr(msr, (u32)val, (u32)(val >> 32));
}

#define __readcr0 	read_cr0
#define __writecr0	write_cr0

#define __readcr3 	read_cr3
#define __writecr3	write_cr3

#define __readcr2()	__extension__ ({	\
	uintptr_t cr2;				\
	__asm("mov %%cr2, %0" : "=r" (cr2));	\
	cr2;	\
})
#define __readcr4() 	__extension__ ({	\
	uintptr_t cr4;				\
	__asm("mov %%cr4, %0" : "=r" (cr4));	\
	cr4;	\
})
#define __writecr4(cr4)				\
	__asm("mov %0, %%cr4" :: "r"(cr4))

#define __inbytestring(port, addr, count)	insb(port, addr, count)
#define __inwordstring(port, addr, count)	insw(port, addr, count)
#define __indwordstring(port, addr, count)	insl(port, addr, count)

#define __inbyte(port)				inb(port)
#define __inword(port)				inw(port)
#define __indword(port)				inl(port)

#define __outbytestring(port, addr, count)	outsb(port, addr, count)
#define __outwordstring(port, addr, count)	outsw(port, addr, count)
#define __outdwordstring(port, addr, count)	outsl(port, addr, count)

#define __outbyte(port, value)			outb(value, port)
#define __outword(port, value)			outw(value, port)
#define __outdword(port, value)			outl(value, port)
#endif

/* Generic GCC (MinGW and native Linux).  */
#define __ud2()		__asm __volatile("ud2")
#define __wbinvd()	__asm __volatile("wbinvd")
#define __invd() 	__asm __volatile("invd")
#define __halt() 	__asm __volatile("hlt")
#define __invlpg(addr)	__asm __volatile("invlpg (%0)" :: "r" (addr) : "memory")
#define __readeflags()	({							\
	u64 rflags;								\
	__asm __volatile("pushfq\n\tpopq %0" : "=r" (rflags));			\
	rflags;									\
})

#define DEFINE_SEL_READER(name, instr)				\
	static inline u16 name(void)				\
	{							\
		u16 tmp;					\
		__asm __volatile(instr : "=r" (tmp));		\
		return tmp;					\
	}

DEFINE_SEL_READER(__sldt, "sldt %0")
DEFINE_SEL_READER(__str, "str %0")
DEFINE_SEL_READER(__readcs, "movw %%cs, %0")
DEFINE_SEL_READER(__readds, "movw %%ds, %0")
DEFINE_SEL_READER(__reades, "movw %%es, %0")
DEFINE_SEL_READER(__readfs, "movw %%fs, %0")
DEFINE_SEL_READER(__readgs, "movw %%gs, %0")
DEFINE_SEL_READER(__readss, "movw %%ss, %0")
#else
/* MSVC: those are defined in vmx.asm (MASM)  */
extern void __lgdt(const void *);
extern void __sgdt(void *);

extern void __lldt(u16);
extern u16 __sldt(void);

extern void __ltr(u16);
extern u16 __str(void);

extern u16 __reades(void);
extern u16 __readcs(void);
extern u16 __readss(void);
extern u16 __readds(void);
extern u16 __readfs(void);
extern u16 __readgs(void);

extern uintptr_t __lar(uintptr_t);
extern void __invd(void);
extern void __writecr2(uintptr_t);
#endif

/*
 * Helper functions for segmentation (IDT, GDT, LDT, etc.)
 */
#ifndef __linux__
#ifndef __NetBSD__
#include <pshpack1.h>
#endif
#endif
struct gdtr {
	u16 limit;
	uintptr_t base;
} __packed;

struct tss {
	u32 reserved1;
	u64 sp0;
	u64 sp1;
	u64 sp2;
	u64 reserved2;
	u64 ist[7];
	u32 reserved3;
	u32 reserved4;
	u16 reserved5;
	u16 io_bitmap_base;
	u64 io_bitmap[PAGE_SIZE*2 + 1];
} __packed;

typedef union __packed {
	u64 i;
	struct {
		u16 lo;
		u16 sel;
		u8 ist : 3;
		u8 zero : 5;
		u8 type : 5;
		u8 dpl : 2;
		u8 p : 1;
		u16 mid;
	};
} kidt_entry_t;

struct kidt_entry64 {
	kidt_entry_t e32;
	u32 hi;
	u32 zero;
} __packed;
#ifndef __linux__
#ifndef __NetBSD__
#include <poppack.h>
#endif
#endif

#ifndef _MSC_VER
#define __sidt(idt)	__asm __volatile("sidt %0" : "=m" (*idt));
#define __lidt(idt)	__asm __volatile("lidt %0" :: "m" (*idt));
#define __sgdt(gdt)	__asm __volatile("sgdt %0" : "=m" (*gdt));
#define __lgdt(gdt)	__asm __volatile("lgdt %0" :: "m" (*gdt));
#endif

#define LOW_U16_U64(x) ((u64)(x) & 0xFFFF)
#define MID_U16_U64(x) (((u64)(x) >> 16) & 0xFFFF)
#define HIGH_U32_U64(x) ((u64)(x) >> 32)

static inline bool idte_present(const struct kidt_entry64 *e)
{
	return e->e32.p;
}

static inline u16 idte_sel(const struct kidt_entry64 *e)
{
	return e->e32.sel;
}

static inline u64 idte_addr(const struct kidt_entry64 *e)
{
	const kidt_entry_t *e32 = &e->e32;
	return (u64)e->hi << 32 | ((u32)e32->mid << 16 | e32->lo);
}

static inline struct kidt_entry64 *idt_entry(uintptr_t base, unsigned n)
{
	struct kidt_entry64 *table = (struct kidt_entry64 *)base;
	return &table[n];
}

static inline void pack_entry(struct kidt_entry64 *entry, u16 selector, uintptr_t addr)
{
	kidt_entry_t *e = &entry->e32;

	entry->hi = HIGH_U32_U64(addr);
	entry->zero = 0;

	e->lo = LOW_U16_U64(addr);
	e->sel = selector;
	e->ist = 0;
	e->zero = 0;
	e->type = GATE_INTERRUPT;
	e->dpl = 0;
	e->p = 1;
	e->mid = MID_U16_U64(addr);
}

static inline void put_entry(uintptr_t base, unsigned n, struct kidt_entry64 *entry)
{
	memcpy(idt_entry(base, n), entry, sizeof(*entry));
}

static inline void set_intr_gate(unsigned n, u16 selector, uintptr_t base, uintptr_t addr)
{
	struct kidt_entry64 entry;
	pack_entry(&entry, selector, addr);
	put_entry(base, n, &entry);
}

typedef union __packed {
	u64 all;
	struct {
		u64 limit_low : 16;
		u64 base_low : 16;
		u64 base_mid : 8;
		u64 type : 4;
		u64 system : 1;
		u64 dpl : 2;
		u64 present : 1;
		u64 limit_high : 4;
		u64 avl : 1;
		u64 l : 1;
		u64 db : 1;
		u64 gran : 1;
		u64 base_high : 8;
	};
} segmentdesc_t;

typedef struct __packed {
	segmentdesc_t d32;
	u32 base_upper32;
	u32 reserved;
} segmentdesc64_t;

static inline segmentdesc_t *segment_desc(uintptr_t gdt, u16 sel)
{
	return (segmentdesc_t *)(gdt + (sel & ~3));
}

static uintptr_t segment_desc_base(segmentdesc_t *desc)
{
	uintptr_t base = desc->base_high << 24 | desc->base_mid << 16 | desc->base_low;
	if (!desc->system)
		base |= (uintptr_t)(((segmentdesc64_t *)desc)->base_upper32) << 32;

	return base;
}

static uintptr_t __segmentbase(uintptr_t gdt, u16 sel)
{
	if (!sel)
		return 0;

	/* If it's an LDT segment, load the LDT  */
	if (sel & 4) {
		segmentdesc_t *ldt = segment_desc(gdt, __sldt());
		uintptr_t ldt_base = segment_desc_base(ldt);
		return segment_desc_base(segment_desc(ldt_base, sel));
	}

	return segment_desc_base(segment_desc(gdt, sel));
}

#ifndef __linux__
/*
 *
 * Taken from the Linux kernel
 * 	arch/x86/include/asm/apicdef.h
 *
 * Constants for various Intel APICs. (local APIC, IOAPIC, etc.)
 *
 * Alan Cox <Alan.Cox@linux.org>, 1995.
 * Ingo Molnar <mingo@redhat.com>, 1999, 2000
 *
 * Some modifications for reading/writing into LAPIC/IOAPIC:
 *	Ahmed Samy <asamy@protonmail.com>
 */
#define IO_APIC_DEFAULT_PHYS_BASE	0xfec00000
#define	APIC_DEFAULT_PHYS_BASE		0xfee00000

/*
 * This is the IO-APIC register space as specified
 * by Intel docs:
 */
#define IO_APIC_SLOT_SIZE		1024

#define	APIC_ID		0x20

#define	APIC_LVR	0x30
#define	APIC_LVR_MASK		0xFF00FF
#define	APIC_LVR_DIRECTED_EOI	(1 << 24)
#define	GET_APIC_VERSION(x)	((x) & 0xFFu)
#define	GET_APIC_MAXLVT(x)	(((x) >> 16) & 0xFFu)
#define	APIC_INTEGRATED(x)	(1)
#define	APIC_XAPIC(x)		((x) >= 0x14)
#define	APIC_EXT_SPACE(x)	((x) & 0x80000000)
#define	APIC_TASKPRI	0x80
#define	APIC_TPRI_MASK		0xFFu
#define	APIC_ARBPRI	0x90
#define	APIC_ARBPRI_MASK	0xFFu
#define	APIC_PROCPRI	0xA0
#define	APIC_EOI	0xB0
#define	APIC_EOI_ACK		0x0 /* Docs say 0 for future compat. */
#define	APIC_RRR	0xC0
#define	APIC_LDR	0xD0
#define	APIC_LDR_MASK		(0xFFu << 24)
#define	GET_APIC_LOGICAL_ID(x)	(((x) >> 24) & 0xFFu)
#define	SET_APIC_LOGICAL_ID(x)	(((x) << 24))
#define	APIC_ALL_CPUS		0xFFu
#define	APIC_DFR	0xE0
#define	APIC_DFR_CLUSTER		0x0FFFFFFFul
#define	APIC_DFR_FLAT			0xFFFFFFFFul
#define	APIC_SPIV	0xF0
#define	APIC_SPIV_DIRECTED_EOI		(1 << 12)
#define	APIC_SPIV_FOCUS_DISABLED	(1 << 9)
#define	APIC_SPIV_APIC_ENABLED		(1 << 8)
#define	APIC_ISR	0x100
#define	APIC_ISR_NR     0x8     /* Number of 32 bit ISR registers. */
#define	APIC_TMR	0x180
#define	APIC_IRR	0x200
#define	APIC_ESR	0x280
#define	APIC_ESR_SEND_CS	0x00001
#define	APIC_ESR_RECV_CS	0x00002
#define	APIC_ESR_SEND_ACC	0x00004
#define	APIC_ESR_RECV_ACC	0x00008
#define	APIC_ESR_SENDILL	0x00020
#define	APIC_ESR_RECVILL	0x00040
#define	APIC_ESR_ILLREGA	0x00080
#define APIC_LVTCMCI	0x2f0
#define	APIC_ICR	0x300
#define	APIC_DEST_SELF		0x40000
#define	APIC_DEST_ALLINC	0x80000
#define	APIC_DEST_ALLBUT	0xC0000
#define	APIC_ICR_RR_MASK	0x30000
#define	APIC_ICR_RR_INVALID	0x00000
#define	APIC_ICR_RR_INPROG	0x10000
#define	APIC_ICR_RR_VALID	0x20000
#define	APIC_INT_LEVELTRIG	0x08000
#define	APIC_INT_ASSERT		0x04000
#define	APIC_ICR_BUSY		0x01000
#define	APIC_DEST_LOGICAL	0x00800
#define	APIC_DEST_PHYSICAL	0x00000
#define	APIC_DM_FIXED		0x00000
#define	APIC_DM_FIXED_MASK	0x00700
#define	APIC_DM_LOWEST		0x00100
#define	APIC_DM_SMI		0x00200
#define	APIC_DM_REMRD		0x00300
#define	APIC_DM_NMI		0x00400
#define	APIC_DM_INIT		0x00500
#define	APIC_DM_STARTUP		0x00600
#define	APIC_DM_EXTINT		0x00700
#define	APIC_VECTOR_MASK	0x000FF
#define	APIC_ICR2	0x310
#define	GET_APIC_DEST_FIELD(x)	(((x) >> 24) & 0xFF)
#define	SET_APIC_DEST_FIELD(x)	((x) << 24)
#define	APIC_LVTT	0x320
#define	APIC_LVTTHMR	0x330
#define	APIC_LVTPC	0x340
#define	APIC_LVT0	0x350
#define	APIC_LVT_TIMER_BASE_MASK	(0x3 << 18)
#define	GET_APIC_TIMER_BASE(x)		(((x) >> 18) & 0x3)
#define	SET_APIC_TIMER_BASE(x)		(((x) << 18))
#define	APIC_TIMER_BASE_CLKIN		0x0
#define	APIC_TIMER_BASE_TMBASE		0x1
#define	APIC_TIMER_BASE_DIV		0x2
#define	APIC_LVT_TIMER_ONESHOT		(0 << 17)
#define	APIC_LVT_TIMER_PERIODIC		(1 << 17)
#define	APIC_LVT_TIMER_TSCDEADLINE	(2 << 17)
#define	APIC_LVT_MASKED			(1 << 16)
#define	APIC_LVT_LEVEL_TRIGGER		(1 << 15)
#define	APIC_LVT_REMOTE_IRR		(1 << 14)
#define	APIC_INPUT_POLARITY		(1 << 13)
#define	APIC_SEND_PENDING		(1 << 12)
#define	APIC_MODE_MASK			0x700
#define	GET_APIC_DELIVERY_MODE(x)	(((x) >> 8) & 0x7)
#define	SET_APIC_DELIVERY_MODE(x, y)	(((x) & ~0x700) | ((y) << 8))
#define	APIC_MODE_FIXED		0x0
#define	APIC_MODE_NMI		0x4
#define	APIC_MODE_EXTINT	0x7
#define	APIC_LVT1	0x360
#define	APIC_LVTERR	0x370
#define	APIC_TMICT	0x380
#define	APIC_TMCCT	0x390
#define	APIC_TDCR	0x3E0
#define APIC_SELF_IPI	0x3F0
#define	APIC_TDR_DIV_TMBASE	(1 << 2)
#define	APIC_TDR_DIV_1		0xB
#define	APIC_TDR_DIV_2		0x0
#define	APIC_TDR_DIV_4		0x1
#define	APIC_TDR_DIV_8		0x2
#define	APIC_TDR_DIV_16		0x3
#define	APIC_TDR_DIV_32		0x8
#define	APIC_TDR_DIV_64		0x9
#define	APIC_TDR_DIV_128	0xA
#define	APIC_EFEAT	0x400
#define	APIC_ECTRL	0x410
#define APIC_EILVTn(n)	(0x500 + 0x10 * n)
#define	APIC_EILVT_NR_AMD_K8	1	/* # of extended interrupts */
#define	APIC_EILVT_NR_AMD_10H	4
#define	APIC_EILVT_NR_MAX	APIC_EILVT_NR_AMD_10H
#define	APIC_EILVT_LVTOFF(x)	(((x) >> 4) & 0xF)
#define	APIC_EILVT_MSG_FIX	0x0
#define	APIC_EILVT_MSG_SMI	0x2
#define	APIC_EILVT_MSG_NMI	0x4
#define	APIC_EILVT_MSG_EXT	0x7
#define	APIC_EILVT_MASKED	(1 << 16)

#define MAX_IO_APICS 128
#define MAX_LOCAL_APIC 32768

/*
 * All x86-64 systems are xAPIC compatible.
 * In the following, "apicid" is a physical APIC ID.
 */
#define XAPIC_DEST_CPUS_SHIFT	4
#define XAPIC_DEST_CPUS_MASK	((1u << XAPIC_DEST_CPUS_SHIFT) - 1)
#define XAPIC_DEST_CLUSTER_MASK	(XAPIC_DEST_CPUS_MASK << XAPIC_DEST_CPUS_SHIFT)
#define APIC_CLUSTER(apicid)	((apicid) & XAPIC_DEST_CLUSTER_MASK)
#define APIC_CLUSTERID(apicid)	(APIC_CLUSTER(apicid) >> XAPIC_DEST_CPUS_SHIFT)
#define APIC_CPUID(apicid)	((apicid) & XAPIC_DEST_CPUS_MASK)

enum ioapic_dest {
	IOAPIC_FIXED_DEST = 0,
	IOAPIC_LOWEST_PRIO = 1,
	IOAPIC_DEST_SMI = 2,
	IOAPIC_DEST_RSVD = 3,
	IOAPIC_DEST_NMI = 4,
	IOAPIC_DEST_INIT = 5,
	IOAPIC_DEST_RSVD2 = 6,
	IOAPIC_DEST_EXTINTR = 7,
};
#endif

#ifdef __NetBSD__
#include <machine/cpufunc.h>
#endif

static inline bool lapic_in_kernel(void)
{
#ifdef __NetBSD__
	rdmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_ENABLE;
#else
	return __readmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_ENABLE;
#endif
}

#ifndef __linux__
static inline bool x2apic_enabled(void)
{
#ifdef __NetBSD__
	rdmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_X2APIC;
#else
	return __readmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_X2APIC;
#endif
}
#endif

static inline bool lapic_is_bsp(void)
{
#ifdef __NetBSD__
	rdmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_BSP;
#else
	return __readmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_BSP;
#endif
}

static inline u64 lapic_base_phys(void)
{
#ifdef __NetBSD__
	rdmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_BASE;
#else
	return __readmsr(MSR_IA32_APICBASE) & MSR_IA32_APICBASE_BASE;
#endif
}

#define IOAPIC_REGSEL		0x00
#define IOAPIC_WIN		0x10

#define IOAPIC_ID		0x00
#define IOAPIC_VER		0x01
#define IOAPIC_ARB		0x02
#define IOAPIC_REDTBL		0x10

static inline void ioapic_write(u64 base, u8 reg, u32 val)
{
	*(volatile u32 *)(base + IOAPIC_REGSEL) = reg;
	*(volatile u32 *)(base + IOAPIC_WIN) = val;
}

static inline u32 ioapic_read(u64 base, u8 reg)
{
	*(volatile u32 *)(base + IOAPIC_REGSEL) = reg;
	return *(volatile u32 *)(base + IOAPIC_WIN);
}

static inline void ioapic_set_entry(u64 base, u8 index, u64 data)
{
	ioapic_write(base, IOAPIC_REDTBL + index * 2, (u32)data);
	ioapic_write(base, IOAPIC_REDTBL + index * 2 + 1, data >> 32);
}

static inline u32 ioapic_count(u64 base)
{
	return ((ioapic_read(base, IOAPIC_VER) >> 16) & 0xFF) + 1;
}

static inline void __lapic_write(u64 base, u32 reg, u32 val)
{
	*(volatile u32 *)(base + reg) = val;
}

static inline void __lapic_write64(u64 base, u32 reg, u64 val)
{
	/* XXX:  x2APIC case:  see exit.c (vcpu_handle_msr_write).  */
	*(volatile u64 *)(base + reg) = val;
}

static inline u32 __lapic_read(u64 base, u32 reg)
{
	return *(volatile u32 *)(base + reg);
}

static inline u64 __lapic_read64(u64 base, u32 reg)
{
	return *(volatile u64 *)(base + reg);
}

static inline bool lapic_write(u32 reg, u32 val)
{
	void *base = mm_remap_iomem(lapic_base_phys(), PAGE_SIZE);
	if (!base)
		return false;

	__lapic_write((u64)base, reg, val);
	mm_unmap_iomem(base, PAGE_SIZE);
	return true;
}

static inline u32 lapic_read(u32 reg)
{
	void *base = mm_remap_iomem(lapic_base_phys(), PAGE_SIZE);
	if (!base)
		return false;

	u32 val = __lapic_read((u64)base, reg);
	mm_unmap_iomem(base, PAGE_SIZE);
	return val;
}

static inline void lapic_send_self_ipi(u8 vector)
{
	u32 icr = vector | APIC_DEST_SELF | APIC_DM_FIXED | APIC_DEST_PHYSICAL;
	lapic_write(APIC_ICR, icr);
}

#endif
