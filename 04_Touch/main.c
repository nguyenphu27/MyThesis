/**
	waiting GPIO22 set to 1 , create file 
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main()
{
	FILE *fp;
  while (1)
  {
	  //Sleep still 2 second
	  sleep(2); 
	 // when GPIO22 set to 1
        if(GPIO_GEN3==1)
        {
           // create new file
            fp = fopen("/cwd/touch_finish", "w"); 
	  break;
        }
   }
 return 0;
 }
	
}

