
#pragma once

#include <sys/ppu_thread.h>
#include <sys/timer.h>

class thread
{
public:
    sys_ppu_thread_t id = SYS_PPU_THREAD_ID_INVALID;

    thread() = default;

    thread(void(*callback)(), int priority = 1000, int stackSize = 2048, const char* name = "", int flag = SYS_PPU_THREAD_CREATE_JOINABLE)
    {
        sys_ppu_thread_create(&id, [](uint64_t arg) -> void
            {
                if (arg)
                    reinterpret_cast<void(*)()>((uint32_t)arg)();

                sys_ppu_thread_exit(0);

            }, reinterpret_cast<uint64_t>(callback), priority, stackSize, flag, name);
    }

    bool running()
    {
        return (id != SYS_PPU_THREAD_ID_INVALID);
    }

    bool join()
    {
        if (!running())
            return false;

        uint64_t exitCode;
        return sys_ppu_thread_join(id, &exitCode) == CELL_OK;
    }
};