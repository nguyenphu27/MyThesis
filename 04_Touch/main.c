/**
	waiting GPIO22 set to 1 , create file 
 */

#include <stdio.h>

   
void main()
{
	FILE *fp;
  while (1)
  {
	 // when GPIO22 set to 1
        if(GPIO_GEN3==1)
        {
           // create new file
            fp = fopen("/cwd/touch_finish", "r"); 
	  break;
        }
  }
 while(1)
 {
	 
 }
	
}

