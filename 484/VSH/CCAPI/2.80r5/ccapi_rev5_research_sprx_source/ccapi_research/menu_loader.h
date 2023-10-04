CDECL uint32_t vshmain_0624D3AE();
#define GetGameProcessID vshmain_0624D3AE

#pragma comment(lib, "vshmain_export_stub")
#include <cell/pad.h>

uint64_t g_game_procces_id = 0;
bool bLoadMenu = true;
bool enableCCAPISysCall = true;
static void* vsh_pdata_addr = NULL;

// redefinition of pad bit flags
#define    PAD_SELECT    (1<<0)
#define    PAD_L3        (1<<1)
#define    PAD_R3        (1<<2)
#define    PAD_START     (1<<3)
#define    PAD_UP        (1<<4)
#define    PAD_RIGHT     (1<<5)
#define    PAD_DOWN      (1<<6)
#define    PAD_LEFT      (1<<7)
#define    PAD_L2        (1<<8)
#define    PAD_R2        (1<<9)
#define    PAD_L1        (1<<10)
#define    PAD_R1        (1<<11)
#define    PAD_TRIANGLE  (1<<12)
#define    PAD_CIRCLE    (1<<13)
#define    PAD_CROSS     (1<<14)
#define    PAD_SQUARE    (1<<15)



#define unk_title_id_var ""
#define unk_title_id_var_as_char ''

bool IsGameAW(char* cur_title_id)
{
   if (!strcmp(cur_title_id, unk_title_id_var "BLUS31466") || !strcmp(cur_title_id, unk_title_id_var "BLES02077") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES02078") || !strcmp(cur_title_id, unk_title_id_var "BLES02079") ||
      !strcmp(cur_title_id, unk_title_id_var "NPUB31553") || !strcmp(cur_title_id, unk_title_id_var "NPEB02087"))
   {
      return true;
   }

   return false;
}
bool IsGameBO1(char* cur_title_id)
{
   if (!strcmp(cur_title_id, unk_title_id_var "BLES01031") || !strcmp(cur_title_id, unk_title_id_var "BLES01032") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES01033") || !strcmp(cur_title_id, unk_title_id_var "BLES01034") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES01035") || !strcmp(cur_title_id, unk_title_id_var "BLES01105") ||
      !strcmp(cur_title_id, unk_title_id_var "BLUS30591"))
   {
      return true;
   }

   return false;
}

bool IsGameBO2(char* cur_title_id)
{
   if (!strcmp(cur_title_id, unk_title_id_var "BLUS31011") || !strcmp(cur_title_id, unk_title_id_var "BLES01717") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES01718") || !strcmp(cur_title_id, unk_title_id_var "BLES01719") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES01720") || !strcmp(cur_title_id, unk_title_id_var "BLUS31141") ||
      !strcmp(cur_title_id, unk_title_id_var "BLUS31140") || !strcmp(cur_title_id, unk_title_id_var "NPEB01205"))
   {
      return true;
   }

   return false;
}

bool IsGameBO3(char* cur_title_id)
{
   if (!strcmp(cur_title_id, unk_title_id_var "BLES02166") || !strcmp(cur_title_id, unk_title_id_var "BLES02168") ||
      !strcmp(cur_title_id, unk_title_id_var "NPUB31054") || !strcmp(cur_title_id, unk_title_id_var "NPUB31055") ||
      !strcmp(cur_title_id, unk_title_id_var "NPUB31056") || !strcmp(cur_title_id, unk_title_id_var "NPUB31665") ||
      !strcmp(cur_title_id, unk_title_id_var "NPEB02266") || !strcmp(cur_title_id, unk_title_id_var "BLUS31527"))
   {
      return true;
   }

   return false;
}

bool IsGameGhosts(char* cur_title_id)
{
   if (!strcmp(cur_title_id, unk_title_id_var "BLES01948") || !strcmp(cur_title_id, unk_title_id_var "BLES01945") ||
      !strcmp(cur_title_id, unk_title_id_var "BLUS31270") || !strcmp(cur_title_id, unk_title_id_var "NPUB31301") ||
      !strcmp(cur_title_id, unk_title_id_var "NPEB01832"))
   {
      return true;
   }

   return false;
}

bool IsGameMW3(char* cur_title_id)
{
   if (!strcmp(cur_title_id, unk_title_id_var "BLUS30838") || !strcmp(cur_title_id, unk_title_id_var "BLES01432") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES01428") || !strcmp(cur_title_id, unk_title_id_var "BLES01429") ||
      !strcmp(cur_title_id, unk_title_id_var "NPUB30787") || !strcmp(cur_title_id, unk_title_id_var "NPUB30788") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES01433") || !strcmp(cur_title_id, unk_title_id_var "BLES01431") ||
      !strcmp(cur_title_id, unk_title_id_var "NPEB00964"))
   {
      return true;
   }

   return false;
}

