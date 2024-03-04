####################################################################################
# MIT License
#
# Copyright (c) 2024 jackkyyang
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
####################################################################################

# 操作系统入口
# 功能： 为操作系统设置栈空间，然后进入初始化函数
# 暂时不考虑多核启动的问题，只需要4KB的栈空间

    # 连接到代码段
   .section .text.entry
   .globl _entry
_entry:

    # 设置栈指针到栈顶
    la sp, boot_stack_top
    call start

    # 使用bss区作为初始化时的栈空间
   .section .bss.stack
   .globl boot_stack
boot_stack:
    # 大小为 4KB
   .space 4096
   .globl boot_stack_top
boot_stack_top: