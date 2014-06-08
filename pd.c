#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "/home/pi/m2x-c/m2x.h"
#include "/home/pi/m2x-c/feed.h"
#include "bcm2835.h"



// Blinks on RPi pin GPIO 11 when GPIO 15 is connected to push button and button is pressed
#define PIN RPI_GPIO_P1_11
#define PIN1 RPI_GPIO_P1_15

const char *M2X_KEY="c0e7b8d05a3ecae3e169f4809ca96f41";
const char *FEED_ID="6814d8d63196c96bdbc0f0ed3718397a";
const char *STREAM_NAME="test-data-stream";

int main()
{
    int status_data=0; 
    m2x_context *ctx = NULL;
	char buf[1024], *str = NULL; 

    if (!bcm2835_init()) 
		return 1;

    // Set the pin to be an output
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
	
	// Set RPI pin P1-15 to be an input
    bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    bcm2835_gpio_set_pud(PIN1, BCM2835_GPIO_PUD_UP);
    
    // blink and post to m2x
    while (status_data<1024)
    {
		if(bcm2835_gpio_lev(PIN1)==1)
		{
			// Turn it on
			bcm2835_gpio_write(PIN, HIGH);
			status_data++;
			sprintf(buf, "{\"value\":\"%d\"}", status_data);
			ctx = m2x_open(M2X_KEY);
			if ((m2x_feed_update_stream(ctx, FEED_ID, STREAM_NAME, buf, NULL) == 0) &&
			(m2x_feed_stream(ctx, FEED_ID, STREAM_NAME, &str) == 0)) 
			{
				printf("%s\n", str);
				m2x_free(str); 
			}
			m2x_close(ctx);
			//printf("I am high\n");
			// wait a bit
			delay(1000);
		}
		else
		{
			// turn it off
			bcm2835_gpio_write(PIN, LOW);
			//printf("I am low\n");
			// wait a bit
			delay(1000);
		} 
	}
     
	return 0;
}
