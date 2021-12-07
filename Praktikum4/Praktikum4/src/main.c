/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include <ioport.h>
#include <board.h>

#define USART_SERIAL_EXAMPLE             &USARTC0
#define USART_SERIAL_EXAMPLE_BAUDRATE    9600
#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT            false

static char reads[100];
char *str1 = "check\n\0";

void serial_init()
{
	USARTC0_BAUDCTRLB = 0;
	USARTC0_BAUDCTRLA = 0x0C;
	
	USARTC0_CTRLA = 0;
	USARTC0_CTRLC = USART_CHSIZE_8BIT_gc;
	
	USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

char receiveChar() {
	while (!(USARTC0_STATUS & USART_RXCIF_bm));
	return USARTC0_DATA;
}

void receiveString() {
	int i = 0;
	while(1){
		char inp = receiveChar();
		//char inp = usart_getchar(USART_SERIAL_EXAMPLE);
		if(inp=='\n') break;
		else reads[i++] = inp;
		delay_ms(1);
	}
	reads[i] = '\0';
}

void sendChar(char c) {
	while( !(USARTC0_STATUS & USART_DREIF_bm) ); //Wait until DATA buffer is empty
	USARTC0_DATA = c;
}

void sendString(char *text) {
	while(*text) {
		sendChar(*text++);
		//usart_putchar(USART_SERIAL_EXAMPLE, *text++);
		delay_ms(1);
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	gfx_mono_init();
	
	pmic_init();
	cpu_irq_enable();
	
	gpio_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);
	
	PORTC_OUTSET = PIN3_bm; // PC3 as TX
	PORTC_DIRSET = PIN3_bm; //TX pin as output
	
	PORTC_OUTCLR = PIN2_bm; //PC2 as RX
	PORTC_DIRCLR = PIN2_bm; //RX pin as input
	
	serial_init();
	
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	
	usart_init_rs232(USART_SERIAL_EXAMPLE, &USART_SERIAL_OPTIONS);
	
	gfx_mono_draw_string("Laser status:", 0, 0, &sysfont);
	
	while(1) {
		if (gpio_pin_is_high(GPIO_PUSH_BUTTON_1) && gpio_pin_is_low(GPIO_PUSH_BUTTON_2)){
			gpio_set_pin_high(LED0_GPIO);
			
			sendString(str1);
			
			gfx_mono_draw_string("sending...    ", 0, 0, &sysfont);
			gfx_mono_draw_string("              ", 0, 8, &sysfont);
			gpio_set_pin_low(LED0_GPIO);
			
			receiveString();
			
			gfx_mono_draw_string("Received:", 0, 8, &sysfont);
			gfx_mono_draw_string(reads, 0, 16, &sysfont);
			delay_ms(50);
		}
	}
}
