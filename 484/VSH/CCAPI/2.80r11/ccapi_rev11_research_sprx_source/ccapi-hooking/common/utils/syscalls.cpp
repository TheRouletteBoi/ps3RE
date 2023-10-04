
#include "syscalls.hpp"
#include "hooking.hpp"

__declspec(naked, noinline) uint64_t syscall(int index, ...)
{
    asm
    (
        "mflr %r0;"
        "std %r0, 0x10(%r1);"
        "stdu %r1, -0x70(%r1);"
        "mr %r0, %r3;"
        "mr %r3, %r4;"
        "mr %r4, %r5;"
        "mr %r5, %r6;"
        "mr %r6, %r7;"
        "mr %r7, %r8;"
        "mr %r8, %r9;"
        "mr %r9, %r10;"
        "mr %r10, %r11;"
        "mr %r11, %r0;"
        "sc;"
        "addi %r1, %r1, 0x70;"
        "ld %r0, 0x10(%r1);"
        "mtlr %r0;"
        "blr;"
    );
}

void Sys_ExitEntryPoint()
{
    syscall(41, 0);
}

void Sys_FinalizeModule()
{
    uint64_t moduleArguments[5]{ 40, 2, 0, 0, 0 };
    syscall(482, syscall(461, Sys_FinalizeModule), 0, moduleArguments, 0, 0, 0);
}

int64_t Sys_GetCurrentTime()
{
    int64_t sec, nsec;
    syscall(145, &sec, &nsec);

    return sec * 1000 + nsec / 1000000;
}

int64_t sys_timeBase = 0;
int Sys_Milliseconds()
{
    static bool initialized = false;

    if (!initialized)
    {
        sys_timeBase = Sys_GetCurrentTime();
        initialized = true;
    }

    return Sys_GetCurrentTime() - sys_timeBase;
}

void Sys_Sleep(int millis)
{
    syscall(141, millis * 1000);
}

int Proc_GetCurrentPid()
{
    return syscall(1);
}

int Proc_WriteMemory(int pid, uintptr_t address, const void* data, size_t len)
{
    return syscall(905, pid, address, len, data);
}

int Proc_ReadMemory(int pid, uintptr_t address, void* data, size_t len)
{
    return syscall(904, pid, address, len, data);
}