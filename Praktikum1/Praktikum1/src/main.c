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

static char strbuf[128];

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	//init board
	board_init();

	//init lcd
	gfx_mono_init();

	//set background lcd on
	gpio_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);

	//set display - output lcd
	gfx_mono_draw_string("Sistem Tertanam 21/22",0, 0, &sysfont);
	gfx_mono_draw_string("Praktikum 01 : I/O",0, 8, &sysfont);
	
	//toggle flag 1=low, 0=high
	int flagLed0=1;
	int flagLed1=1;
	int flagBuzzer=0;

	uint64_t counter = 0;

	//toggle led. if 0 > 1, if 1 > 0
	gpio_toggle_pin(LED0);
	gpio_toggle_pin(LED1);
	gpio_toggle_pin(LED2);
	
	//delay 1000ms = 1s
	delay_ms(1000);
	
	//toggle led. if 0 > 1, if 1 > 0
	gpio_toggle_pin(LED0);
	gpio_toggle_pin(LED1);
	gpio_toggle_pin(LED2);
	
	//set J1 Pin 0 to output
	ioport_set_pin_dir(J1_PIN0, IOPORT_DIR_OUTPUT);
	
	flagLed0=1;
	flagLed1=0;

	//forever loop
	while(1){
		//check if button pressed
		if(ioport_get_pin_level(GPIO_PUSH_BUTTON_0)==0){
			flagLed0 = (flagLed0==0) ? 1 : 0;	
			flagLed1 = (flagLed1==0) ? 1 : 0;
		}//check if button pressed
		if(ioport_get_pin_level(GPIO_PUSH_BUTTON_1)==0){
			flagBuzzer = (flagBuzzer==0) ? 1 : 0;
		}//check if button pressed
		if(ioport_get_pin_level(GPIO_PUSH_BUTTON_2)==0){
			counter = counter+1;
		}
		//set pin based on flag.
		ioport_set_pin_level(LED0_GPIO,flagLed0);
		ioport_set_pin_level(LED1_GPIO,flagLed1);
		ioport_set_pin_level(J1_PIN0, flagBuzzer);

		snprintf(strbuf, sizeof(strbuf), "Counter : %3d",counter); // Show counter to lcd
		gfx_mono_draw_string(strbuf,0, 16, &sysfont);
		
		//delay for every loop
		delay_ms(50);
	}
}
