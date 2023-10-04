
#include "hooking.hpp"
#include "syscalls.hpp"

extern "C"
{
    __declspec(naked, noinline) void exec_handler()
    {
        asm
        (
            // Save r0
            "std       %r0, 0x70(%r1);"

            // Save r1 as r1 + 0x300 to get the hooked function stack address
            "addi      %r0, %r1, 0x300;"
            "std       %r0, 0x78(%r1);"

            // Save all gprs
            "std       %r4, 0x90(%r1);"
            "std       %r5, 0x98(%r1);"
            "std       %r6, 0xA0(%r1);"
            "std       %r7, 0xA8(%r1);"
            "std       %r8, 0xB0(%r1);"
            "std       %r9, 0xB8(%r1);"
            "std       %r10, 0xC0(%r1);"
            "std       %r11, 0xC8(%r1);"
            "std       %r12, 0xD0(%r1);"
            "std       %r13, 0xD8(%r1);"
            "std       %r14, 0xE0(%r1);"
            "std       %r15, 0xE8(%r1);"
            "std       %r16, 0xF0(%r1);"
            "std       %r17, 0xF8(%r1);"
            "std       %r18, 0x100(%r1);"
            "std       %r19, 0x108(%r1);"
            "std       %r20, 0x110(%r1);"
            "std       %r21, 0x118(%r1);"
            "std       %r22, 0x120(%r1);"
            "std       %r23, 0x128(%r1);"
            "std       %r24, 0x130(%r1);"
            "std       %r25, 0x138(%r1);"
            "std       %r26, 0x140(%r1);"
            "std       %r27, 0x148(%r1);"
            "std       %r28, 0x150(%r1);"
            "std       %r29, 0x158(%r1);"
            "std       %r30, 0x160(%r1);"
            "std       %r31, 0x168(%r1);"

            // Save all sprs
            "mfctr     %r0;"
            "std       %r0, 0x178(%r1);"
            "mfcr      %r0;"
            "std       %r0, 0x180(%r1);"
            "mfxer     %r0;"
            "std       %r0, 0x188(%r1);"

            // Save all fprs
            "stfd      %f0, 0x190(%r1);"
            "stfd      %f1, 0x198(%r1);"
            "stfd      %f2, 0x1A0(%r1);"
            "stfd      %f3, 0x1A8(%r1);"
            "stfd      %f4, 0x1B0(%r1);"
            "stfd      %f5, 0x1B8(%r1);"
            "stfd      %f6, 0x1C0(%r1);"
            "stfd      %f7, 0x1C8(%r1);"
            "stfd      %f8, 0x1D0(%r1);"
            "stfd      %f9, 0x1D8(%r1);"
            "stfd      %f10, 0x1E0(%r1);"
            "stfd      %f11, 0x1E8(%r1);"
            "stfd      %f12, 0x1F0(%r1);"
            "stfd      %f13, 0x1F8(%r1);"
            "stfd      %f14, 0x200(%r1);"
            "stfd      %f15, 0x208(%r1);"
            "stfd      %f16, 0x210(%r1);"
            "stfd      %f17, 0x218(%r1);"
            "stfd      %f18, 0x220(%r1);"
            "stfd      %f19, 0x228(%r1);"
            "stfd      %f20, 0x230(%r1);"
            "stfd      %f21, 0x238(%r1);"
            "stfd      %f22, 0x240(%r1);"
            "stfd      %f23, 0x248(%r1);"
            "stfd      %f24, 0x250(%r1);"
            "stfd      %f25, 0x258(%r1);"
            "stfd      %f26, 0x260(%r1);"
            "stfd      %f27, 0x268(%r1);"
            "stfd      %f28, 0x270(%r1);"
            "stfd      %f29, 0x278(%r1);"
            "stfd      %f30, 0x280(%r1);"
            "stfd      %f31, 0x288(%r1);"

            // Pass a pointer to the hook context as r3
            "addi      %r3, %r1, 0x70;"

            // Call our hook handler
            "bl        ._ZN4hook7handlerER13HookContext_t;"
            "nop;"

            // Restore sprs
            "ld        %r0, 0x170(%r1);"
            "mtlr      %r0;"
            "ld        %r0, 0x178(%r1);"
            "mtctr     %r0;"
            "ld        %r0, 0x180(%r1);"
            "mtcr      %r0;"
            "ld        %r0, 0x188(%r1);"
            "mtxer     %r0;"

            // Restore gprs
            "ld        %r0, 0x70(%r1);"
            "nop;"
            "ld        %r2, 0x80(%r1);"
            "ld        %r3, 0x88(%r1);"
            "ld        %r4, 0x90(%r1);"
            "ld        %r5, 0x98(%r1);"
            "ld        %r6, 0xA0(%r1);"
            "ld        %r7, 0xA8(%r1);"
            "ld        %r8, 0xB0(%r1);"
            "ld        %r9, 0xB8(%r1);"
            "ld        %r10, 0xC0(%r1);"
            "ld        %r11, 0xC8(%r1);"
            "ld        %r12, 0xD0(%r1);"
            "ld        %r13, 0xD8(%r1);"
            "ld        %r14, 0xE0(%r1);"
            "ld        %r15, 0xE8(%r1);"
            "ld        %r16, 0xF0(%r1);"
            "ld        %r17, 0xF8(%r1);"
            "ld        %r18, 0x100(%r1);"
            "ld        %r19, 0x108(%r1);"
            "ld        %r20, 0x110(%r1);"
            "ld        %r21, 0x118(%r1);"
            "ld        %r22, 0x120(%r1);"
            "ld        %r23, 0x128(%r1);"
            "ld        %r24, 0x130(%r1);"
            "ld        %r25, 0x138(%r1);"
            "ld        %r26, 0x140(%r1);"
            "ld        %r27, 0x148(%r1);"
            "ld        %r28, 0x150(%r1);"
            "ld        %r29, 0x158(%r1);"
            "ld        %r30, 0x160(%r1);"
            "ld        %r31, 0x168(%r1);"

            // Restore fprs
            "lfd       %f0, 0x190(%r1);"
            "lfd       %f1, 0x198(%r1);"
            "lfd       %f2, 0x1A0(%r1);"
            "lfd       %f3, 0x1A8(%r1);"
            "lfd       %f4, 0x1B0(%r1);"
            "lfd       %f5, 0x1B8(%r1);"
            "lfd       %f6, 0x1C0(%r1);"
            "lfd       %f7, 0x1C8(%r1);"
            "lfd       %f8, 0x1D0(%r1);"
            "lfd       %f9, 0x1D8(%r1);"
            "lfd       %f10, 0x1E0(%r1);"
            "lfd       %f11, 0x1E8(%r1);"
            "lfd       %f12, 0x1F0(%r1);"
            "lfd       %f13, 0x1F8(%r1);"
            "lfd       %f14, 0x200(%r1);"
            "lfd       %f15, 0x208(%r1);"
            "lfd       %f16, 0x210(%r1);"
            "lfd       %f17, 0x218(%r1);"
            "lfd       %f18, 0x220(%r1);"
            "lfd       %f19, 0x228(%r1);"
            "lfd       %f20, 0x230(%r1);"
            "lfd       %f21, 0x238(%r1);"
            "lfd       %f22, 0x240(%r1);"
            "lfd       %f23, 0x248(%r1);"
            "lfd       %f24, 0x250(%r1);"
            "lfd       %f25, 0x258(%r1);"
            "lfd       %f26, 0x260(%r1);"
            "lfd       %f27, 0x268(%r1);"
            "lfd       %f28, 0x270(%r1);"
            "lfd       %f29, 0x278(%r1);"
            "lfd       %f30, 0x280(%r1);"
            "lfd       %f31, 0x288(%r1);"

            // Restore r1 and return
            "ld        %r1, 0x78(%r1);"
            "blr;"
        );
    }

    __declspec(naked, noinline) void jump_to_handler()
    {
        asm
        (
            // Allocate enough space to save the registers
            "stdu      %r1, -0x300(%r1);"

            // Save r2, r3 and the link register now because we will need them
            "std       %r2, 0x80(%r1);"
            "std       %r3, 0x88(%r1);"
            "mflr      %r2;"
            "std       %r2, 0x170(%r1);"

            // Get the opd pointer from exec_handler() and call it
            "li        %r3, 0;"
            "oris      %r3, %r3, exec_handler@h;"
            "ori       %r3, %r3, exec_handler@l;"
            "lwz       %r2, 4(%r3);"
            "lwz       %r3, 0(%r3);"
            "mtlr      %r3;"
            "blr;"
        );
    }
}

