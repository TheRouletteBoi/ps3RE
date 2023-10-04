

//#define CCAPI_481_DEX 1
//#define CCAPI_481_DEX_IN_VSH 1
//#define CCAPI_481_DEX_IN_GAME 1
//#define CCAPI_482_DEX 0
#define CCAPI_484_DEX 1
#define CCAPI_484_DEX_IN_VSH 1
//#define CCAPI_484_DEX_IN_GAME 1

#include <cellstatus.h>
#include <sys/prx.h>
#include <string.h>
#include <limits.h>
#include <np.h>
#include <fastmath.h>
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <sys/memory.h>
#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/process.h>
#include <sysutil/sysutil_msgdialog.h>
#include <sysutil/sysutil_oskdialog.h>
#include <sysutil/sysutil_userinfo.h>
#include <cell/sysmodule.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netex/net.h>
#include <netex/errno.h>
#include <netex/libnetctl.h>
#include <netex/sockinfo.h>
#include <sys/socket.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <ppu_asm_intrinsics.h>
#include <ppu_intrinsics.h>
#include <cell/dbgfont.h>
#include <cell/gcm.h>
#include <cell/cell_fs.h>


#define CCAPI_FILE_PATH				"/dev_flash/sys/internal/sys_audio.sprx"
#define TOC_CCAPI_484D				0x7DEBC0
#define VSH_PROC_ID					0x1000300 // on first boot
#define DBGAGENT_PROC_ID			0x1000400  // on first boot
#define GAME_PROC_ID				0x01010200 // on first boot    | all these will change if soft reboot or vsh reboot or if you which games


#define AW_FURY_START_ADR			0xD00ED000  // FMT eboot 0xD00ED000      // 0xD0100000 paradox eboot w paradox loaded
#define TOC_DESTINY_BO2				0xD01E3960
#define BO2_DESTINY_START_ADR		0xD0146000
#define BO3_FATALITY_START_ADR		0xD01E0000  // this address is allocated already if you use the default eboot but when you use his eboot that adderss no no longer allocated. its going to be DDDDDDDD DDDDDDDD

#define BYTES_PER_WRITE				0x6000

#include "uitils.h"
#include "aw_fury.h"
#include "bo2_destiny.h"
#include "bo3_fatality.h"

SYS_MODULE_INFO(ccapi_research, 0, 1, 1);
SYS_MODULE_START(prx_entry);


/*
TODO: 
   - find main() funciton in the payload and then try calling it using bl or bctlr.   load it into empty memory
   - return 0 on create thread and then try alloate page to see which one create memory avaliable at 0xD00ED000





NOTES:
- The way he blocks tmapi & ccapi I think it by returning 0 in the get process list function in the kernel
- it seems like he allocate memory to that addres first then he injects the payload to that address then he starts the thread at that address thus making the payload run


   FURY
   payload stat  D00ED000
   payload TOC   D016F5C0


*/

sys_ppu_thread_t test_thread_id = SYS_PPU_THREAD_ID_INVALID;
sys_ppu_thread_t ccapi_menu_loader_thread_id = SYS_PPU_THREAD_ID_INVALID;




template <typename T>
T(*call(unsigned int address, unsigned int toc))(...)
{
   opd_s opd = { address, toc };
   T(*func)(...) = (T(*)(...))&opd;
   return func;
}


#define CcxCall uint64_t __attribute__((naked))

template <typename... Args>
static CcxCall sys_ccapi_call(uint64_t num, Args... arg)
{
   // doesn't work when called from a thread wtf??
   __asm__
   (
      "sc;" // ccsc = EF455314
      "blr;"
   );
}

static uint32_t FindSyscallInstruction(uint32_t address)
{
   while (1)
   {
      if (!*(uint32_t *)address)
      {
         address = (address - 0x4); // 0x4 for each instruction
         break;
      }
      else if (*(uint32_t *)(address) == 0x44000002) // 44000002 sc
      {
         break;
      }
      else
      {
         address = (address + 0x4); // // 0x4 for each instruction
      }
   }

   return address;
}

