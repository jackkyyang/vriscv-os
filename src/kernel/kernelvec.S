        #
        # interrupts and exceptions while in supervisor
        # mode come here.
        #
        # the current stack is a kernel stack.
        # push all registers, call kerneltrap().
        # when kerneltrap() returns, restore registers, return.
        #
.globl kerneltrap
.globl kernelvec
.align 4
kernelvec:
        # make room to save registers.
        addi sp, sp, -128

        # save the registers.
        sd ra, 0(sp)
        sd sp, 4(sp)
        sd gp, 8(sp)
        sd tp, 12(sp)
        sd t0, 16(sp)
        sd t1, 20(sp)
        sd t2, 24(sp)
        sd s0, 28(sp)
        sd s1, 32(sp)
        sd a0, 36(sp)
        sd a1, 40(sp)
        sd a2, 44(sp)
        sd a3, 48(sp)
        sd a4, 52(sp)
        sd a5, 56(sp)
        sd a6, 60(sp)
        sd a7, 64(sp)
        sd s2, 68(sp)
        sd s3, 72(sp)
        sd s4, 76(sp)
        sd s5, 80(sp)
        sd s6, 84(sp)
        sd s7, 88(sp)
        sd s8, 92(sp)
        sd s9, 96(sp)
        sd s10, 100(sp)
        sd s11, 104(sp)
        sd t3, 108(sp)
        sd t4, 112(sp)
        sd t5, 116(sp)
        sd t6, 120(sp)

        # call the C trap handler in trap.c
        call kerneltrap

        # restore registers.
        ld ra, 0(sp)
        ld sp, 4(sp)
        ld gp, 8(sp)
        #在多核系统中，不需要restore tp (包括hartid), in case we moved CPUs
        ld tp, 12(sp)
        ld t0, 16(sp)
        ld t1, 20(sp)
        ld t2, 24(sp)
        ld s0, 28(sp)
        ld s1, 32(sp)
        ld a0, 36(sp)
        ld a1, 40(sp)
        ld a2, 44(sp)
        ld a3, 48(sp)
        ld a4, 52(sp)
        ld a5, 56(sp)
        ld a6, 60(sp)
        ld a7, 64(sp)
        ld s2, 68(sp)
        ld s3, 72(sp)
        ld s4, 76(sp)
        ld s5, 80(sp)
        ld s6, 84(sp)
        ld s7, 88(sp)
        ld s8, 92(sp)
        ld s9, 96(sp)
        ld s10, 100(sp)
        ld s11, 104(sp)
        ld t3, 108(sp)
        ld t4, 112(sp)
        ld t5, 116(sp)
        ld t6, 120(sp)

        addi sp, sp, 128

        # return to whatever we were doing in the kernel.
        mret

        #
        # machine-mode timer interrupt.
        #