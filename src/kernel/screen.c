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

#include "screen.h"
#include "riscv.h"
#include "typedef.h"
#include "mem_layout.h"

//TODO, 控制光标位置，并允许在光标位置插入数据

//帧缓冲区头数据结构
typedef struct frm_buf_h_t
{
    // 软件配置值，如果lock为1，代表软件正在处理数据，不要让设备做内容搬运
    uint8_t  frm_buf_lock;
    uint8_t  frm_buf_change; // 0, 没有改变，不需要搬运帧缓冲区
    uint32_t frm_data_num; // 0, 代表没有有效数据
    uint16_t screen_width;
    uint16_t screen_height;
    uint32_t frame_buf_max_len;
}FrameBufferH;

void putc_screen(const char c){
    FrameBufferH* screen_header = (FrameBufferH*)(SCR_BASE);
    uint8_t* screen_frm_buf_base = (uint8_t*)((SCR_BASE) + sizeof(FrameBufferH));

    screen_header->frm_buf_lock = 1;
    __sync_synchronize(); // 确保写成功后才能进行后面的操作

    uint8_t* frm_buf_end = screen_frm_buf_base + screen_header->frm_data_num;
    *frm_buf_end = c;
    if ((MXLEN_T)frm_buf_end == (SCR_BASE + SCR_SIZE - 1))
    {
        screen_header->frm_data_num = 0;
    }
    else {
        screen_header->frm_data_num += 1;
    }
    screen_header->frm_buf_change = 1;

    __sync_synchronize(); // 确保写成功后才能进行后面的操作
    screen_header->frm_buf_lock = 0;

}

void del_char()
{
    FrameBufferH* screen_header = (FrameBufferH*)(SCR_BASE);
    uint8_t* screen_frm_buf_base = (uint8_t*)((SCR_BASE) + sizeof(FrameBufferH));
    screen_header->frm_buf_lock = 1;
    __sync_synchronize(); // 确保写成功后才能进行后面的操作
    if (screen_header->frm_data_num > 0)
    {
        uint8_t* frm_buf_end = screen_frm_buf_base + screen_header->frm_data_num - 1;
        *frm_buf_end = 0;
        screen_header->frm_data_num -=1;
    }
    screen_header->frm_buf_change = 1;
    __sync_synchronize(); // 确保写成功后才能进行后面的操作
    screen_header->frm_buf_lock = 0;

}