void ReplaceSyscallInstruction(uint32_t func) // (dex only) dex can only write to sprx functions
{
   printf("func: 0x%X\n", func);
   // call this function before calling sys_ccapi_call
   opd_s *opd = (opd_s *)(func);
   printf("opd->sub: 0x%X\n", opd->sub);
   uint32_t ccsc_syscall_instruction = 0xEF455314; // ccsc = EF455314

   uint32_t addr = FindSyscallInstruction(opd->sub);
   printf("addr: 0x%X\n", addr);

   sys_dbg_write_process_memory(addr, &ccsc_syscall_instruction, 0x4);
}


int CCAPIWriteProcessMemory(sys_pid_t pid, void *destination, const void *source, size_t size)
{
   int ret = sys_ccapi_call(0x123, pid, destination, size, source);
   printf("0x123 ret: 0x%X\n", ret);
   return ret;
}
int CCAPIReadProcessMemory(sys_pid_t pid, void *destination, void *source, size_t size)
{
   // 0x80000100 if buffer is invalid or failed to write?
   // 0x80001000 failed process doesnt not exist
   // 0x80000600 invalid read
   // returns 0x80001000 if process isn't acive, returns 0 if sucessful
   uint64_t ret = sys_ccapi_call(0x785, pid, destination, size, source);
   printf("0x785 ret: 0x%X\n", ret);
   return ret;
}
int CCAPICreateProcessThread(sys_pid_t pid, thread_t* thread, void *entry, uint64_t arg, int prio, size_t stacksize, const char *threadname) // void(*entry)(uint64_t), sys_ppu_thread_t *arg
{
   uint64_t ret = sys_ccapi_call(0x357, pid, thread, entry, arg, prio, stacksize, threadname);
   printf("0x357 ret: 0x%X\n", ret);
   return ret;
}
int CCAPIAllocatePage(sys_pid_t pid, uint64_t size, uint64_t page_size, uint64_t flags, uint64_t is_executable, uint64_t *kernel_page_adr, uint64_t *game_page_adr)
{
   uint64_t ret = sys_ccapi_call(0x977, pid, size, page_size, flags, is_executable, kernel_page_adr, game_page_adr);
   printf("0x977 ret: 0x%X\n", ret);
   return ret;
}
int CCAPIEnableSysCall()
{
   uint64_t ret = sys_ccapi_call(0x241);
   printf("0x241 ret: 0x%X\n", ret);
   return ret;
}
int CCAPIConsoleShutDown(int shutdown_type)
{
   uint64_t ret = sys_ccapi_call(0x117, shutdown_type);
   printf("0x117 ret: 0x%X\n", ret);
   return ret;
}



#define SYSCALL8_OPCODE_PROC_CREATE_THREAD			0x6CE5
#define SYSCALL8_OPCODE_GET_MODULE_INFO				0x6CE7
#define SYSCALL8_OPCODE_GET_MODULE_SEGMENTS			0x6CE7
#define SYSCALL8_OPCODE_SET_PROC_MEMORY				0x6CF1
#define SYSCALL8_OPCODE_GET_PROC_MEMORY				0x6CF3
#define SYSCALL8_OPCODE_PROC_PAGE_ALLOCATE			0x6CF5
#define SYSCALL8_OPCODE_PROC_LOAD_MODULE_NO_THREAD	0x6CF7

// comfrimed working :)
int ps3mapi_create_process_thread(sys_pid_t pid, thread_t* thread, void *entry, uint64_t arg, int prio, size_t stacksize, const char *threadname)
{
   system_call_8(207, SYSCALL8_OPCODE_PROC_CREATE_THREAD, (uint64_t)pid, (uint64_t)thread, (uint64_t)entry, (uint64_t)arg, (uint64_t)prio, (uint64_t)stacksize, (uint64_t)threadname);
   return_to_user_prog(int);
}

int ps3mapi_set_process_memory(sys_pid_t pid, void *destination, const void *source, size_t size)
{
   system_call_5(207, SYSCALL8_OPCODE_SET_PROC_MEMORY, (uint64_t)pid, (uint64_t)destination, (uint64_t)source, (uint64_t)size);
   return_to_user_prog(int);
}

int ps3mapi_page_allocate(sys_pid_t pid, uint64_t size, uint64_t page_size, uint64_t flags, uint64_t is_executable, uint64_t *page_address)
{
   system_call_7(207, SYSCALL8_OPCODE_PROC_PAGE_ALLOCATE, (uint64_t)pid, (uint64_t)size, (uint64_t)page_size, (uint64_t)flags, (uint64_t)is_executable, (uint64_t)page_address);
   return_to_user_prog(int);
}

