#include "payload/payload.h"
#include <psp2kern/kernel/suspend.h>
#include <psp2kern/kernel/modulemgr.h>

uint32_t spl_exec_code(void *cbuf, uint32_t csize, uint32_t arg, int copy_cbuf);

static int sysevent_handler(int resume, int eventid, void *args, void *opt)
{
    (void)args;
    (void)opt;
    if ((resume) && (eventid == 0x100000))
        spl_exec_code(&payload, payload_len, 0, 1);
    return 0;
}

void _start() __attribute__((weak, alias("module_start")));
int module_start(void)
{

    if (spl_exec_code(&payload, payload_len, 0, 1) != 0)
        SCE_KERNEL_START_FAILED;
    ksceKernelRegisterSysEventHandler("setdip", sysevent_handler, NULL);

    return SCE_KERNEL_START_SUCCESS;
}