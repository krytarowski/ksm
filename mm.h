/*
 * ksm - a really simple and fast x64 hypervisor
 * Copyright (C) 2016, 2017 Ahmed Samy <asamy@protonmail.com>
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
#ifndef __MM_H
#define __MM_H

#ifdef __linux__
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/sched.h>
#endif

#ifdef __NetBSD__
#define __HAVE_DIRECT_MAP
#include <sys/param.h>
#include <sys/types.h>
#include <sys/kmem.h>
#include <uvm/uvm.h>
#include <machine/pmap.h>
#endif

#define PGD_SHIFT_P		39
#define PUD_SHIFT_P		30
#define PMD_SHIFT_P		21
#define PTE_SHIFT_P		12

#define VA_BITS			48
#define VA_MASK			((1ULL << VA_BITS) - 1)
#define VA_SHIFT		16

#ifndef PTX_MASK
#define PTX_MASK		0x1FF
#endif
#define PUD_MASK_P		0x3FFFF
#define PMD_MASK_P		0x7FFFFFF
#define PTE_MASK_P		0xFFFFFFFFF

/*
 * The traditional page table management carries on, but 
 * the naming convention is different per-platform, so not
 * to be confused, here are the names:
 *
 *	Regular (AMD tables): PML4, PDPT, PDT, PT
 *	Linux tables:	 PML4, PGD, PUD, PMD
 *	Windows tables:	 PML4, PXE, PPE, PDE
 *
 * To sync this, we just use linux convention because it's more
 * convenient.
 */
#define PAGE_PRESENT		0x1	/* Present and locked in the lock position  */
#define PAGE_WRITE		0x2	/* Writable  */
#define PAGE_USER		0x4	/* User page  */
#define PAGE_WRITETHRU		0x8	/* Write through  */
#define PAGE_CACHEDISABLE	0x10	/* No caching  */
#define PAGE_ACCESSED		0x20	/* Processor: Set when accessed  */
#define PAGE_DIRTY		0x40	/* Processor: Set when wrote to  */
#define PAGE_LARGE		0x80	/* Large page  */
#define PAGE_GLOBAL		0x100	/* Global page, see CR4.PGE  */
#define PAGE_COPYONWRITE	0x200	/* CoW mapping  */
#define PAGE_PROTOTYPE		0x400	/* Linux: Used for I/O mapping  */
#define PAGE_TRANSIT		0x800	/* Linux: Hidden by kmemcheck  */
#define PAGE_PA_MASK		(0xFFFFFFFFFULL << PAGE_SHIFT)
#define PAGE_PA(page)		((page) & PAGE_PA_MASK)
#define PAGE_FN(page)		(((page) >> PTE_SHIFT_P) & PTE_MASK_P)
#define PAGE_PPA(pte)		(PAGE_PA(pte->pte))
#define PAGE_PFN(pte)		(PAGE_FN(pte->pte))
#define PAGE_NX			0x8000000000000000	/* No execute  */
#define PAGE_LPRESENT		(PAGE_PRESENT | PAGE_LARGE)

#define PGF_PRESENT		0x1	/* present fault  */
#define PGF_WRITE		0x2	/* write fault  */
#define PGF_SP			0x4	/* supervisor fault (SMEP, SMAP)  */
#define PGF_RSVD		0x8	/* reserved bit was set fault  */
#define PGF_FETCH		0x10	/* fetch fault  */
#define PGF_PK			0x20	/* Protection key fault  */
#define PGF_SGX			0x40	/* SGX induced fault  */

#define PGD_INDEX_P(addr)		(((addr) >> PGD_SHIFT_P) & PTX_MASK)
#define PUD_INDEX_P(addr)		(((addr) >> PUD_SHIFT_P) & PTX_MASK)
#define PMD_INDEX_P(addr)		(((addr) >> PMD_SHIFT_P) & PTX_MASK)
#define PTE_INDEX_P(addr)		(((addr) >> PTE_SHIFT_P) & PTX_MASK)

#ifndef __linux__
/* be in the same boat  */
typedef struct { unsigned long long pgd; } pgd_t;
typedef struct { unsigned long long pud; } pud_t;
typedef struct { unsigned long long pmd; } pmd_t;
typedef struct { unsigned long long pte; } pte_t;

/* Determined at runtime (on Windows 10 these are not static.)  */
extern uintptr_t pxe_base;
extern uintptr_t ppe_base;
extern uintptr_t pde_base;
extern uintptr_t pte_base;