int ps3mapi_process_load_module_no_thread(sys_pid_t pid, char* path)
{
   system_call_3(207, SYSCALL8_OPCODE_PROC_LOAD_MODULE_NO_THREAD, (uint64_t)pid, (uint64_t)path);
   return_to_user_prog(int);
}

void write_payload(uint32_t start_addr, const char *file)
{
   int next_write = 0;

   int fd = 0; uint64_t read_e = 1;

   if (cellFsOpen(file, CELL_FS_O_RDONLY, &fd, NULL, 0) == CELL_FS_SUCCEEDED)
   {
      while (read_e)
      {
         char *data = (char *)malloc(BYTES_PER_WRITE);
         if (cellFsRead(fd, (void *)(data), BYTES_PER_WRITE, &read_e) == CELL_FS_SUCCEEDED)
         {
            CCAPIWriteProcessMemory(GAME_PROC_ID, (void *)(start_addr + next_write), data, read_e);
            next_write += read_e;
            if (data)
               free(data);
         }
      }

      cellFsClose(fd);
   }
}














uint64_t ccapi_syscall_stub(uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10)
{
   __nop();
   __nop();
   __nop();
   __nop();
   __nop();
   __nop();
   __nop();
   __nop();
   __nop();
}

int inject_count = 0;
bool bthread_created = false;

