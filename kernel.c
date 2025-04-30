#include <stddef.h>
#include <stdint.h>
#include "../hooking.h"
#include "../plugins.h"

static int is_safe_mode(kbl_param_struct *kbl_param)
{
    uint32_t v;
    if (kbl_param->debug_flags[7] != 0xFF)
        return 1;
    v = kbl_param->boot_type_indicator_2 & 0x7F;
    if (v == 0xB || (v == 4 && kbl_param->resume_context_addr))
    {
        v = ~kbl_param->field_CC;
        if (((v >> 8) & 0x54) == 0x54 && (v & 0xC0) == 0)
            return 1;
        else
            return 0;
    }
    else if (v == 4)
        return 0;
    if (v == 0x1F || (uint32_t)(v - 0x18) <= 1)
        return 1;
    else
        return 0;
}

static int is_update_mode(kbl_param_struct *kbl_param)
{
    if (kbl_param->debug_flags[4] != 0xFF)
        return 1;
    else
        return 0;
}

static inline int skip_patches(kbl_param_struct *kbl_param)
{
    return is_safe_mode(kbl_param) || is_update_mode(kbl_param);
}

__attribute__((noinline)) static void patch_dip_bits(patch_args_struct *patch_args, const uint32_t *bit_numbers, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        uint32_t bit_num = bit_numbers[i];
        uint32_t offset = (bit_num >> 5) * 4;
        uint32_t bit = bit_num & 31;
        
        uint32_t *target = (uint32_t *)((uint8_t *)patch_args->kbl_param->dip_switches + offset);
        
        uint32_t mask = 1U << bit;
        uint32_t patch_value = *target | mask;
        patch_args->kbl_memcpy(target, &patch_value, sizeof(uint32_t));
    }
}

void _start() __attribute__((weak, alias("module_start")));
int module_start(uint32_t argc, void *args)
{
    patch_args_struct *patch_args = args;
    if (patch_args->this_version != PATCH_ARGS_VERSION)
        return -1;
    else if (skip_patches(patch_args->kbl_param))
        return 0;
    else if (CTRL_BUTTON_HELD(patch_args->ex_ctrl, CTRL_LEFT))
        return 0;

    const uint32_t bits_to_patch[] = {228};
    patch_dip_bits(patch_args, bits_to_patch, sizeof(bits_to_patch) / sizeof(bits_to_patch[0]));

    return 0;
}