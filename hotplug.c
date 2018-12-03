/*
 * ksm - a really simple and fast x64 hypervisor
 * Copyright (C) 2016, 2017 Ahmed Samy <asamy@protonmail.com>
 *
 * Public domain
 */
#ifdef __linux__
#include <linux/cpu.h>
#elif defined(__NetBSD__)
#include <sys/param.h>
#include <sys/types.h>
#else
#include <ntddk.h>
#endif

#include "ksm.h"
#include "compiler.h"

#ifdef __linux__
static inline void do_cpu(void *v)
{
	int(*f) (struct ksm *) = v;
	int ret = f(ksm);

	KSM_DEBUG("On CPU calling %d\n", ret);
}

static int ksm_hotplug_cpu(struct notifier_block *nfb, unsigned long action, void *hcpu)
{
	unsigned long cpu = (unsigned long)hcpu;

	KSM_DEBUG("CPU %ld action: %ld\n", cpu, action);
	switch (action) {
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
		get_online_cpus();
		if (cpu_online(cpu))
			smp_call_function_single(cpu, do_cpu, __ksm_init_cpu, 1);
		put_online_cpus();
		break;
	case CPU_DOWN_PREPARE:
	case CPU_DYING:
	case CPU_DYING_FROZEN:
		smp_call_function_single(cpu, do_cpu, __ksm_exit_cpu, 1);
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block cpu_notify = {
	.notifier_call = ksm_hotplug_cpu
};

int register_cpu_callback(void)
{
	register_hotcpu_notifier(&cpu_notify);
	return 0;
}

void unregister_cpu_callback(void)
{
	unregister_hotcpu_notifier(&cpu_notify);
}
#elif defined(__NetBSD__)
#else
static void *hotplug_cpu;

static void ksm_hotplug_cpu(void *ctx, PKE_PROCESSOR_CHANGE_NOTIFY_CONTEXT change_ctx, PNTSTATUS op_status)
{
	/* CPU Hotplug callback, a CPU just came online.  */
	GROUP_AFFINITY affinity;
	GROUP_AFFINITY prev;
	PPROCESSOR_NUMBER pnr;
	int status;

	if (change_ctx->State == KeProcessorAddCompleteNotify) {
		pnr = &change_ctx->ProcNumber;
		affinity.Group = pnr->Group;
		affinity.Mask = 1ULL << pnr->Number;
		KeSetSystemGroupAffinityThread(&affinity, &prev);

		KSM_DEBUG_RAW("New processor\n");
		status = __ksm_init_cpu(ksm);
		if (!NT_SUCCESS(status))
			*op_status = status;

		KeRevertToUserGroupAffinityThread(&prev);
	}
}

int register_cpu_callback(void)
{
	hotplug_cpu = KeRegisterProcessorChangeCallback(ksm_hotplug_cpu, NULL, 0);
	if (!hotplug_cpu)
		return STATUS_UNSUCCESSFUL;

	return 0;
}

void unregister_cpu_callback(void)
{
	KeDeregisterProcessorChangeCallback(hotplug_cpu);
}

#endif
