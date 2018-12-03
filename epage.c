/*
 * ksm - a really simple and fast x64 hypervisor
 * Copyright (C) 2016, 2017 Ahmed Samy <asamy@protonmail.com>
 *
 * Executable page hooking, see comments down below for more
 * information.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef EPAGE_HOOK
#ifdef __linux__
#include <linux/kernel.h>
#else
#include <ntddk.h>
#endif

#include "ksm.h"
#include "percpu.h"

/*!
 * To use this interface, call ksm_hook_epage() on the target function,
 * e.g.:
 * \code
 *	ksm_hook_epage(MmMapIoSpace, hkMmMapIoSpace);
 * \endcode
 *
 * And for original function call:
 * \code
 *	vcpu_vmfunc(EPTP_NORMAL, 0);
 *	void *ret = MmMapIoSpace(x, y, z);
 *	vcpu_vmfunc(EPTP_EXHOOK, 0);
 *	return ret;
 * \endcode
 */
void ksm_handle_epage(struct vcpu *vcpu, struct epage_info *epage)
{
	struct ept *ept = &vcpu->ept;
	struct ksm *k = vcpu_to_ksm(vcpu);
	int mt = ept_memory_type(k, epage->dpa);

	/* Called from vmcall (exit.c)  */
	ept_alloc_page(EPT4(ept, EPTP_EXHOOK), EPT_ACCESS_EXEC, mt, epage->dpa, epage->cpa);
	ept_alloc_page(EPT4(ept, EPTP_RWHOOK), EPT_ACCESS_RW, mt, epage->dpa, epage->dpa);
	ept_alloc_page(EPT4(ept, EPTP_NORMAL), EPT_ACCESS_ALL, mt, epage->dpa, epage->dpa);

	cpu_invvpid(k, epage->origin);
	cpu_invept(k, epage->dpa, EPTP(ept, vcpu_eptp_idx(vcpu)));
}

void ksm_handle_epage_ve(struct epage_info *epage, struct ept_ve_around *ve)
{
	/* called from an EPT violation  */
	if (ve->info->exit & EPT_ACCESS_RW)
		ve->eptp_next = EPTP_RWHOOK;
	else
		ve->eptp_next = EPTP_EXHOOK;
}

#ifndef __linux__
#include <pshpack1.h>
#endif
struct trampoline {
	u8 push;
	u32 lo;
	u32 mov;
	u32 hi;
	u32 ret;
} __packed;
#ifndef __linux__
#include <poppack.h>
#endif

static void epage_init_trampoline(struct trampoline *trampo, u64 to)
{
	// push lo
	// mov dword ptr [rsp + 0x4], hi
	// ret
	trampo->push = 0x68;
	trampo->lo = (u32)to;
	trampo->mov = 0x042444C7;
	trampo->hi = to >> 32;
	trampo->ret = 0xC3;
}

static DEFINE_DPC(__do_hook_page, __vmx_vmcall, HCALL_HOOK, ctx);
static DEFINE_DPC(__do_unhook_page, __vmx_vmcall, HCALL_UNHOOK, ctx);

/*
 * Note!!!
 * This function is not very robust, e.g. pages that are not
 * physically contiguous will cause havoc, on the Linux kernel
 * this can be a problem when hooking kernel pages, specfiically
 * module pages as those are allocated using vmalloc() and are not
 * physically contiguous, so be careful.
 *
 * On windows, kernel pages are always physically contiguous unless they are
 * in the PAGE section, so this will handle most cases.
 *
 * On windows, you can lock pages using:
 * \code
 *	PMDL mdl = IoAllocateMdl(original, PAGE_SIZE, FALSE, FALSE, NULL);
 *	MmProbeAndLockPages(mdl, KernelMode, IoReadAccess);
 * \endcode
 *
 * Then unlock in unhook:
 * \code
 *	MmUnlockPages(mdl);
 *	IoFreeMdl(mdl);
 * \endcode
 *
 *	Notes on hooking out-of-kernel pages (e.g. userspace pages or similar):
 *	
 *	When hooking a userspace specific function, you should first attach to that
 *	specific process (if not already), to make sure that the current CR3 is
 *	updated.  Also do note that userspace pages tend to be paged out all the
 *	time, so the above notes also apply.
 *
 * Do also note the inline-code provided above is not tested, but should work.
 */
