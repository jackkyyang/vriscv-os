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
#include "print.h"
#include "mem_layout.h"
#include "kbd.h"
#include "intctrl.h"


// check if it's an external interrupt or software interrupt,
// and handle it.
// returns 2 if timer interrupt,
// 1 if other device,
// 0 if not recognized.
int devintr()
{
  MXLEN_T mcause = r_mcause();

  if((mcause & 0x8000000000000000L) &&
     (mcause & 0xff) == KBD_INT_ID){
    // this is a machine custom interrupt, defined by platform.

    // 处理前，清理中断值
    int_clr(KBD_INT_ID);

    kbd_int_proc();
    // the PLIC allows each device to raise at most one
    // interrupt at a time; tell the PLIC the device is
    // now allowed to interrupt again.
    return 1;
  }
  else {
    return 0;
  }
//   else if(mcause == 0x8000000000000001L){
//     // software interrupt from a machine-mode timer interrupt,
//     // forwarded by timervec in kernelvec.S.

//     if(cpuid() == 0){
//       clockintr();
//     }

//     // acknowledge the software interrupt by clearing
//     // the SSIP bit in sip.
//     w_sip(r_sip() & ~2);

//     return 2;
//   } else {
//     return 0;
//   }
}

// 中断处理
void kerneltrap() {
  int which_dev = 0;
  MXLEN_T mepc = r_mepc();
  MXLEN_T mstatus = r_mstatus();
  // MXLEN_T mcause = r_mcause();

  if((mstatus & MSTATUS_MPP_MASK) == MSTATUS_MPP_M)
    panic("kerneltrap: not from machine mode");

  // 处理中断时必须关闭中断使能
  if(mint_get() != 0)
    panic("kerneltrap: interrupts enabled");

  if((which_dev = devintr()) == 0){
    panic("kerneltrap, illegal interupt source!");
  }

  // the yield() may have caused some traps to occur,
  // so restore trap registers for use by kernelvec.S's sepc instruction.
  w_mepc(mepc);
  w_mstatus(mstatus);
}