uint64_t ccapi_syscall(uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10)
{
   printf("ccapi_lv2_syscall\n");
   printf("r3: 0x%X\n", r3);
   printf("r4: 0x%X\n", r4);
   printf("r5: 0x%X\n", r5);
   printf("r6: 0x%X\n", r6);
   printf("r7: 0x%X\n", r7);
   printf("r8: 0x%X\n", r8);
   printf("r9: 0x%X\n", r9);
   printf("r10: 0x%X\n", r10);

   if (r3 == 0x241)
   {
      // ccsc(0x241, opd_of_ccsc, func_of_ccsc, E8410028 ld r2,0x28(r1),  E8410028 ld r2,0x28(r1), FFFFFFFF)

      // buff, value, value2
      // buff, value
      printf("r4 0: 0x%016llX\n", *(uint64_t *)(r4));

      if (*(uint64_t *)(r4) == 0x8000000000565000)
      {
         printf("r6 0: 0x%016llX\n", *(uint64_t *)(r6));
         //return 0;
      }

      if (r5 == 0x1)
      {
         if (*(uint64_t *)(r4) == 0x8000000000565000)
         {
            printf("r6 1: 0x%016llX\n", *(uint64_t *)(r6));
            printf("r7 1: 0x%016llX\n", *(uint64_t *)(r7));
         }
      }


      if (r5 == 0x20)
      {
         printf("r7 2: 0x%016llX\n", *(uint64_t *)(r7));
         // this will cause not implemted return 
         //return 0;
      }

      // returning 0 on this command will cuase  ENOSYS/0x80010003
   }

   if (r3 == 0xCCC)
   {
      printf("bypassed tmapi and syscall8\n");
      return 0;
   }
   if (r3 == 0x123)
   {
      uint64_t inject_bytes = *(uint64_t *)(r7);
      if (!bthread_created)
      {
         //if (inject_bytes != 0x0000000000000000) {
         char next_file_name[250];
         memset(next_file_name, 0, sizeof(next_file_name));
         snprintf(next_file_name, sizeof(next_file_name), "/dev_usb000/plugins/inject_write_process_%i_at_0x%X.bin", inject_count, r5);
         printf("next_file_name: %s\n", next_file_name);
         int fd;
         if (cellFsOpen(next_file_name, CELL_FS_O_CREAT | CELL_FS_O_TRUNC | CELL_FS_O_WRONLY, &fd, NULL, 0) == CELL_FS_SUCCEEDED)
         {
            cellFsWrite(fd, (const void *)r7, r6, NULL);
            cellFsClose(fd);
            inject_count++;
         }
         printf("inject_count: %i\n", inject_count);
         //}
      }

      // only block the null bytes after thread has been created
      /*if (inject_bytes == 0x0000000000000000)
      {
      if (r5 != 0xD0165000) // this address is used to write more thread code
      {
      printf("blocking null bytes at: 0x%X\n", r5);
      return 0;
      }
      }*/
   }
   if (r3 == 0x357)
   {
      //return 0; // will cause the menu to load into the address space but will overwrite everything to null bytes if thread fails to start. address space will be allocated though

      bthread_created = true;

      printf("create_thread\n");
      printf("r7 64: 0x%016llX\n", r7); // to print the thread arg

      // r5: 0x00000000D00ED000     r6: 0xD00ED00000000000      r10: 0x00000000007A2FD8
      printf("r5: 0x%016llX     r6: 0x%016llX      r10: 0x%016llX\n", *(uint64_t *)(r5), *(uint64_t *)(r6), *(uint64_t *)(r10));

      //uint32_t thread_opd[2];
      //thread_opd[0] = 0x00000000;
      //thread_opd[1] = 0xD00ED000; // payload start address

      //uint32_t thread_arg;
      // r5: 0xD00D6B60 = 0x00000000D00ED000
      // r6: 0xD00D6B64 = 0xD00ED00000000000
      //call<uint32_t>(0x770448, TOC_CCAPI_484D)(0x357, 0x1010200, thread_opd, /*&thread_arg*/ /*0xD00D6B64*/ 0, 0x78000, 0x7D0, 0x4000, "hello_thread");

      /*
      ccapi_lv2_syscall
      r3: 0x357
      r4: 0x1010200
      r5: 0xD00D6B60
      r6: 0xD00D6B64
      r7: 0x78000
      r8: 0x7D0
      r9: 0x4000
      r10: 0xD00D6B70
      create_thread
      r5: 0x00000000D00ED000     r6: 0xD00ED00000000000      r10: 0x00000000007A2FD8
      thread_opd: 0x00000000D00ED000     thread_func: 0x0000000000000000
      ret: 0x0
      */
   }
   if (r3 == 0x977)
   {
      //return 0; // will say menu was unable to load and address space will not load

      printf("allocate_page\n");
      printf("r9: 0x%016llX      r10: 0x%016llX\n", *(uint64_t *)(r9), *(uint64_t *)(r10));
   }
   

#ifdef CCAPI_481_DEX_IN_GAME
   //uint64_t ret = call<uint64_t>(0xD0149720, TOC_DESTINY_BO2)(r3, r4, r5, r6, r7, r8, r9, r10);
#endif
   uint64_t ret = ccapi_syscall_stub(r3, r4, r5, r6, r7, r8, r9, r10);
   printf("ret: 0x%X\n", ret);
   return ret;
}
uint64_t ccapi_decrypt_packet(uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10)
{
   printf("ccapi_decrypt_packet\n");
   printf("r3: 0x%X\n", r3);
   printf("r4: 0x%X\n", r4);
   printf("r5: 0x%X\n", r5);
   printf("r6: 0x%X\n", r6);
   printf("r7: 0x%X\n", r7);
   printf("r8: 0x%X\n", r8);
   printf("r9: 0x%X\n", r9);
   printf("r10: 0x%X\n", r10);


   uint64_t ret = call<uint64_t>(0x7A2FAC, TOC_CCAPI_484D)(r3, r4, r5, r6, r7, r8, r9, r10);
   printf("ret: 0x%X  0x%016llX\n", ret, *(uint64_t *)(ret)); // returns address of the command id
   return ret;
}

uint64_t ccapi_get_command_id_r3(uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6)
{
   printf("r3: 0x%016llX  0x%016llX\n", r3, *(uint64_t *)(r3)); // command id

   // check if address is avaliable in game 0xD00ED000
   if (*(uint64_t *)(r3) == 0x0000240F00000000)
   {
      *(uint64_t *)(r3) = 0x0; // this will cause error starting menu
   }

   __asm__(
      "clrldi     %r3,%r3,32;"
      //"lwz        %r3,0x0(%r3);"
   );

   return r3;
}

