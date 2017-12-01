/*******************************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/
/*!\mainpage Main Page
*
* \section intro_sec Introduction
*
* This is the code documentation for the MAXREFDES117# subsystem reference design.
* 
*  The Files page contains the File List page and the Globals page.
* 
*  The Globals page contains the Functions, Variables, and Macros sub-pages.
*
* \image html MAXREFDES117_Block_Diagram.png "MAXREFDES117# System Block Diagram"
* 
* \image html MAXREFDES117_firmware_Flowchart.png "MAXREFDES117# Firmware Flowchart"
*
*/
#include "stm32f103c8t6.h"
#include "mbed.h"
#include "string.h"

Serial pc(USBTX, USBRX);    //initializes the serial port
Serial pc1(USBTX1, USBRX1);    //initializes the serial port


char r[9];

// the setup routine runs once when you press reset:
int main() {
	pc.baud(9600);
  pc.format(8,SerialBase::None,1);
	int check=0;
	label:
	wait(1);
	while(1)
	{
		pc.gets(r,6);
		if(strcmp(r,"ID   ")==0)pc.printf("temp");
		if(strcmp(r,"start")==0)
		{
			pc.printf("start");
			break;
		}
		wait(1);
	}
	
	int i=0;
	int a=0;
	float ob,am=0;
	wait(1);
	pc1.baud(9600);
	pc1.format(8,SerialBase::None,1);
	wait(1);
	while(1){
		pc1.putc(0xa5);
		pc1.putc(0x15);
		pc1.putc(0xba);
		pc1.gets(r,10);

		for(i=0;i<8;i++)
		{
			a = r[i] + a;
		}
		a = a & 0xff;
		if(a == int(r[i]))
		{
			wait(1);
			ob=(float)(r[4]<<8|r[5])/100;
			pc.printf("%f",ob);
		}
		a=0;
		wait(1);
		pc.gets(r,5);
		wait(1);
		if(strcmp(r,"stop")==0)
		{
			pc.printf("stop");
			break;
		}
	}
	goto label;
}

