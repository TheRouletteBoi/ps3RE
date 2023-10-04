
#include "common/utils/thread.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/syscalls.hpp"
#include <sys/prx.h>
#include <stdarg.h>

#pragma diag_suppress 77
SYS_MODULE_INFO( a, 0, 1, 1);
SYS_MODULE_START(module_start);
SYS_MODULE_STOP(module_stop);

bool contentPrinted = 0;
char out[0x10000];

thread g_entryThread;

// VSH 4.84 dex
auto malloc = reinterpret_cast<void* (*)(size_t size)>(0x6FBA00);
auto memcpy = reinterpret_cast<void* (*)(void* to, const void* from, size_t size)>(0x6FC128);
auto printf = reinterpret_cast<int(*)(const char* fmt, ...)>(0x6FC348);
auto vprintf = reinterpret_cast<int(*)(const char* fmt, va_list args)>(0x6FC398);
auto snprintf = reinterpret_cast<int(*)(char* buffer, int size, const char* fmt, ...)>(0x6FC358);
auto vsnprintf = reinterpret_cast<int(*)(char* buffer, int size, const char* fmt, va_list args)>(0x6FC1F8);
auto strncat = reinterpret_cast<char* (*)(char* buffer, const char* str, int size)>(0x6FC300);

uintptr_t CCAPIBaseAddress = 0;

void DebugLog(const char* fmt, ...)
{
    static char buffer[1024];

    va_list args;
    va_start(args, fmt);

    if (contentPrinted)
    {
        vprintf(fmt, args);
    }
    else
    {
        vsnprintf(buffer, 1024, fmt, args);
        strncat(out, buffer, 0x4000);
    }

    va_end(args);
}

void DumpHex(const void* data, size_t size) {
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i) {
        DebugLog("%02X ", ((unsigned char*)data)[i]);
        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char*)data)[i];
        }
        else {
            ascii[i % 16] = '.';
        }
        if ((i + 1) % 8 == 0 || i + 1 == size) {
            DebugLog(" ");
            if ((i + 1) % 16 == 0) {
                DebugLog("|  %s \n", ascii);
            }
            else if (i + 1 == size) {
                ascii[(i + 1) % 16] = '\0';
                if ((i + 1) % 16 <= 8) {
                    DebugLog(" ");
                }
                for (j = (i + 1) % 16; j < 16; ++j) {
                    DebugLog("   ");
                }
                DebugLog("|  %s \n", ascii);
            }
        }
    }
}

#define SetupHook(ret_type, name, args) \
    __attribute__((naked, noinline)) ret_type name##_stub args { asm("nop;nop;nop;nop;nop;nop;nop;nop;"); } \
    ret_type name##_hook args

SetupHook(int, sub_CC4C, (int a1, int a2, int a3))
{
    return sub_CC4C_stub(a1, a2, a3);
}

SetupHook(void, DecryptString_Internal, (const char* out, const char* in, int size, char key))
{
    DecryptString_Internal_stub(out, in, size, key);

    auto addr = in - CCAPIBaseAddress;

    if (addr != (const char*)0x0005C0C8)
    {
        DebugLog("[String]: 0x%08X\n", in - CCAPIBaseAddress);
        DumpHex(out, size);
    }
        //DebugLog("[String]: 0x%08X - '%s'\n", in - CCAPIBaseAddress, out);
}

void RegisterDump(HookContext_t& ctx)
{
    DebugLog("Register Info:\n");
    DebugLog("     LR: 0x%016llX     CR: 0x%016llX\n", ctx.lr, ctx.cr);
    DebugLog("    CTR: 0x%016llX\n\n", ctx.ctr);
    DebugLog("  GPR 0: 0x%016llX  GPR 1: 0x%016llX\n", ctx.r0, ctx.r1);
    DebugLog("  GPR 2: 0x%016llX  GPR 3: 0x%016llX\n", ctx.r2, ctx.r3);
    DebugLog("  GPR 4: 0x%016llX  GPR 5: 0x%016llX\n", ctx.r4, ctx.r5);
    DebugLog("  GPR 6: 0x%016llX  GPR 7: 0x%016llX\n", ctx.r6, ctx.r7);
    DebugLog("  GPR 8: 0x%016llX  GPR 9: 0x%016llX\n", ctx.r8, ctx.r9);
    DebugLog("  GPR10: 0x%016llX  GPR11: 0x%016llX\n", ctx.r10, ctx.r11);
    DebugLog("  GPR12: 0x%016llX  GPR13: 0x%016llX\n", ctx.r12, ctx.r13);
    DebugLog("  GPR14: 0x%016llX  GPR15: 0x%016llX\n", ctx.r14, ctx.r15);
    DebugLog("  GPR16: 0x%016llX  GPR17: 0x%016llX\n", ctx.r16, ctx.r17);
    DebugLog("  GPR18: 0x%016llX  GPR19: 0x%016llX\n", ctx.r18, ctx.r19);
    DebugLog("  GPR20: 0x%016llX  GPR21: 0x%016llX\n", ctx.r20, ctx.r21);
    DebugLog("  GPR22: 0x%016llX  GPR23: 0x%016llX\n", ctx.r22, ctx.r23);
    DebugLog("  GPR24: 0x%016llX  GPR25: 0x%016llX\n", ctx.r24, ctx.r25);
    DebugLog("  GPR26: 0x%016llX  GPR27: 0x%016llX\n", ctx.r26, ctx.r27);
    DebugLog("  GPR28: 0x%016llX  GPR29: 0x%016llX\n", ctx.r28, ctx.r29);
    DebugLog("  GPR30: 0x%016llX  GPR31: 0x%016llX\n\n", ctx.r30, ctx.r31);
    DebugLog("    XER: 0x%016llX\n\n", ctx.xer);
}

