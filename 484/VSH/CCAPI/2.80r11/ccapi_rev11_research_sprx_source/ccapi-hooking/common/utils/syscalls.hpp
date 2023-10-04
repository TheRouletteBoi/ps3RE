
#pragma once

#include <sys/sys_types.h>

uint64_t syscall(int index, ...);

void Sys_ExitEntryPoint();
void Sys_FinalizeModule();

int64_t Sys_GetCurrentTime();
int Sys_Milliseconds();
void Sys_Sleep(int millis);

int Proc_GetCurrentPid();
int Proc_WriteMemory(int pid, uintptr_t address, const void* data, size_t len);
int Proc_ReadMemory(int pid, uintptr_t address, void* data, size_t len);