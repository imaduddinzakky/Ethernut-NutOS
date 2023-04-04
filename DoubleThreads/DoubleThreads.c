#include <compiler.h>
#include <sys/timer.h>
#include <io.h>
#include <dev/board.h>
#include <sys/thread.h>

THREAD(Thread1, arg)
{
    while (1){
        PORTB = 0b00110000;
        NutSleep(100);
        PORTB = 0x00;
        NutSleep(100);
    }
}

THREAD(Thread2, arg)
{
    while (1){
        PORTB = 0b11000000;
        NutSleep(500);
        PORTB = 0x00;
        NutSleep(500);
    }
}

void TimerTick(uint32_t td)
{
    static uint32_t tick_count = 0;
    tick_count++;

}

int main(void)
{
    DDRB=0b11111111;
    PORTB=0b00000000;
    NutThreadCreate("t1", Thread1, 0, 512);
    NutThreadCreate("t2", Thread2, 0, 512);
    NutThreadSetPriority(254);
    NutThreadYield();

    while(1){
    NutSleep(1000);
    }
return 0;
}