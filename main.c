#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

const uint PIN_BLUELED = 10;
const uint PIN_REDLED = 13;
const uint PIN_YELLOWLED = 14;
const uint PIN_GREENLED = 15;

const uint PIN_BTN = 9;

int main() {
	bi_decl(bi_program_description("This is a program to blink 3 LEDs."));
	bi_decl(bi_1pin_with_name(PIN_BLUELED, "External Blue LED"));
	bi_decl(bi_1pin_with_name(PIN_REDLED, "External Red LED"));
	bi_decl(bi_1pin_with_name(PIN_YELLOWLED, "External Yellow LED"));
	bi_decl(bi_1pin_with_name(PIN_GREENLED, "External Green LED"));
	bi_decl(bi_1pin_with_name(PIN_BTN, "External button to interupt sequence"));
	
	stdio_init_all();
	
	gpio_init(PIN_BLUELED);
	gpio_set_dir(PIN_BLUELED, GPIO_OUT);
	
	gpio_init(PIN_REDLED);
	gpio_set_dir(PIN_REDLED, GPIO_OUT);
	
	gpio_init(PIN_YELLOWLED);
	gpio_set_dir(PIN_YELLOWLED, GPIO_OUT);
	
	gpio_init(PIN_GREENLED);
	gpio_set_dir(PIN_GREENLED, GPIO_OUT);
	
	gpio_init(PIN_BTN);
	gpio_set_dir(PIN_BTN, GPIO_IN);
	gpio_pull_up(PIN_BTN);
	
	//states
	// 1 - blue / red
	// 2 - red
	// 3 - red yellow
	// 4 - green
	// 5 - yellow
	uint state = 2;
	uint curstate = 2;
	
	while(1) {	
		
		printf("State: %d\n", state);
		
		if(!gpio_get(PIN_BTN)) {
				puts("Button Pressed");
				gpio_put(PIN_BLUELED,1);
				state = 1;
		}
			
		switch(state) {
			case 1:				
				// finish stop sequence (if green)
				
				if (curstate == 3) {
					// red
					gpio_put(PIN_REDLED,1);
					gpio_put(PIN_YELLOWLED,0);
					gpio_put(PIN_GREENLED, 0);
					sleep_ms(2000);
				} else if (curstate == 4) {
					// yellow
					gpio_put(PIN_REDLED,0);
					gpio_put(PIN_YELLOWLED,1);
					gpio_put(PIN_GREENLED, 0);
					sleep_ms(2000);
					gpio_put(PIN_YELLOWLED,0);
					// red
					gpio_put(PIN_REDLED,1);
					sleep_ms(2000);
				} else if (curstate == 5) {
					// red
					gpio_put(PIN_REDLED,1);
					gpio_put(PIN_YELLOWLED,0);
					gpio_put(PIN_GREENLED, 0);
					sleep_ms(2000);
				} else {
					puts("ERROR state numbering not correct");
				}
				
				puts("flash for 10 seconds");
				for (int i = 0; i < 5; i++) {
					gpio_put(PIN_BLUELED,0);
					sleep_ms(1000);
					gpio_put(PIN_BLUELED,1);
					sleep_ms(1000);
				}
			
				gpio_put(PIN_BLUELED,0);
				gpio_put(PIN_REDLED,0);
				gpio_put(PIN_YELLOWLED,0);
				gpio_put(PIN_GREENLED, 0);
				state = 2;
				break;
			case 2:
				curstate = 2;
				gpio_put(PIN_BLUELED,0);
				gpio_put(PIN_REDLED,1);
				gpio_put(PIN_YELLOWLED,0);
				gpio_put(PIN_GREENLED, 0);
				state = 3;
				sleep_ms(2000);
				break;
			case 3:
				curstate = 3;
				gpio_put(PIN_BLUELED,0);
				gpio_put(PIN_REDLED,1);
				gpio_put(PIN_YELLOWLED,1);
				gpio_put(PIN_GREENLED, 0);
				state = 4;
				sleep_ms(2000);
				break;
			case 4:
				curstate = 4;
				gpio_put(PIN_BLUELED,0);
				gpio_put(PIN_REDLED,0);
				gpio_put(PIN_YELLOWLED,0);
				gpio_put(PIN_GREENLED, 1);
				state = 5;
				sleep_ms(2000);
				break;
			case 5:
				curstate = 5;
				gpio_put(PIN_BLUELED,0);
				gpio_put(PIN_REDLED,0);
				gpio_put(PIN_YELLOWLED,1);
				gpio_put(PIN_GREENLED, 0);
				state = 2;
				sleep_ms(2000);
				break;
		}
	
	}
}
