#include <compiler.h>
#include <sys/timer.h>
#include <io.h>
#include <dev/board.h>
#include <sys/thread.h>
#include <cfg/crt.h>    /* Floating point configuration. */
#include <string.h>
#include <stdio.h>

static char *banner = "\nNut/OS UART Sample " __DATE__ " " __TIME__"\n";
static prog_char presskey_P[] = "Press any key...";
static prog_char pgm_ptr[] = "\nHello stranger!\n";

static char inbuf[128];
int got;
char *cp;
uint32_t baud = 115200;
FILE *uart;

THREAD(Thread1, arg)
{
    while (1){
        PORTB = 0b10000000;
        NutSleep(500);
        PORTB = 0x00;
        NutSleep(100);
    }
}

THREAD(Thread2, arg)
{
    while (1){
        PORTB = 0b01000000;
        NutSleep(500);
        PORTB = 0x00;
        NutSleep(200);
    }
}

THREAD(Thread3, arg)
{
    while (1){
        PORTB = 0b00100000;
        NutSleep(500);
        PORTB = 0x00;
        NutSleep(300);
    }
}

THREAD(Thread4, arg)
{
    while (1){
        PORTB = 0b00010000;
        NutSleep(500);
        PORTB = 0x00;
        NutSleep(400);
    }
}

THREAD(Thread5, arg)
{
	for (;;) {
	fputs("\nEnter your name: ", uart);
        fflush(uart);
        fgets(inbuf, sizeof(inbuf), uart);

        /*
         * Chop off trailing linefeed.
         */
        cp = strchr(inbuf, '\n');
        if (cp)
            *cp = 0;

        /*
         * Streams support formatted output as well as printing strings
         * from program space.
         */
        if (inbuf[0])
            fprintf(uart, "\nHello %s!\n", inbuf);
        else {
            fputs_P(pgm_ptr, uart);
        }

        /*
         * Just to demonstrate formatted floating point output.
         * In order to use this, we need to link the application
         * with nutcrtf instead of nutcrt for pure integer.
         */
	#ifdef STDIO_FLOATING_POINT
        dval += 1.0125;
        fprintf(uart, "FP %f\n", dval);
	#endif
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
	
	
	#ifdef STDIO_FLOATING_POINT
    float dval = 0.0;
	#endif
	NutRegisterDevice(&DEV_CONSOLE, 0, 0);
	uart = fopen(DEV_CONSOLE.dev_name, "r+");
	_ioctl(_fileno(uart), UART_SETSPEED, &baud);
	_write(_fileno(uart), banner, strlen(banner));
    {
        _write_P(_fileno(uart), presskey_P, sizeof(presskey_P));
    }
	_write(_fileno(uart), 0, 0);
	got = _read(_fileno(uart), inbuf, sizeof(inbuf));
    _write(_fileno(uart), inbuf, got);
	
	
    NutThreadCreate("t1", Thread1, 0, 512);
    NutThreadCreate("t2", Thread2, 0, 512);
    NutThreadCreate("t3", Thread3, 0, 512);
	NutThreadCreate("t4", Thread4, 0, 512);
	NutThreadCreate("t5", Thread5, 0, 512);
	NutThreadSetPriority(254);

    NutThreadYield();
    while(1){
    NutSleep(1000);
    }
return 0;
}