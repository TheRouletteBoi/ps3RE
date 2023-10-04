

/*

ccapi config
/dev_flash2/config.cfg     // how did I find this?  I searched for syscall num 0x321 (fs_open)  and breakpointer where it was being called and took a look at the r4 register which was address to the path :)


ccapi command id's:
0x2491   connect
0x10CAA  unknown
0x2492   get version   (ccapi version, frimware version) ???
0x314    get temperature
0x3341   notify
0x28AC   ring buzzer
0x359    set led
0x1527   get process list
0x1BA    write process memory
0xAF     set current idps & psid
0xF2     set boot idps & psid
0x4B9A   spoof OFW Mode
0x32BA   shut down ps3
0x287    get process name
0x2491   unknown
0x265E   unknown
0x16A    read process memory
0x13461  unknown
0x6001   unknown
0x240F   unknown
0x1D8B   disable tmapi & syscall8
0x100F   unknown
0x6BE    allocate page
0x1600   create thread


ccapi syscall command id's:
0x630   GetProcessList
0x700   GetProcessName
0x123   WriteProcessMemory
0x785   ReadProcessMemory
0x977   allocate_page
0x357   ppu_user_thread_create
0xCCC   disable tmapi & syscall8
0x780   set_idps
0x780   set_pdid
0x530   spoof_ofw_mode
0x241   enable ccapi sys calls (you can call ccapi func from a thread)
0x117   unknown
0x450   unknown
0x750   unknown


*/




/*
sys_ccapi_call(0x241);
sys_ccapi_call(0x117, 0x200);
sys_ccapi_call(0x450, r4, r5, r6, r7);
sys_ccapi_call(0x750, r4, r5, r6);
*/




/*
// bytes to scan for
sc = 44000002

// lv2 kernel
8000000000009EB8 = 38 60 00 01 intead of 54 63 06 3E with ccapi which forces 0x8000000000054694 return 1
800000000000A3B4 = 38 60 00 01 intead of 54 63 06 3E with ccapi which forces 0x8000000000054694 return 1


0x007BC1A8
0x007BBEE4
*/


