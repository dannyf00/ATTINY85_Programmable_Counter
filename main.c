//dividing clock using Timer0 CTC mode
//User specifies three parameters:
// 1. Fin_PS: input prescaler. 1..256
// 2. Fin_DIV: output divider ratio
// 3. Fin_OFFSET: output offset between OC0A and OC0B
//
//Input and Output:
// Input on CLKi/PB3 pin
// Output on OC0A/PB0 and OC0B/PB1, with an offset determined by Fin_OFFSET
//
// Output Frequency = Input frequency / Fin_PS / Fin_DIV
//
//
//
//#include <avr/io.h>							//we use gcc-avr
#include "gpio.h"
#include "delay.h"							//we use software delays
#include "tmr0.h"							//we use tmr0

//hardware configuration
#define Fin_DIV			250					//Input frequency divide ratio
#define Fin_OFFSET		10					//output offset between OC0A/OC0B. between 1..Fin_DIV
#define Fin_PS			TMR0_PS1x			//Input frequency prescaler, _PS1x/_PS8x/_PS_64x/_PS_256x/_PS_1024x
#define SYNC_OUT							//sync pin activated on PB2. PB2 is flipped on compare match -> at the same frequency at OCR0A/OCR0B
//end hardware configuration

//global defines


//global variables

//output sync handler to flip PB2 on compare match to OCR0B
void sync_isr(void) {
	IO_FLP(PORTB, 1<<2);					//flip PB2
}

int main(void) {

	mcu_init();								//reset the mcu
	IO_OUT(DDRB, (1<<1) | (1<<0));			//Output on OC0A/PB0 and OC0B/PB1, initially low and in sync
	//tmr0 CTC mode, 1x prescaler, divide by 200
	tmr0_init(Fin_PS, Fin_DIV, Fin_OFFSET);	//reset timer0

#if defined(SYNC_OUT)
	IO_OUT(DDRB, 1<<2);						//output enabled on PB2 to indicate sync
	tmr0_act(sync_isr);						//instal output sync handler
	ei();									//enable interrupt
#endif

	while(1) {
	}

	return 0;
}
