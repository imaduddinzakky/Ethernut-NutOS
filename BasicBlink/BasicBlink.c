#define F_CPU 16000000UL
#include <compiler.h>
#include <sys/timer.h>

int main (void)
{
	DDRB=0b11111111;
	PORTB=0b00000000;

	while(1){
	PORTB= 0b00000000;
	NutSleep(100);
	PORTB= 0b11111111;
	NutSleep(100);
	}

}