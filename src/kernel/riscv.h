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

#ifndef __RISCV_H__
#define __RISCV_H__

#include "typedef.h"

typedef uint32_t MXLEN_T;
#define MXLEN 32

// 读取硬件hart ID
static inline MXLEN_T r_mhartid() {
  MXLEN_T x;
  asm volatile("csrr %0, mhartid" : "=r" (x) );
  return x;
}

// mstatus 掩码
#define MSTATUS_MPP_MASK (3U << 11) // previous mode.
#define MSTATUS_MPP_M (3U << 11)
#define MSTATUS_MPP_S (1U << 11)
#define MSTATUS_MPP_U (0U << 11)
#define MSTATUS_MIE (1U << 3)    // machine-mode interrupt enable.

static inline MXLEN_T r_mstatus() {
  MXLEN_T x;
  asm volatile("csrr %0, mstatus" : "=r" (x) );
  return x;
}

static inline void w_mstatus(MXLEN_T x) {
  asm volatile("csrw mstatus, %0" : : "r" (x));
}

// 打开M模式全局中断使能
static inline void mint_on() {
  w_mstatus(r_mstatus() | MSTATUS_MIE);
}

// 关闭M模式全局中断使能
static inline void mint_off() {
  w_mstatus(r_mstatus() & ~MSTATUS_MIE);
}

// 获取M模式全局中断使能值
// 0: mstatus.mie == 0
// 1: mstatus.mie == 1
static inline int mint_get() {
  MXLEN_T x = r_mstatus();
  return (x & MSTATUS_MIE) != 0;
}

// 每个中断的独立使能
#define MIE_MEIE (1L << 11) // external
#define MIE_MTIE (1L << 7)  // timer
#define MIE_MSIE (1L << 3)  // software

// 获取MIE寄存器值
static inline MXLEN_T r_mie() {
  MXLEN_T x;
  asm volatile("csrr %0, mie" : "=r" (x) );
  return x;
}

// 设置每个中断的独立使能 (mie)
static inline void w_mie(MXLEN_T x) {
  asm volatile("csrw mie, %0" : : "r" (x));
}

// 设置MEPC
// 用于指定MRET的返回地址
static inline void w_mepc(MXLEN_T x) {
  asm volatile("csrw mepc, %0" : : "r" (x));
}

// 获取MEPC
// 在trap的时候保存发生异常或者被中断的指令地址
static inline MXLEN_T r_mepc() {
  MXLEN_T x;
  asm volatile("csrr %0, mepc" : "=r" (x) );
  return x;
}

// 获取异常/中断编号
static inline MXLEN_T r_mcause() {
  MXLEN_T x;
  asm volatile("csrr %0, mcause" : "=r" (x) );
  return x;
}

// 设置M模式的中断入口地址
static inline void w_mtvec(MXLEN_T x) {
  asm volatile("csrw mtvec, %0" : : "r" (x));
}

// 写入 mscratch
static inline void w_mscratch(MXLEN_T x) {
  asm volatile("csrw mscratch, %0" : : "r" (x));
}

// 获取 mscratch
static inline MXLEN_T r_mscratch() {
  MXLEN_T x;
  asm volatile("csrr %0, mscratch" : "=r" (x) );
  return x;
}

// 获取 mtval
static inline MXLEN_T r_mtval() {
  MXLEN_T x;
  asm volatile("csrr %0, mtval" : "=r" (x) );
  return x;
}

// 获取当前栈指针
static inline MXLEN_T r_sp()
{
  MXLEN_T x;
  asm volatile("mv %0, sp" : "=r" (x) );
  return x;
}

// // 写入栈指针
static inline void w_sp(MXLEN_T x) {
  asm volatile("mv sp, %0" : : "r" (x));
}

// 获取 time
static inline MXLEN_T r_time() {
  MXLEN_T x;
  asm volatile("csrr %0, time" : "=r" (x) );
  return x;
}

#endif