void inject_rpc_thread()
{
   uint32_t rpc_function[] = {
      0x60000000, // lis        r3,0x1
      0x60000000, // ori        r3,r3,0x68
      0x60000000, // lis        r4,0x1005
      0x60000000, // ori        r4,r4,0x8
      0x60000000, // stw        r3,0x0(r4)
      0x60000000, // stw        r2,0x4(r4)
      0x60000000, // stw        r4,0x8(r4)
      0x60000000, // lis        r3,0x1005
      0x60000000, // lis        r4,0x1005
      0x60000000, // ori        r4,r4,0x10
      0x60000000, // li         r5,0x0
      0x60000000, // li         r6,0x0
      0x60000000, // li         r7,0x300
      0x60000000, // li         r8,0x3000
      0x60000000, // li         r9,0x0
      0x60000000, // lis        r10,0x1005
      0x60000000, // ori        r10,r10,0x50
      0x60000000, // li         r11,0x34
      0x60000000, // sc
      0x60000000, // lis        r3,0x1005
      0x60000000, // ld         r3,0x0(r3)
      0x60000000, // li         r11,0x35
      0x60000000, // sc
      0x60000000, // nop
      0x60000000, // nop
      0x60000000, // b          0x0001005C
      0x3D801005, // lis        r12,0x1005
      0x618C0100, // ori        r12,r12,0x100
      0x806C0000, // lwz        r3,0x0(r12)
      0x2C030000, // cmpwi      r3,0x0
      0x41820040, // beq        0x000100B8
      0x7C6B1B78, // mr         r11,r3
      0x38600000, // li         r3,0x0
      0x906C0000, // stw        r3,0x0(r12)
      0xE86C0008, // ld         r3,0x8(r12)
      0xE88C0010, // ld         r4,0x10(r12)
      0xE8AC0018, // ld         r5,0x18(r12)
      0xE8CC0020, // ld         r6,0x20(r12)
      0xE8EC0028, // ld         r7,0x28(r12)
      0xE90C0030, // ld         r8,0x30(r12)
      0xE92C0038, // ld         r9,0x38(r12)
      0xE94C0040, // ld         r10,0x40(r12)
      0x44000002, // sc
      0x3D801005, // lis        r12,0x1005
      0x618C0100, // ori        r12,r12,0x100
      0xF86C0048, // std        r3,0x48(r12)
      0x3C601006, // lis        r3,0x1006
      0x81830090, // lwz        r12,0x90(r3)
      0x2C0C0000, // cmpwi      r12,0x0 
      0x41820088, // beq        0x0001014C
      0xE8830008, // ld         r4,0x8(r3)
      0xE8A30010, // ld         r5,0x10(r3)
      0xE8C30018, // ld         r6,0x18(r3)
      0xE8E30020, // ld         r7,0x20(r3)
      0xE9030028, // ld         r8,0x28(r3)
      0xE9230030, // ld         r9,0x30(r3)
      0xE9430038, // ld         r10,0x38(r3)
      0xE9630040, // ld         r11,0x40(r3)
      0xC0230048, // lfs        f1,0x48(r3)
      0xC0430050, // lfs        f2,0x50(r3)
      0xC0630058, // lfs        f3,0x58(r3)
      0xC0830060, // lfs        f4,0x60(r3)
      0xC0A30068, // lfs        f5,0x68(r3)
      0xC0C30070, // lfs        f6,0x70(r3)
      0xC0E30078, // lfs        f7,0x78(r3)
      0xC1030080, // lfs        f8,0x80(r3)
      0xC1230088, // lfs        f9,0x88(r3)
      0xE8630000, // ld         r3,0x0(r3)
      0x7D8903A6, // mtspr      ctr,r12
      0x3D801006, // lis        r12,0x1006
      0xF84C00A8, // std        r2,0xA8(r12)
      0xE98C00B0, // ld         r12,0xB0(r12)
      0x2C0C0000, // cmpwi      r12,0x0
      0x41820008, // beq        0x0001012C
      0x7D826378, // mr         r2,r12
      0x4E800421, // bctrl
      0x3C801006, // lis        r4,0x1006
      0x38A00000, // li         r5,0x0
      0xE84400A8, // ld         r2,0xA8(r4)
      0xF8A400B0, // std        r5,0xB0(r4)
      0x90A40090, // stw        r5,0x90(r4)
      0xF8640098, // std        r3,0x98(r4)
      0xD02400A0, // stfs       f1,0xA0(r4)
      0x38600064, // li         r3,0x64
      0x3960008D, // li         r11,0x8D
      0x44000002, // sc
      0x4BFFFF10, // b          0x00010068
   };


   // need to test that it can call game addresses

   CCAPIWriteProcessMemory(GAME_PROC_ID, (void *)0x10000, (const void *)rpc_function, sizeof(rpc_function));
}


