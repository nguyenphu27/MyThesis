/**
	waiting GPIO22 set to 1 , create file 
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main()
{
  try
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
                fp = fopen("./touch_finish", "w"); 
	
	        break;
             }
      }
	  
  }
  catch (excecption &ex)
    {
        cout << "ERROR CODE : " << ex;
    }
 return 0;
 }
	
}

