typedef unsigned int uint32_t;

void __attribute__((optimize("O0"))) _start(void)
{
    char buf[4];

    // Bit 214 (Disable ASLR)
    *(uint32_t *)buf = *(uint32_t *)0xE0062218; // DIP base + 0x18
    buf[2] |= 1 << 6;                           // Byte 2, bit 6

    *(uint32_t *)0xE0030000 = *(uint32_t *)0xE0062200;
    *(uint32_t *)0xE0030004 = *(uint32_t *)0xE0062204;
    *(uint32_t *)0xE0030008 = *(uint32_t *)0xE0062208;
    *(uint32_t *)0xE003000C = *(uint32_t *)0xE006220C;
    *(uint32_t *)0xE0030010 = *(uint32_t *)0xE0062210;
    *(uint32_t *)0xE0030014 = *(uint32_t *)0xE0062214;
    *(uint32_t *)0xE0030018 = *(uint32_t *)buf;

    // Bit 228 (Enable hardware break/watch point)
    *(uint32_t *)buf = *(uint32_t *)0xE006221C; // DIP base + 0x1C
    buf[0] |= 1 << 4;                           // Byte 0, bit 4
    *(uint32_t *)0xE003001C = *(uint32_t *)buf;

    // Trigger write to DIP switches keyslot
    *(uint32_t *)0xE0030020 = 0x00000510;

    return;
}