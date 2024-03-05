/*
MIT License

Copyright (c) 2023 jackkyyang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



// ------------------------------------------------------------
// 00001000 -- start address of boot ROM, if it exists
// 00010000 -- start address of INTCTRL
// 00020000 -- start address of KeyBoard
// 00040000 -- start address of Screen
// 80000000 -- start address of DRAM, boot ROM jumps here in machine mode
//             -kernel loads the kernel here
// ------------------------------------------------------------

// 设备地址由VRISCV指定
//
#ifndef __MEM_LAYOUT_H__
#define __MEM_LAYOUT_H__

#define DRAM_BASE 0x80000000
#define DRAM_END  (DRAM_BASE + DRAM_SIZE - 1)

// 显示设备
// 显示器地址空间为256KB
#define SCR_BASE  0x00040000 // 256KB
#define SCR_END  (SCR_BASE + SCR_SIZE - 1)

// 键盘
// 键盘地址空间为4KB
#define KBD_BASE  0x00020000 //128KB
#define KBD_END  (KBD_BASE + KBD_SIZE - 1)

// INTCTRL
// 容量设置为 4KB
#define INTCTRL_BASE  0x00010000 // 64K
#define INTCTRL_END  (INTCTRL_BASE + INTCTRL_SIZE - 1)

// ROM
// 容量设置为 8KB
#define ROM_BASE  0x00001000
#define ROM_END  (KBD_BASE + ROM_SIZE - 1)


#define SCREEN_INT_ID 16
#define KBD_INT_ID 18

#endif // __MEM_LAYOUT_H__