#ifdef __NetBSD__
uintptr_t __pa(uintptr_t va)
{
    bool success;
    paddr_t pa;

    success = pmap_extract(pmap_kernel(), (vaddr_t)va, &pa);

    KASSERT(success);

    return pa;
}

uintptr_t __va(uintptr_t pa)
{
    return (uintptr_t)(PMAP_MAP_POOLPAGE(pa));
}
#else
#define __pa(va)	\
	MmGetPhysicalAddress((void *)(va)).QuadPart
#define __va(pa)	\
	(uintptr_t *)MmGetVirtualForPhysical((PHYSICAL_ADDRESS) { .QuadPart = (uintptr_t)(pa) })
#endif

#define pte_present(p)		((((pte_t *)(&(p)))->pte) & (PAGE_PRESENT | PAGE_GLOBAL))

static inline pgd_t *pgd_offset(uintptr_t cr3, uintptr_t va)
{
	return (pgd_t *)__va(PAGE_PA(cr3)) + PGD_INDEX_P(va);
}

static inline pud_t *pud_offset(pgd_t *pgd, uintptr_t va)
{
	return (pud_t *)__va(PAGE_PPA(pgd)) + PUD_INDEX_P(va);
}

static inline pmd_t *pmd_offset(pud_t *pud, uintptr_t va)
{
	return (pmd_t *)__va(PAGE_PPA(pud)) + PMD_INDEX_P(va);
}

static inline pte_t *pte_offset(pmd_t *pmd, uintptr_t va)
{
	return (pte_t *)__va(PAGE_PPA(pmd)) + PTE_INDEX_P(va);
}
#endif

#define pte_large(p)		((((pte_t *)(&(p)))->pte) & PAGE_LARGE)
#define page_align(addr)	((uintptr_t)(addr) & ~(PAGE_SIZE - 1))

static inline bool page_aligned(uintptr_t addr)
{
	return (addr & (PAGE_SIZE - 1)) == 0;
}

static inline size_t round_to_pages(size_t size)
{
	return (size >> PAGE_SHIFT) + ((size & (PAGE_SIZE - 1)) != 0);
}

static inline u16 addr_offset(uintptr_t addr)
{
	return addr & (PAGE_SIZE - 1);
}

static inline bool same_page(uintptr_t a1, uintptr_t a2)
{
	return page_align(a1) == page_align(a2);
}

static inline bool is_canonical_addr(u64 addr)
{
	return (s64)addr >> 47 == (s64)addr >> 63;
}

#ifdef __linux__
static inline pgd_t *va_to_pgd(uintptr_t va)
{
	return pgd_offset(current->mm, va);
}

static inline pud_t *va_to_pud(uintptr_t va)
{
	return pud_offset(va_to_pgd(va), va);
}

static inline pmd_t *va_to_pmd(uintptr_t va)
{
	return pmd_offset(va_to_pud(va), va);
}

static inline pte_t *va_to_pte(uintptr_t va)
{
	return pte_offset_kernel(va_to_pmd(va), va);
}

static inline pte_t *pte_from_cr3_va(uintptr_t cr3, uintptr_t va)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	pgd = pgd_offset(current->mm, va);
	if (pgd_none(*pgd) || pgd_bad(*pgd))
		return NULL;

	pud = pud_offset(pgd, va);
	if (pud_none(*pud) || pud_bad(*pud))
		return NULL;

	pmd = pmd_offset(pud, va);
	if (pmd_none(*pmd) || pmd_bad(*pmd))
		return NULL;

	if (pmd_large(*pmd))
		return (pte_t *)pmd;

	return pte_offset_kernel(pmd, va);
}

static inline void *mm_alloc_page(void)
{
	return (void *)get_zeroed_page(GFP_KERNEL | GFP_ATOMIC);
}

static inline void __mm_free_page(void *v)
{
	free_page((unsigned long)v);
}

static inline void *mm_alloc_pool(size_t size)
{
	return kmalloc(size, GFP_KERNEL | GFP_ATOMIC | __GFP_ZERO);
}

static inline void __mm_free_pool(void *v)
{
	kfree(v);
}

static inline bool mm_is_kernel_addr(void *va)
{
	return (uintptr_t)va >= PAGE_OFFSET;
}

extern void *mm_remap(u64 phys, size_t size);
extern void mm_unmap(void *addr, size_t size);
extern void *mm_remap_iomem(u64 phys, size_t size);
extern void mm_unmap_iomem(void *addr, size_t size);
extern void *kmap_virt(void *addr, size_t len, pgprot_t prot);
static inline void *kmap_exec(void *addr, size_t len)
{
	return kmap_virt(addr, len, PAGE_KERNEL_EXEC);
}

