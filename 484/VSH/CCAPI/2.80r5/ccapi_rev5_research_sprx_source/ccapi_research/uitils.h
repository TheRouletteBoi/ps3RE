

struct opd_s
{
   uint32_t sub;
   uint32_t toc;
};

typedef struct
{
   void *unk_0; // ptr to some funcs
   uint64_t unk_8;
   uint32_t unk_10;
   uint32_t unk_14;
   void *unk_18;
   void *unk_20; // same as unk_18? :S
   uint64_t unk_28[3];
   void *unk_40; // same as unk_0?
              // ...
} thread_t;




#define CDECL extern "C"

CDECL void* _sys_malloc(size_t size);
#define malloc _sys_malloc
CDECL void _sys_free(void* ptr);
#define free _sys_free
CDECL void* _sys_memalign(size_t boundary, size_t size);
#define memalign _sys_memalign
CDECL void* _sys_memchr(const void* s, int c, size_t n);
#define memchr _sys_memchr
CDECL int _sys_memcmp(const void* s1, const void* s2, size_t n);
#define memcmp _sys_memcmp
CDECL void* _sys_memcpy(void* dest, const void* src, size_t n);
#define memcpy _sys_memcpy
CDECL void* _sys_memset(void* m, int c, size_t n);
#define memset _sys_memset
CDECL void* _sys_memmove(void* str1, const void* str2, size_t n);
#define memmove _sys_memmove
CDECL int _sys_printf(const char* fmt, ...);
#define printf _sys_printf
CDECL int _sys_snprintf(char* buffer, size_t len, const char* fmt, ...);
#define snprintf _sys_snprintf
CDECL int _sys_sprintf(char* buffer, const char* fmt, ...);
#define sprintf _sys_sprintf
CDECL char* _sys_strcat(char* dest, const char* src);
#define strcat _sys_strcat
CDECL char* _sys_strchr(const char* s, int c);
#define strchr _sys_strchr
CDECL int _sys_strcmp(const char* s1, const char* s2);
#define strcmp _sys_strcmp
CDECL char* _sys_strcpy(char* dest, const char* src);
#define strcpy _sys_strcpy
CDECL size_t _sys_strlen(const char* str);
#define strlen _sys_strlen
CDECL int _sys_strncasecmp(const char* s1, const char* s2, size_t n);
#define strncasecmp _sys_strncasecmp
CDECL char* _sys_strncat(char* dest, const char* src, size_t n);
#define strncat _sys_strncat
CDECL int _sys_strncmp(const char* s1, const char* s2, size_t n);
#define strncmp _sys_strncmp
CDECL char* _sys_strncpy(char* dest, const char* src, size_t n);
#define strncpy _sys_strncpy
CDECL char* _sys_strrchr(const char* s, int c);
#define strrchr _sys_strrchr
CDECL int _sys_tolower(int c);
#define tolower _sys_tolower
CDECL int _sys_toupper(int c);
#define toupper _sys_toupper
CDECL int _sys_vprintf(const char* format, va_list arg);
#define vprintf _sys_vprintf
CDECL int _sys_vsnprintf(char* buffer, std::size_t buf_size, const char* format, va_list arg);
#define vsnprintf _sys_vsnprintf
CDECL int _sys_vsprintf(char* s, const char* format, va_list arg);
#define vsprintf _sys_vsprintf










#define SC_PEEK_LV2                     6
#define SC_POKE_LV2                     7
#define SC_PEEK_LV1                     8
#define SC_POKE_LV1                     9
static inline uint64_t peekq(uint64_t addr) //lv2
{
   system_call_1(SC_PEEK_LV2, addr);
   return_to_user_prog(uint64_t);
}
static inline void pokeq(uint64_t addr, uint64_t val) //lv2
{
   system_call_2(SC_POKE_LV2, addr, val);
}



int32_t WriteMemory(uint64_t ea, const void * data, uint32_t size)
{
   system_call_4(905, (uint64_t)sys_process_getpid(), ea, size, (uint64_t)data);
   return_to_user_prog(int32_t);
}

void ExitThread(uint64_t ThreadID)
{
   system_call_1(41, ThreadID);
}
                  
static inline void _sys_ppu_thread_exit(uint64_t val)
{
   system_call_1(41, val); // prxloader = mandatory; cobra = optional; ccapi = don't use !!!
}