#define SC_COBRA_SYSCALL8                               8
#define SYSCALL8_OPCODE_PS3MAPI						0x7777
#define PS3MAPI_OPCODE_SET_PROC_MEM					0x0032

int WriteProcessMemory_PS3MAPI(sys_pid_t pid, void *destination, const void *source, size_t size)
{
   system_call_6(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_PS3MAPI, PS3MAPI_OPCODE_SET_PROC_MEM, (uint64_t)pid, (uint64_t)destination, (uint64_t)source, (uint64_t)size);
   return_to_user_prog(int);
}


#define KB(n) (1024*n)

#include "menu_loader.h"

void test_thread(uint64_t arg)
{
   printf("thread hello\n");
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<>); // enable ccapi syscall
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, void *, size_t, const void *>); // write process params
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, void *, size_t, void *>); // read process params
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, thread_t*, void *, uint64_t, int, size_t, const char *>); // create thread params
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t*, uint64_t*>); // allocate page
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<int>); // console shutdown


   printf("enabling ccsc\n");

   CCAPIEnableSysCall(); // crashes here if you dont use is_debuggable func in kernel or if the eboot is for cex.  it will work if eboot is for dex.     ***UPDATE: this happned becuase cex eboot will not work with dex syscalls, so to do this you need to replaced the byes in HxD instead of doing the method above.

   printf("done enabling ccsc\n");


   // doesn't work here wtf??
   char tempbytes[] = { 0x77, 0xFF, 0xFF, 0xFF };
   //char tempbytes2[4];
   int ret_ccapi_write = CCAPIWriteProcessMemory(VSH_PROC_ID, (void *)0x10004, tempbytes, 0x4);
   printf("ret_ccapi_write: 0x%X\n", ret_ccapi_write);


   printf("done writing mem to vsh proc\n");

   Sleep(5000);

   printf("starting while loop\n");

   while (true)
   {
      /*
      uint32_t VSH_DATA;
      CCAPIReadProcessMemory(VSH_PROC_ID, (void *)0x10050000, (void *)&VSH_DATA, 0x4);
      if (VSH_DATA == 0x01000000) {
         printf("on xmb test option\n");


         char tempbytes[] = { 0x00, 0x00, 0x00, 0x00 };
         CCAPIWriteProcessMemory(VSH_PROC_ID, (void *)0x10050000, tempbytes, 0x4);
      }*/

      uint32_t GAME_DATA;
      CCAPIReadProcessMemory(GAME_PROC_ID, (void *)0x10020000, (void *)&GAME_DATA, 0x4);

      if (GAME_DATA == 0x01000000) {
         printf("allocating page...\n");

         // Destiny
         uint64_t kernel_adr, game_adr;
         CCAPIAllocatePage(GAME_PROC_ID, 0x9D000, 0x100, 0x2F, 0x1, &kernel_adr, &game_adr); //  KB(628) KB(604)    0x97000 old destiny   size of file in your computer details  "Size on Disc"  http://prntscr.com/p7iei7

         // FURY
         //CCAPIAllocatePage(GAME_PROC_ID, 0x7C000, 0x100, 0x2F, 0x1, &kernel_adr, &game_adr); //  KB(496)

         // Fatality
         //CCAPIAllocatePage(GAME_PROC_ID, 0x6F000, 0x100, 0x2F, 0x1, &unk3, &unk4); //  KB(444)


         //uint64_t addr_spacee;
         //int ret_page = ps3mapi_page_allocate(GAME_PROC_ID, 0x7C000, 0x100, 0x2F, 0x1, &addr_spacee); //  0x2D  0x32  0x35   0x2F
         //printf("ret_page: 0x%016llX\n", ret_page);
         //printf("addr_spacee: 0x%016llX\n", addr_spacee);

         /*
         // both work :)
         // when I did the ccapi allocate page first
         ret_page: 0x80010003
         addr_spacee: 0xD0169000

         // first time
         ret_page: 0x80010003
         addr_spacee: 0xD00ED000


         ret_page: 0x80010003
         addr_spacee: 0xD00ED000
         */

         // kernel_adr: 00x8000000100078000   game_adr: 0x00000000D00ED000
         printf("kernel_adr: 0x%016llX   game_adr: 0x%016llX\n", kernel_adr, game_adr); // unk3: 0x80000001   unk4: 0x0

         char tempbytes[] = { 0x00, 0x00, 0x00, 0x00 };
         CCAPIWriteProcessMemory(GAME_PROC_ID, (void *)0x10020000, tempbytes, 0x4);
      }

      if (GAME_DATA == 0x00010000) {
         printf("injecting destiny\n");

         //inject_bo2_destiny();
         //inject_rpc_thread();

         inject_aw_fury();

         //inject_bo3_fatality();

         char tempbytes[] = { 0x00, 0x00, 0x00, 0x00 };
         CCAPIWriteProcessMemory(GAME_PROC_ID, (void *)0x10020000, tempbytes, 0x4);
      }
      if (GAME_DATA == 0x00000100) {
         printf("creating thread for destiny\n");


         //uint64_t ret = call<uint64_t>(BO2_DESTINY_START_ADR, TOC_DESTINY_BO2)();

         uint32_t toc[2];
         //toc[0] = BO2_DESTINY_START_ADR;
         toc[0] = AW_FURY_START_ADR;
         //toc[0] = BO3_FATALITY_START_ADR;
         toc[1] = 0x00000000;

         // destiny arg thread
         thread_t th;
         CCAPICreateProcessThread(GAME_PROC_ID, &th, toc, 0, 0x7D0, 0x4000, "ccapi_created_thread");

         //int ret_thread = ps3mapi_create_process_thread(GAME_PROC_ID, &th, toc, 0, 0x7D0, 0x4000, "ps3mapi_created_thread");
         //printf("ret_thread: 0x%X\n", ret_thread); // ret_thread: 0x80010005

          // fury arg thread
         //thread_t th;
         //CCAPICreateProcessThread(GAME_PROC_ID, &th, toc, 0x100078000, 0x7D0, 0x4000, "ccapi_created_thread");

         char tempbytes[] = { 0x00, 0x00, 0x00, 0x00 };
         CCAPIWriteProcessMemory(GAME_PROC_ID, (void *)0x10020000, tempbytes, 0x4);
      }

      if (GAME_DATA == 0x00000001) {

         printf("calling test hook\n");
         //hook_function(0x42FC8, func_stub, my_func, &orig_my_func);
         ps3mapi_process_load_module_no_thread(GAME_PROC_ID, "/dev_hdd0/tmp/Yisus.sprx");


         char tempbytes[] = { 0x00, 0x00, 0x00, 0x00 };
         CCAPIWriteProcessMemory(GAME_PROC_ID, (void *)0x10020000, tempbytes, 0x4);
      }


      Sleep(10000);
   }

   sys_ppu_thread_exit(0);
}