static inline void *kmap_write(void *addr, size_t len)
{
	return kmap_virt(addr, len, PAGE_KERNEL);
}
#else
/*
 * You can use the following functions for address translation in general
 * but if you're translating a userspace address, then either make sure
 * the cr3 is set to that userspace process, or use pgd_offset(), etc 
 * functions instead.
 */
static inline pgd_t *va_to_pgd(uintptr_t va)
{
	uintptr_t off = (va >> PGD_SHIFT_P) & PTX_MASK;
	return (pgd_t *)pxe_base + off;
}

static inline pud_t *va_to_pud(uintptr_t va)
{
	uintptr_t off = (va >> PUD_SHIFT_P) & PUD_MASK_P;
	return (pud_t *)ppe_base + off;
}

static inline pmd_t *va_to_pmd(uintptr_t va)
{
	uintptr_t off = (va >> PMD_SHIFT_P) & PMD_MASK_P;
	return (pmd_t *)pde_base + off;
}

static inline pte_t *va_to_pte(uintptr_t va)
{
	uintptr_t off = (va >> PTE_SHIFT_P) & PTE_MASK_P;
	return (pte_t *)pte_base + off;
}

static inline pte_t *pte_from_cr3_va(uintptr_t cr3, uintptr_t va)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	pgd = pgd_offset(cr3, va);
	if (!pte_present(*pgd))
		return NULL;

	pud = pud_offset(pgd, va);
	if (!pte_present(*pud))
		return NULL;

	pmd = pmd_offset(pud, va);
	if (!pte_present(*pmd))
		return NULL;

	if (pte_large(pmd))
		return (pte_t *)pmd;

	return pte_offset(pmd, va);
}

static inline void *mm_remap(u64 phys, size_t size)
{
	return MmMapIoSpace((PHYSICAL_ADDRESS) { .QuadPart = phys },
			    size, MmNonCached);
}

static inline void mm_unmap(void *addr, size_t size)
{
	return MmUnmapIoSpace(addr, size);
}

static inline void *mm_remap_iomem(u64 phys, size_t size)
{
	return mm_remap(phys, size);
}

static inline void mm_unmap_iomem(void *addr, size_t size)
{
	return mm_unmap(addr, size);
}

static inline void *mm_alloc_page(void)
{
	void *v = ExAllocatePoolWithTag(NonPagedPool, PAGE_SIZE, 0);
	if (v)
		memset(v, 0, PAGE_SIZE);

	return v;
}

static inline void __mm_free_page(void *v)
{
	ExFreePoolWithTag(v, 0);
}

static inline void *mm_alloc_pool(size_t size)
{
	void *v = ExAllocatePoolWithTag(NonPagedPool, size, 0);
	if (v)
		memset(v, 0, size);

	return v;
}

static inline void __mm_free_pool(void *v)
{
	ExFreePool(v);
}

static inline bool mm_is_kernel_addr(void *va)
{
	return va >= MmSystemRangeStart;
}
#endif

static inline void mm_free_pool(void *v, size_t size)
{
	memset(v, 0, size);
	__mm_free_pool(v);
}

static inline void mm_free_page(void *v)
{
	memset(v, 0, PAGE_SIZE);
	__mm_free_page(v);
}

static inline u64 va_to_pa(uintptr_t va)
{
	pte_t *pte = (pte_t *)va_to_pmd(va);
	if (!pte_large(*pte))
		pte = va_to_pte(va);

	if (!pte_present(*pte))
		return 0;

	return PAGE_PPA(pte) | addr_offset(va);
}

static inline void set_pte_flags(pte_t *pte, int flags)
{
	if (pte && (pte->pte & flags) != flags)
		pte->pte |= flags;
}

static inline void mark_pte_dirty(pte_t *pte)
{
	set_pte_flags(pte, PAGE_DIRTY);
}

static inline void mark_pte_accessed(pte_t *pte)
{
	set_pte_flags(pte, PAGE_ACCESSED);
}

struct pmem_range {
	u64 start;
	u64 end;
};

#define MAX_RANGES	32
extern int mm_cache_ram_ranges(struct pmem_range *ranges, int *count);

struct mtrr_range {
	bool fixed;
	u8 type;
	u64 start;
	u64 end;
};

#define MAX_VAR_MTRR		255
#define MAX_FIXED_MTRR		11*8
#define MAX_MTRR		MAX_VAR_MTRR + MAX_FIXED_MTRR
extern void mm_cache_mtrr_ranges(struct mtrr_range *ranges, int *count, u8 *def_type);

#endif