/*
4.84 DEX

r4 new:  *(uint64_t *)(r4), *(uint64_t *)(r4 + 0x08)

ccapi_lv2_syscall
r3: 0x0000000000000241
r4: 0x00000000D0008D30
r5: 0x0000000000000000
r6: 0x0000000020600164
r7: 0x0000000000000000
r8: 0x0000000000040100
r9: 0x00000000007D4BF8
r10: 0x0000000000041D94
r4 new: 0x8000000000565000 0x00000000007586D8
ret: 0x0000000000000000

ccapi_lv2_syscall
r3: 0x0000000000000241
r4: 0x00000000D0008A20
r5: 0x0000000000000001
r6: 0x0000000000748938
r7: 0x0000000000748414
r8: 0x0000000000748418
r9: 0x00000000007D4BF8
r10: 0x0000000000000001
r4 new: 0x8000000000565000 0x00000000007586D8
ret: 0x0000000000000001

ccapi_lv2_syscall
r3: 0x0000000000000241
r4: 0x0000000020216828
r5: 0x0000000000000020
r6: 0x0000000000000002
r7: 0x0000000020040054
r8: 0x00000000200401C8
r9: 0x00000000007D4BF8
r10: 0x00000000010000E7
r4 new: 0x0000002000000021 0x2004005420040054
ret: 0x0000000000000000

ccapi_lv2_syscall
r3: 0x0000000000000241
r4: 0x0000000020216828
r5: 0x0000000000000020
r6: 0x0000000000000002
r7: 0x0000000020040054
r8: 0x00000000200401C8
r9: 0x00000000007D4BF8
r10: 0x00000000010000E9
r4 new: 0x0000002000000021 0x2004005420040054
ret: 0x0000000000000000

ccapi_lv2_syscall
r3: 0x0000000000000241
r4: 0x0000000020216828
r5: 0x0000000000000020
r6: 0x0000000000000002
r7: 0x0000000020040054
r8: 0x00000000200401C8
r9: 0x00000000007D4BF8
r10: 0x00000000010000EA
r4 new: 0x0000002000000021 0x2004005420040054
ret: 0x0000000000000000

ccapi_lv2_syscall
r3: 0x0000000000000241
r4: 0x0000000020216828
r5: 0x0000000000000020
r6: 0x0000000000000002
r7: 0x0000000020040054
r8: 0x00000000200401C8
r9: 0x00000000007D4BF8
r10: 0x00000000010000E8
r4 new: 0x0000002000000021 0x2004005420040054
ret: 0x0000000000000000




















when ps3 starts up

ccapi call disable tmapi
ccapi_lv2_syscall
r3: 0x785
r4: 0x0
r5: 0x10050
r6: 0x30
r7: 0x201910A0
r8: 0x201910D0
r9: 0x201910A0
r10: 0x1000034
ret: 0x0
ccapi_lv2_syscall
r3: 0x123
r4: 0x0
r5: 0x10050
r6: 0x30
r7: 0xD0033C40
r8: 0x201910A0
r9: 0x1
r10: 0x31
ret: 0x0
ccapi_lv2_syscall
r3: 0x785
r4: 0x0
r5: 0x35EBBC
r6: 0x4
r7: 0x201AA1C0
r8: 0x201AA1C4
r9: 0x201AA1C0
r10: 0x1000034
ret: 0x0
ccapi_lv2_syscall
r3: 0x123
r4: 0x0
r5: 0x35EBBC
r6: 0x4
r7: 0xD0033BD0
r8: 0x201AA1C0
r9: 0x2
r10: 0x5
ret: 0x0
ccapi_lv2_syscall
r3: 0x785
r4: 0x0
r5: 0x622634
r6: 0x4
r7: 0x201A59A0
r8: 0x201A59A4
r9: 0x201A59A0
r10: 0x1000034
ret: 0x0
ccapi_lv2_syscall
r3: 0x123
r4: 0x0
r5: 0x622634
r6: 0x4
r7: 0xD0033BD0
r8: 0x201A59A0
r9: 0x3
r10: 0x5
ret: 0x0
ccapi_lv2_syscall
r3: 0x241
r4: 0xD0008D30
r5: 0x0
r6: 0x20600164
r7: 0x0
r8: 0x40100
r9: 0x7D4BF8
r10: 0x41D94
ret: 0x0
ccapi_lv2_syscall
r3: 0x241
r4: 0xD0008A20
r5: 0x1
r6: 0x748938
r7: 0x748414
r8: 0x748418
r9: 0x7D4BF8
r10: 0x1
ret: 0x1
ccapi_lv2_syscall
r3: 0x241
r4: 0x201B22B8
r5: 0x20
r6: 0x2
r7: 0x20040054
r8: 0x200401C8
r9: 0x7D4BF8
r10: 0x101004B
ret: 0x0
ccapi_lv2_syscall
r3: 0x241
r4: 0x201B22B8
r5: 0x20
r6: 0x2
r7: 0x20040054
r8: 0x200401C8
r9: 0x7D4BF8
r10: 0x101005C
ret: 0x0
ccapi_lv2_syscall
r3: 0x241
r4: 0x201B22B8
r5: 0x20
r6: 0x2
r7: 0x20040054
r8: 0x200401C8
r9: 0x7D4BF8
r10: 0x101005D
ret: 0x0
ccapi_lv2_syscall
r3: 0x241
r4: 0x201B22B8
r5: 0x20
r6: 0x2
r7: 0x20040054
r8: 0x200401C8
r9: 0x7D4BF8
r10: 0x101005B
ret: 0x0
*/























/*
on ps3 start up 4.81 DEX

ccapi_lv2_syscall
r3: 0x241
r4: 0x7C3D40
r5: 0x770570
r6: 0x7734CC
r7: 0x7736E0
r8: 0x200401C8
r9: 0x7C5DD8
r10: 0x10000BF
r4 0: 0x0077057000770570
ret: 0x0

ccapi_lv2_syscall
r3: 0x241
r4: 0x7C3D40
r5: 0x770570
r6: 0x7734CC
r7: 0x7736E0
r8: 0x200401C8
r9: 0x7C5048
r10: 0x10000C0
r4 0: 0x0077057000770570
ret: 0x0

*/