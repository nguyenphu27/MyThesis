/**
	waiting GPIO17 set to 1 , create file 
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <bcm2835.h>
// Input on RPi pin GPIO 17
#define PIN RPI_GPIO_P1_11
int main()
{
  try
  {
//If you call this, it will not actually access the GPIO
// Use for testing
// bcm2835_set_debug(1);
	if (!bcm2835_init())
	return 1;
	   // Set the pin to be an output
  //  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
 
// Set RPI pin P1-11 to be an input
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);	  
	 FILE *fp;
    while (1)
    {
	    // Turn it on
	//bcm2835_gpio_write(PIN, HIGH);
	    uint8_t value = bcm2835_gpio_lev(PIN);
	    // when GPIO17 set to 1
            if(value==1)
            {
            // create new file
                fp = fopen("./touch_finish", "w"); 	
	        break;
             }
	   //Sleep still 2 second
	    sleep(2);  
      }
	  bcm2835_close();
	
  }
  catch (excecption &ex)
    {
        cout << "ERROR CODE : " << ex;
    }
 return 0;
 }
	
}

