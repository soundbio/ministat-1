/******************************************************************************
* Project Name		: UART_Bootloader
* File Name			: main.c
* Version 			: 1.0
* Device Used		: CY8C4245AXI-483
* Software Used		: PSoC Creator 3.0
* Compiler    		: ARMGCC 4.7.3, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT-049-42xx PSoC 4 Prototyping Kit 
*
********************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

/*********************************************************************************
*                           THEORY OF OPERATION
* The Bootlader uses UART interface to bootload a new project. The Switch on the 
* board needs to be pressed during power up to enter into the bootlader. The LED 
* on the board blinks every 500ms to indicate that the bootloader is active and 
* is ready for a bootload operation.
* Default UART Port Configuration for bootloading the PSoC 4 in CY8CKIT-049-42xx
* Baud Rate : 115200 bps
* Data Bits : 8
* Stop Bits : 1
* Parity    : None
* 
* Note: Debugging is disabled in the System DWR Editor. To enable debugging, change
*       the Debug Select to SWD in the System tab of the UART_Bootloader.cydwr file 
***********************************************************************************/

#include <project.h>

/* Define the Bootload Switch timeout in ms */
#define SWITCH_PRESS_TIMEOUT 100

int main()
{
    
	uint8 counter = 0;	
	
	/* Enable global interrupts*/
	CyGlobalIntEnable;
	
	/* Check if the switch is pressed during power up */ 
	if(Boot_P0_7_Read() == 0)
	{
		
		for(counter = 0; counter <= SWITCH_PRESS_TIMEOUT; counter++)
		{
			/* Delay for 1ms */
			CyDelay(1);
				
			/* If the switch is released before specified time, do not enter the 
			 * bootloader */
			if(Boot_P0_7_Read() != 0)
				break;
		}
						 
		if(counter > SWITCH_PRESS_TIMEOUT)
		{
			/* If the switch was pressed for more than 100 millisecond counter 
			 * value will be 100. If so, set the flash run type as bootloader to 
			 * wait for a bootload operation */
			Bootloader_SET_RUN_TYPE (Bootloader_START_BTLDR);
		}
	}
		
	/*Indicate that you have entered the bootloader mode.*/
	PWM_Start();
	
	/* Start the Bootloader */
	Bootloader_Start();
	/* The “Bootloader_Start()” API will either wait for a bootload operation or 
	 * will launch the application depending on whether or not the API
	 * "Bootloader_SET_RUN_TYPE(Bootloader_START_BTLDR)" was called because of the 
	 * switch being pressed during power up */
			
    for(;;)
    {
        
    }
}

/* [] END OF FILE */