void Sleep(uint64_t msecs)
{
   system_call_1(141, msecs * 1000)
}


void MidFunctionHook(int Native, int Destination)
{
   int FuncBytes[1];
   FuncBytes[0] = *(int*)Destination;
   memcpy((void*)Native, FuncBytes, 4);

}

int32_t sys_dbg_write_process_memory1(void *address, const void *data, size_t size)
{
   system_call_4(905, (uint64_t)sys_process_getpid(), (uint64_t)address, size, (uint64_t)data);
   return_to_user_prog(int32_t);
}

int32_t sys_dbg_read_process_memory(uint64_t address, void *data, size_t size)
{
   system_call_4(904, (uint64_t)sys_process_getpid(), address, size, (uint64_t)data);
   return_to_user_prog(int32_t);
}

int32_t sys_dbg_write_process_memory(uint64_t address, const void *data, size_t size)
{
   system_call_4(905, (uint64_t)sys_process_getpid(), address, size, (uint64_t)data);
   return_to_user_prog(int32_t);
}
void HookFunctionStart(uint32_t functionStartAddress, uint32_t newFunction, uint32_t functionStub)
{
   uint32_t normalFunctionStub[8], hookFunctionStub[4];
   sys_dbg_read_process_memory(functionStartAddress, normalFunctionStub, 0x10);
   normalFunctionStub[4] = 0x3D600000 + ((functionStartAddress + 0x10 >> 16) & 0xFFFF);
   normalFunctionStub[5] = 0x616B0000 + (functionStartAddress + 0x10 & 0xFFFF);
   normalFunctionStub[6] = 0x7D6903A6;
   normalFunctionStub[7] = 0x4E800420;
   sys_dbg_write_process_memory(functionStub, normalFunctionStub, 0x20);
   hookFunctionStub[0] = 0x3D600000 + ((newFunction >> 16) & 0xFFFF);
   hookFunctionStub[1] = 0x616B0000 + (newFunction & 0xFFFF);
   hookFunctionStub[2] = 0x7D6903A6;
   hookFunctionStub[3] = 0x4E800420;
   sys_dbg_write_process_memory(functionStartAddress, hookFunctionStub, 0x10);
}
int32_t HookFunction(uint32_t address, uint32_t function)
{
   uint32_t opcode[4];
   opcode[0] = 0x3D600000 + ((function >> 16) & 0xFFFF);
   opcode[1] = 0x616B0000 + (function & 0xFFFF);
   opcode[2] = 0x7D6903A6;
   opcode[3] = 0x4E800420;
   return sys_dbg_write_process_memory(address, &opcode, 0x10);
}

void BranchLinkedHook(uint32_t branchFrom, uint32_t branchTo)
{
   uint32_t branch;
   if (branchTo > branchFrom)
      branch = 0x48000001 + (branchTo - branchFrom);
   else
      branch = 0x4C000001 - (branchFrom - branchTo);
   sys_dbg_write_process_memory(branchFrom, &branch, 4);
}
int32_t BranchHook(uint32_t branchFrom, uint32_t branchTo)
{
   uint32_t branch;
   if (branchTo > branchFrom)
      branch = 0x48000000 + (branchTo - branchFrom);
   else
      branch = 0x4C000000 - (branchFrom - branchTo);
   return sys_dbg_write_process_memory(branchFrom, &branch, 4);
}



float StringToFloat(const char* s)
{
   float rez = 0, fact = 1;
   if (*s == '-')
   {
      s++;
      fact = -1;
   };
   for (int point_seen = 0; *s; s++)
   {
      if (*s == '.')
      {
         point_seen = 1;
         continue;
      };
      int d = *s - '0';
      if (d >= 0 && d <= 9)
      {
         if (point_seen) fact /= 10.0f;
         rez = rez * 10.0f + (float)d;
      };
   };
   return rez * fact;
}

int StringToInt(const char* s)
{
   if (*s == NULL)
      return 0;

   int res = 0,
      sign = 1,
      i = 0;

   if (s[0] == '-')
   {
      sign = -1;
      i++;
   }
   for (; s[i] != '\0'; ++i)
   {

      res = res * 10 + s[i] - '0';
   }
   return sign*res;
}