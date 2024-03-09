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
#include "mem_layout.h"
#include "kbd.h"
#include "print.h"


// 硬件约定好的数据结构
// 来自VRiscV
//键盘缓冲头数据结构
// 软件在使用时，必须先lock住缓冲区，然后才可以读后面的数据
// 在完成数据处理后，需要软件主动清空缓冲区内容
typedef struct kbd_buf_h_t
{
    uint32_t kbd_buf_lock; // 软件配置值，如果lock为1，则禁止设备向buf内写入数据
    uint32_t kbd_data_num; // 软件可读写，0, 代表没有有效数据
}KeyBoardBufferH;

// 键盘初始化
void kbd_init(){}

// 处理键盘中断
// trap 入口已经保存上下文，这里只需要处理
void kbd_int_proc(){

    // 获取键盘缓冲区头的数据结构
    KeyBoardBufferH* kbd_header = (KeyBoardBufferH*)(KBD_BASE);
    uint32_t* kbd_data_buf = (uint32_t*)(KBD_BASE + sizeof(KeyBoardBufferH));

    // 向缓冲区加锁，禁止硬件写入新的值
    kbd_header->kbd_buf_lock = 1;
    __sync_synchronize(); // 确保写成功后才能进行后面的操作

    // 获得缓冲区内数据的数量
    uint32_t kbd_data_num = kbd_header->kbd_data_num;
    __sync_synchronize(); // 确保读成功后才能进行后面的操作

    // 处理缓冲区内容
    for (uint32_t i = 0; i < kbd_data_num; i++)
    {
        printf("Press key: %x\n",kbd_data_buf[i]);
    }
    __sync_synchronize();
    kbd_header->kbd_data_num = 0;
    __sync_synchronize();
    kbd_header->kbd_buf_lock = 0;
    __sync_synchronize();

}