extern "C" 
{
    int module_start(size_t args, void* argp)
    {
        // Load ccapi but do not start it immediately in order to patch it
        sys_prx_id_t CCAPI = sys_prx_load_module("/dev_flash/sys/internal/ccapi.sprx", 0, 0);

        // Bad module handle
        if (CCAPI <= 0)
        {
            return -1;
        }

        // Get module informations
        static sys_prx_segment_info_t segments[10];
        static char filename[SYS_PRX_MODULE_FILENAME_SIZE];

        sys_prx_module_info_t info;
        info.size = sizeof(sys_prx_module_info_t);
        info.segments = segments;
        info.segments_num = sizeof(segments) / sizeof(sys_prx_segment_info_t);
        info.filename = filename;
        info.filename_size = sizeof(filename);

        sys_prx_get_module_info(CCAPI, 0, &info);

        // First segment is .text
        CCAPIBaseAddress = info.segments[0].base;

        hook::start(CCAPIBaseAddress + 0x3F124, 20);

        // Patch AreWeSysAudio() to return true, else ccapi will call exit_process2 with args (0, 0, 0)
        uint32_t returnTrue[2]
        {
            0x38600001, // li %r3, 1
            0x4E800020  // blr
        };
        hook::copy(CCAPIBaseAddress + 0x3C8A8, returnTrue, 2);

        hook::jump(CCAPIBaseAddress + 0x3108, *(uintptr_t*)(DecryptString_Internal_hook), *(uintptr_t*)(DecryptString_Internal_stub));

        hook::inject(CCAPIBaseAddress + 0x37054, [](HookContext_t& ctx)
            {
                // LOAD:0000000000037054 7C 09 03 A6                 mtctr     r0            # Move to count register
                ctx.ctr = ctx.r0;

                RegisterDump(ctx);

                int fd;
                if (syscall(801, "/dev_hdd0/ccapi_main_payload_relocated_to_0x80000000007F0000.bin", 1101, &fd, 0666, 0, 0) == 0)
                {
                    auto FindU64 = [](uint64_t* buffer, size_t size, uint64_t value) -> uint64_t*
                    {
                        while (size)
                        {
                            if (*buffer == value)
                                return buffer;

                            buffer++;
                            size--;
                        }
                        return 0;
                    };

                    // Relocate payload to 0x80000000007F0000
                    uint64_t residence = 0x80000000007F0000;
                    uint64_t* payload = (uint64_t*)malloc((size_t)ctx.r6);
                    memcpy(payload, (const void*)(uintptr_t)(ctx.r5), (size_t)ctx.r6);

                    uint64_t* relocationStart = FindU64(payload, ctx.r6 / sizeof(uint64_t), 0x88ED42CD01541111);
                    uint64_t* relocationEnd = FindU64(payload, ctx.r6 / sizeof(uint64_t), 0xAADEFFEE97123654);

                    DebugLog("reloc start: 0x%X\n", (relocationStart - payload) * sizeof(uint64_t));
                    DebugLog("reloc end: 0x%X\n", (relocationEnd - payload) * sizeof(uint64_t));

                    while (relocationStart < relocationEnd)
                    {
                        if ((*relocationStart & 0xFFFFFFFF00000000) == 0xEBADADDC00000000)
                            *relocationStart = (*relocationStart & 0xFFFFFFFF) + residence;

                        relocationStart++;
                    }

                    uint64_t nwrite;
                    syscall(803, fd, payload, ctx.r6, &nwrite);
                    syscall(804, fd);
                    DebugLog("content dumped\n");
                }


                // 0xA280 relocation start
                // 0xAF28 relocation end

            }, true);

        // Start ccapi
        int modres = 0;
        sys_prx_start_module(CCAPI, args, argp, &modres, 0, 0);

        g_entryThread = thread([]
            {
                DebugLog("CCAPIBaseAddress - 0x%X\n", CCAPIBaseAddress);
                sys_timer_sleep(30);
                printf("Printing out buffer\n");
                printf(out);
                contentPrinted = true;
            });

        return 0;
    }

    int module_stop(size_t args, void* argp)
    {
        return 0;
    }
}
