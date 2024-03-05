
#include "typedef.h"
#include "riscv.h"
#include "sleep.h"

void sleep(){
    MXLEN_T sleep_count = 0;
    while (1)
    {
        sleep_count ++;
    }
}