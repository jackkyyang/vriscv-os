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

#include "typedef.h"
#include "riscv.h"
#include "sleep.h"

extern void kernelvec(); // 在kernelvec.S中声明

// 为用户模式申请的栈空间
__attribute__ ((aligned (16))) char u_stack[4096];

// 启动操作系统
void start(){

    // 设置中断向量地址
    w_mtvec((MXLEN_T)kernelvec);


    // mret需要退回到U模式
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_U;
    w_mstatus(x);

    // 退出M模式
    w_mepc((MXLEN_T)sleep);

    // 退出时先保存内核SP
    asm volatile("csrw sscratch, sp");

    w_sp((MXLEN_T)u_stack);
    asm volatile("mret");
}