struct epage_info *ksm_prepare_epage(void *original, void *redirect, bool *exist)
{
	struct epage_info *epage;
	u8 *code_page;
	void *aligned = (void *)page_align(original);
	uintptr_t code_start = (uintptr_t)original - (uintptr_t)aligned;
	struct trampoline trampo;

	BUG_ON(!ksm);
	epage_init_trampoline(&trampo, (uintptr_t)redirect);
	epage = ksm_find_epage(ksm, __pa(original));
	if (epage) {
		/*
		 * Hooking another function in same page.
		 *
		 * Simply just overwrite the start of the
		 * function to the trampoline...
		 */
		*exist = true;
		code_page = epage->c_va;
		memcpy(code_page + code_start, &trampo, sizeof(trampo));
		return epage;
	}

	epage = mm_alloc_pool(sizeof(*epage));
	if (!epage)
		return NULL;

	code_page = mm_alloc_page();
	if (!code_page) {
		mm_free_pool(epage, sizeof(*epage));
		return NULL;
	}

	memcpy(code_page, aligned, PAGE_SIZE);
	memcpy(code_page + code_start, &trampo, sizeof(trampo));

	epage->c_va = code_page;
	epage->cpa = __pa(code_page);
	epage->dpa = __pa(original);
	epage->origin = (u64)aligned;
	return epage;
}

int ksm_hook_epage_on_cpu(struct epage_info *epage, int cpu)
{
	CALL_DPC_ON_CPU(cpu, __do_hook_page, return -1, epage);
	return DPC_RET();
}

int ksm_hook_epage(void *original, void *redirect)
{
	struct epage_info *epage;
	bool exist = false;

	epage = ksm_prepare_epage(original, redirect, &exist);
	if (!epage)
		return ERR_NOMEM;

	if (exist)
		return 0;

	CALL_DPC(__do_hook_page, epage);
	spin_lock(&ksm->epage_lock);
	list_add_tail(&epage->link, &ksm->epage_list);
	spin_unlock(&ksm->epage_lock);
	return DPC_RET();
}

static inline void ksm_free_epage(struct epage_info *epage)
{
	mm_free_page(epage->c_va);
	mm_free_pool(epage, sizeof(*epage));
}

int __ksm_unhook_epage(struct epage_info *epage)
{
	CALL_DPC(__do_unhook_page, (void *)epage->dpa);
	spin_lock(&ksm->epage_lock);
	list_del(&epage->link);
	spin_unlock(&ksm->epage_lock);
	ksm_free_epage(epage);
	return DPC_RET();
}

int ksm_unhook_epage(struct ksm *k, void *va)
{
	struct epage_info *epage = ksm_find_epage(k, __pa(va));
	if (!epage)
		return ERR_NOTH;

	return __ksm_unhook_epage(epage);
}

struct epage_info *ksm_find_epage(struct ksm *k, uintptr_t gpa)
{
	struct epage_info *epage = NULL;
	struct epage_info *ret = NULL;

	spin_lock(&k->epage_lock);
	list_for_each_entry(epage, &k->epage_list, link)
	{
		if (epage->dpa >> PAGE_SHIFT == gpa >> PAGE_SHIFT) {
			ret = epage;
			break;
		}
	}

	spin_unlock(&k->epage_lock);
	return ret;
}

int ksm_epage_init(struct ksm *k)
{
	KSM_INIT_LIST_HEAD(&k->epage_list);
	spin_lock_init(&k->epage_lock);
	return 0;
}

int ksm_epage_exit(struct ksm *k)
{
	struct epage_info *epage = NULL;
	struct epage_info *next;

	list_for_each_entry_safe(epage, next, &k->epage_list, link)
		ksm_free_epage(epage);

	return 0;
}

#endif