namespace hook
{
    struct HookEntry_t
    {
        uintptr_t address;
        uint32_t instruction;
        void(*callback)(HookContext_t&);
    } *table;

    size_t tableEntryCount;
    uintptr_t jumpToHandlerAddress;
    uint32_t jumpToHandlerInstructions[12];

    extern "C" void _sys_memset(void*, int, int);

    void start(uintptr_t jumpAddress, size_t entryCount)
    {
        tableEntryCount = entryCount;
        jumpToHandlerAddress = jumpAddress;

        // Create our hook entry table
        table = new HookEntry_t[tableEntryCount];
        _sys_memset(table, 0, sizeof(HookEntry_t) * tableEntryCount);

        // Save and replace the jump address chosen by the user with the instructions of our jump_to_handler
        copy((uintptr_t)jumpToHandlerInstructions, (uint32_t*)jumpToHandlerAddress, 12);
        copy(jumpToHandlerAddress, *(uint32_t**)(jump_to_handler), 12);
    }

    void stop()
    {
        // Restore all original instructions
        for (int i = 0; i < tableEntryCount; i++)
        {
            if (table[i].address && table[i].instruction)
            {
                copy(table[i].address, &table[i].instruction, 1);
            }
        }

        copy(jumpToHandlerAddress, jumpToHandlerInstructions, 12);
        delete[] table;
    }