extern "C" int prx_entry(void)
{

#ifdef CCAPI_481_DEX


#ifdef CCAPI_481_DEX_IN_VSH
   // view all processes
   pokeq(0x8000000000003B38, 0x386000014E800020);


   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<>); // enable ccapi syscall
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, void *, size_t, const void *>); // write process params
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, void *, size_t, void *>); // read process params
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, thread_t*, void *, uint64_t, int, size_t, const char *>); // create thread params
   ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, uint64_t, uint64_t, uint64_t, uint64_t, uint32_t*, uint32_t*>); // allocate page

   Sleep(500);

   sys_ppu_thread_create(&test_thread_id, test_thread, 0, 3000, 0x02000UL, 0x1, "test_thread");


   #define CCAPI_VSH_BASE_ADDRESS_481D 0x770000

   printf("hooking ccsc 481 DEX\n");
   HookFunctionStart(0x770570, *(int*)ccapi_syscall, *(int*)ccapi_syscall_stub);


#elif CCAPI_481_DEX_IN_GAME

   // *(uint64_t *)(0xD01DBDE8) = *(uint64_t*)((int)ccapi_syscall);

   sys_ppu_thread_create(&test_thread_id, test_thread, 0, 3000, 0x02000UL, 0x1, "game_test_thread");

   //_sys_ppu_thread_exit(0);
#endif