bool IsGameMW2(char* cur_title_id)
{
   if (!strcmp(cur_title_id, unk_title_id_var "BLUS30377") || !strcmp(cur_title_id, unk_title_id_var "BLES00683") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES00684") || !strcmp(cur_title_id, unk_title_id_var "BLES00685") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES00686") || !strcmp(cur_title_id, unk_title_id_var "BLES00687") ||
      !strcmp(cur_title_id, unk_title_id_var "BLES00690") || !strcmp(cur_title_id, unk_title_id_var "BLES00691") ||
      !strcmp(cur_title_id, unk_title_id_var "NPUB30585") || !strcmp(cur_title_id, unk_title_id_var "NPUB30586") ||
      !strcmp(cur_title_id, unk_title_id_var "NPEB00731"))
   {
      return true;
   }

   return false;
}


int32_t sys_get_game_application_title_id(char* title_id)
{
   system_call_1(986, (uint64_t)title_id);
   return_to_user_prog(int32_t);
}

char* GetCurrentGameID()
{
   char csTitle_id[64];
   sys_get_game_application_title_id(csTitle_id);

   // remove unknown var from title id
   // if (csTitle_id[0] == unk_title_id_var_as_char)
   //	memmove(csTitle_id, csTitle_id + 1, strlen(csTitle_id));

   return csTitle_id;
}

int _write_payload(uint32_t start_addr, const char* file)
{
   int next_write = 0;

   int fd = 0; uint64_t pos, read_e = 1;

   if (cellFsOpen(file, CELL_FS_O_RDONLY, &fd, NULL, 0) == CELL_FS_SUCCEEDED)
   {
      while (read_e)
      {
         char* data = (char*)malloc(BYTES_PER_WRITE);
         if (cellFsRead(fd, (void*)(data), BYTES_PER_WRITE, &read_e) == CELL_FS_SUCCEEDED)
         {
            CCAPIWriteProcessMemory(g_game_procces_id, (void*)(start_addr + next_write), data, read_e);
            next_write += read_e;
            if (data)
               free(data);
         }
      }

      cellFsClose(fd);
   }
   else
   {
      return ENOENT;
   }

   return 0;
}

bool inject_menu(const char *file_name, uint32_t file_size)
{
   int ret;
   if (enableCCAPISysCall)
   {
      ret = CCAPIEnableSysCall();
      printf("CCAPIEnableSysCall ret: 0x%X\n", ret);

      if (ret == 0)
      {
         // success, we don't need this option anymore
         enableCCAPISysCall = false;
      }

      if (ret != 0)
      {
         enableCCAPISysCall = false;
         printf("failed to enable ccapi syscall\n");
         printf("something wrong happened\n");
         return false;
      }
   }

   uint64_t kernel_adr, game_adr;
   ret = CCAPIAllocatePage(g_game_procces_id, file_size, 0x100, 0x2F, 0x1, &kernel_adr, &game_adr);
   if (ret != 0)
   {
      printf("failed to allocated\n");
      return false;
   }

   printf("kernel_adr: 0x%016llX\n", kernel_adr);
   printf("game_adr: 0x%016llX\n", game_adr);

   Sleep(10000);

   uint32_t temp_bytes;
   ret = CCAPIReadProcessMemory(g_game_procces_id, (void *)game_adr, (void *)&temp_bytes, 4);
   if (ret != 0)
   {
      printf("address not yet alloacted\n");
      return false;
   }

   Sleep(10000);

   ret = _write_payload(game_adr, file_name);
   if (ret != 0)
   {
      printf("failed to open file\n");
      return false;
   }

   Sleep(10000);

   uint32_t toc[2];
   toc[0] = game_adr;
   toc[1] = 0x00000000;
   thread_t th;
   ret = CCAPICreateProcessThread(g_game_procces_id, &th, toc, 0, 0x7D0, 0x4000, "temp_game_thread");
   if (ret != 0)
   {
      printf("failed to create thread for game\n");
      return false;
   }


   return true;
}

void VSHPadGetData(CellPadData* data)
{
   uint32_t pm_start = 0x10000UL;
   uint64_t pat[2] = { 0x380000077D3F4B78ULL, 0x7D6C5B787C0903A6ULL };

   if (!vsh_pdata_addr)        // first time, get address
   {
      while (pm_start < 0x700000UL)
      {
         if ((*(uint64_t*)pm_start == pat[0])
            && (*(uint64_t*)(pm_start + 8) == pat[1]))
         {
            vsh_pdata_addr = (void*)(uint32_t)(
               (int32_t)((*(uint32_t*)(pm_start + 0x234) & 0x0000FFFF) << 16) +
               (int16_t)(*(uint32_t*)(pm_start + 0x244) & 0x0000FFFF));

            // 0x00748954  4.84D
            printf("vsh_pdata_addr: 0x%X\n", vsh_pdata_addr);

            break;
         }

         pm_start += 4;
      }
   }

   memcpy(data, vsh_pdata_addr, sizeof(CellPadData));
}