    void handler(HookContext_t& ctx)
    {
        // Parse our table
        for (int i = 0; i < tableEntryCount; i++)
        {
            if (table[i].address == (ctx.lr - 4))
                return table[i].callback(ctx);
        }

        // This should never happen, but we never know...
        __builtin_trap();
    }

    void inject(uintptr_t address, void(*callback)(HookContext_t&), bool linked)
    {
        // Parse our table to find a free entry
        HookEntry_t* entry = 0;
        for (int i = 0; i < tableEntryCount; i++)
        {
            if (!table[i].address)
            {
                entry = &table[i];
                break;
            }
        }

        if (!entry)
            return;

        entry->address = address;
        entry->instruction = *(uint32_t*)(address);
        entry->callback = callback;

        // Generate a 'bl jumpToHandlerAddress' instruction
        branch(address, jumpToHandlerAddress, linked);
    }

    // Classic hooking utilities
    void jump(uintptr_t address, uintptr_t callback)
    {
        uint32_t jmpInstructions[]
        {
            0x3D600000 | ((callback >> 16) & 0xFFFF), // lis       %r11, callback@h
            0x616B0000 | (callback & 0xFFFF),         // ori       %r11, %r11, callback@l  
            0x7D6903A6,                               // mtctr     %r11
            0x4E800420                                // bctr
        };

        copy(address, jmpInstructions, 4);
    }

    void jump(uintptr_t address, uintptr_t callback, uintptr_t stub)
    {
        copy(stub, reinterpret_cast<uint32_t*>(address), 4);
        jump(stub + 4 * sizeof(uint32_t), address + 4 * sizeof(uint32_t));

        uint32_t jmpInstructions[]
        {
            0x3D600000 | ((callback >> 16) & 0xFFFF), // lis       %r11, callback@h
            0x616B0000 | (callback & 0xFFFF),         // ori       %r11, %r11, callback@l  
            0x7D6903A6,                               // mtctr     %r11
            0x4E800420                                // bctr
        };

        copy(address, jmpInstructions, 4);
    }

    void branch(uintptr_t from, uintptr_t to, bool linked)
    {
        uint32_t branch = ((0x48000000 | linked) + ((to - from) & 0x3FFFFFF));
        copy(from, &branch, 1);
    }

    void nop(uintptr_t address, size_t instructionCount)
    {
        uint32_t nop = 0x60000000;
        for (int i = 0; i < instructionCount; i++)
        {
            copy(address + i * sizeof(uint32_t), &nop, 1);
        }
    }

    extern "C" void* _sys_memcpy(void* to, const void* from, size_t len);

    void copy(uintptr_t address, uint32_t* instructions, size_t instructionCount)
    {
        Proc_WriteMemory(Proc_GetCurrentPid(), address, instructions, sizeof(uint32_t) * instructionCount);
    }

    uintptr_t toc()
    {
        uintptr_t* entry = *reinterpret_cast<uintptr_t**>(0x1001C);
        return entry[1];
    }
}