#elif CCAPI_482_DEX

   // J Milk research

   //BranchLinkedHook(0x770434, (int)HOOK);
   char noping[] = { 0x7F, 0x64, 0xDB, 0x78 };
   //char compare[] = { 0x2F, 0x83, 0x07, 0x9C };
   /*sys_dbg_write_process_memory(0x77F310, &noping, 4);
   BranchLinkedHook(0x77F314, *(int*)HOOK_Write);
   sys_dbg_write_process_memory(0x77EE40, &noping, 4);
   BranchLinkedHook(0x77EE44, *(int*)HOOK_Read);*/
   //HookFunctionStart(0x770570, *(int*)HOOK, *(int*)hook_Stub);
   /*BranchLinkedHook(*(int*)HOOK_commandID, *(int*)myfunction);
   sys_dbg_write_process_memory((*(int*)HOOK_commandID) + 4, &compare, 4);
   BranchHook((*(int*)HOOK_commandID) + 8, 0x77E488);
   BranchHook(0x77E484, *(int*)HOOK_commandID);*/
   //pokeq(0x8000000000003B38,0x386000014E800020);

#elif CCAPI_484_DEX


#ifdef CCAPI_484_DEX_IN_VSH

   // view all processes
   //pokeq(0x8000000000003B38, 0x386000014E800020);

   // make ccsc, we need to do this for each func with different params becuase thats how the compiler compiler each individual function. meaning they have the same code but different params in ida
   //ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<>); // enable ccapi syscall
   //ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, void *, size_t, const void *>); // write process params
   //ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, void *, size_t, void *>); // read process params
   //ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, thread_t*, void *, uint64_t, int, size_t, const char *>); // create thread params
   //ReplaceSyscallInstruction((uint32_t)sys_ccapi_call<sys_pid_t, uint64_t, uint64_t, uint64_t, uint64_t, uint32_t*, uint32_t*>); // allocate page


   //HookFunctionStart(0x770448, *(int*)ccapi_syscall, *(int*)ccapi_syscall_stub);
   //BranchLinkedHook(0x7AEF3C, *(int*)ccapi_decrypt_packet);
   //BranchLinkedHook(0x7AEF44, *(int*)ccapi_get_command_id_r3);



   //printf("calling ccapi funcs\n");

   

   //char tempbytes[] = { 0xFF, 0xFF, 0xFF, 0xFF };
   //char tempbytes2[4];
   //CCAPIWriteProcessMemory(VSH_PROC_ID, (void *)0x10000, tempbytes, 0x4);
   //CCAPIReadProcessMemory(VSH_PROC_ID, (void *)0x10000, &tempbytes2, 0x4);


   /*
   uint32_t unk3 = 0xD00ED000, unk4 = 0xD00ED000;
   CCAPIAllocatePage(VSH_PROC_ID, 0x7C000, 0x100, 0x2F, 0x1, &unk3, &unk4);
   printf("unk3: 0x%X   unk4: 0x%X\n", unk3, unk4); // unk3: 0x80000001   unk4: 0x0
   */


   /*
   // crashes but thread does get created
   //sys_ppu_thread_t ccapi_thread_id = SYS_PPU_THREAD_ID_INVALID;
   uint64_t meminfo[5];
   uint32_t toc[2];

   meminfo[2] = 0x00000000D00ED000; // this will allocate memory to that address :)
   memcpy((void *)meminfo[2], toc, sizeof(toc));
   CCAPICreateProcessThread(VSH_PROC_ID, toc[0], toc[1], 0x78000, 0x7D0, 0x4000, "ccapi_created_thread");
   //printf("ccapi_thread_id: 0x%X\n", ccapi_thread_id);


   */



   sys_ppu_thread_create(&test_thread_id, test_thread, 0, 3000, 0x02000UL, 0x1, "vsh_test_thread"); // manually with debugger write
   //sys_ppu_thread_create(&ccapi_menu_loader_thread_id, ccapi_menu_loader_thread, 0, 3000, 0x02000UL, 0x1, "ccapi_menu_loader_thread"); // inject automatic with button binds
   _sys_ppu_thread_exit(0);


#elif CCAPI_484_DEX_IN_GAME

   // *(uint64_t *)(0xD01DBDE8) = *(uint64_t*)((int)ccapi_syscall);

   sys_ppu_thread_create(&test_thread_id, test_thread, 0, 3000, 0x02000UL, 0x1, "game_test_thread");

   //_sys_ppu_thread_exit(0);
#endif


#endif

    return SYS_PRX_RESIDENT;
}