void ccapi_menu_loader_thread(uint64_t arg)
{
   // change CCSC with HxD so it can work with CEX to

   // wait for vsh to start up
   Sleep(10000);

   printf("FMT Menu loader thread starting\n");

   /*
   ret = CCAPIEnableSysCall();
   if (ret != 0)
   {
      printf("failed to enable ccapi syscall x2\n");
      goto thread_exit;
   }
   else
   {
      // incompatible version but doesn't reach here becuase it will crash if incompatible
   }
   */

   bool bInjection_result = true;
   bool bWait_for_game_feedback = true;

   uint32_t uiOldpad = 0, uiCurpad = 0;
   CellPadData pData;

   while (true)
   {
      VSHPadGetData(&pData); // start scanning for address

      g_game_procces_id = GetGameProcessID();
      //printf("g_game_procces_id: 0x%X\n", g_game_procces_id);
      if (g_game_procces_id != 0)
      {
         if (bWait_for_game_feedback)
         {
            // 10 works fine on aw
            // 20 for bo2
            // 180 didn't work for bo3, I think it might be that we need the FMT eboot. UPDATE: it was that we needed the FMT eboot
            // 27 should be enough for all games
            for (int x = 0; x < (30 * 100); x++) //30 second delay
            {
               sys_timer_usleep(10000);
               sys_ppu_thread_yield();
            }

            bWait_for_game_feedback = false;
         }

         char *game_id = GetCurrentGameID();
         //printf("game region: %s\n", game_id);

         if (pData.len > 0)
         {
            uiCurpad = (pData.button[2] | (pData.button[3] << 8));
            if (uiCurpad != uiOldpad)
            {
               if ((uiCurpad & PAD_L1) && (uiCurpad & PAD_R2))
               {
                  printf("pressing L1 + R2\n");
                  if (uiCurpad & PAD_L3)
                  {
                     printf("pressing L3\n");

                     if (IsGameAW(game_id)) // Advance Warfare
                     {
                        printf("Advance Warfare inject started\n");
                        if (bLoadMenu)
                        {
                           bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/fury_by_enstone_220.bin", KB(496));
                           bLoadMenu = false;
                        }
                     }
                     else if (IsGameBO1(game_id)) // BO1
                     {
                        printf("Black Ops 1 inject started\n");
                        if (bLoadMenu)
                        {
                           bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/fusion_by_enstone_114.bin", KB(156));
                           bLoadMenu = false;
                        }
                     }
                     else if (IsGameBO2(game_id)) // BO2
                     {
                        printf("Black Ops 2 inject started\n");
                        if (bLoadMenu)
                        {
                           if (true) // load only destiny
                           {
                              bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/destiny_by_enstone_120.bin", KB(628));
                              //bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/destiny_by_enstone_unk.bin", KB(604));
                           }
                           else
                           {
                              bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/revolution_by_enstone_unk.bin", KB(224)); // fake number 224 and fake version _unk. we don't know becuase we never dumped it
                           }

                           bLoadMenu = false;
                        }
                     }
                     else if (IsGameBO3(game_id)) // BO3
                     {
                        printf("Black Ops 3 inject started\n");
                        if (bLoadMenu)
                        {
                           bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/fatality_by_enstone_102.bin", KB(444));
                           bLoadMenu = false;
                        }
                     }
                     else if (IsGameGhosts(game_id)) // Ghosts
                     {
                        printf("Ghosts inject started\n");

                        if (bLoadMenu)
                        {
                           bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/eternity_by_enstone_105.bin", KB(300));
                           bLoadMenu = false;
                        }
                     }
                     else if (IsGameMW3(game_id)) // MW3
                     {
                        printf("Modern Warfare 3 inject started\n");
                        if (bLoadMenu)
                        {
                           if (true) // load only phantom
                           {
                              bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/phantom_by_enstone_221.bin", KB(484));
                           }
                           else
                           {
                              bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/project_memories_by_enstone_388.bin", KB(112));
                           }

                           bLoadMenu = false;
                        }
                     }
                     else if (IsGameMW2(game_id)) // MW2
                     {
                        printf("Modern Warfare 2 inject started\n");
                        if (bLoadMenu)
                        {
                           bInjection_result = inject_menu("/dev_hdd0/tmp/enstone/reborn_by_enstone_446.bin", KB(140));
                           bLoadMenu = false;
                        }
                     }
                     else
                     {
                        // no other game cracks
                     }


                  }
               }
            }

            uiOldpad = uiCurpad;
         }
         else
         {
            uiOldpad = 0;
         }

         if (!bInjection_result) // if something failed then stop the thread
         {
            printf("injection failed\n");
            break;
         }

      }
      else
      {
         g_game_procces_id = 0;
         bLoadMenu = true;
      }



      Sleep(200); // 2000 10000  // smaller sleep for faster input on controller
   }


thread_exit:

   sys_ppu_thread_exit(0);
}
