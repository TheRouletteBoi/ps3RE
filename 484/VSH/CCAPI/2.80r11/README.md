Control Console API V2.80r11 made by Enstone

Table of content:
 1. CCAPI installation
 2. CCAPI loading


1. CCAPI installation
Upon running the CCAPI installer you'll be prompted with a choice [Yes/No] asking "Do you want to *INSTALL* CCAPI ?".
If 'Yes' is selected, the installer will mount /dev_flash as /dev_enstone to make it writeable, it then renames
'/dev_enstone/sys/internal/sys_audio.sprx' to '/dev_enstone/sys/internal/sys_plugin.sprx' and finally copy the
embedded ccapi.sprx as '/dev_enstone/sys/internal/sys_audio.sprx'.

2. CCAPI loading
As ccapi.sprx replaces sys_audio.sprx, it is consequently loaded at boot, as the first vsh module. 

Entry point:
 - Check if the thread id (r13) is null, if it is, it returns.
 - Create a non-joinable thread to initialize ccapi.
 - Check if the toc (r2) is null, if it isn't, ccapi will load sys_plugin.sprx which is the original sys_audio.sprx.

Initialisation thread:
 - Check if the thread id (r13) is null, if it is, it returns.
 - Sleep 16 seconds.
 - Check if '/dev_usb000/noccapi' exists, if it does, it returns. This check acts as a failsafe to not load ccapi in 
   case something goes wrong.
 - Verify that ccapi.sprx runs as '/dev_flash/sys/internal/sys_audio.sprx' by getting the current module handle and 
   informations.
 - Apply some patches to LV1 (hypervisor) depending on the current firmware version
 - Load an LV2 (kernel) payload:
   - Write stage0 at 0x8000000000002000 which will serve as a loader for the dynamic stage2.
   - Write a function descriptor at 0x80000000000019F0 pointing to the entry of stage0.
   - Backup and redirect syscall 491 function descriptor (unimplemented by Sony) to the stage0 entry descriptor.
   - Call syscall 491 with arguments 
     - r3: kernel offset table for the current firmware
     - r4: stage2 address in userland
     - r5: stage2 size
     - r6: a pointer to a stack variable
   - Restore the function descriptor of syscall 491
   - Set back the stage0 entry descriptor to 0
   - Fill the stage0 area (0x8000000000002000) with 0
 - Check if custom ccapi calls works (ccsc)
 - Load vsh modules from '/dev_usb000/plugins/'
 - Wait until network has been initialized
 - Install CCAPI hooks into VirtualShell (vsh.self)
 - Notify that CCAPI has been loaded
 - Spawn CCAPI server threads

Stage0:
 - Write to LV1 (hypervisor) HTAB
 - Allocate enough memory for stage2
 - Copy stage2 from userland to LV2 (kernel)
 - Relocate the payload variables by checking for 0xEBADADDC00000000 which is the base of the payload
 - Flush instruction cache
 - Execute the stage2
 - Call sub_8000000000068630 ?

Stage2:
 - to be continued