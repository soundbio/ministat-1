/******************************************************************************
* Project Name		: UART_Bootloader
* File Name			: UART_Btldr.c
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
/*******************************************************************************
* Description:
* This file contains the bootloader communication functions implemented using 
* UART. Some portion of this code depends on the code placed in UART receive 
* interrupt routine in main.c file.
********************************************************************************/

#include "UART.h" 
#include "project.h"

/* Byte to Byte time out in ms for detecting end of block data from host*/
#define BYTE2BYTE_TIME_OUT 2

#define BTLDR_SIZEOF_READ_BUFFER 0xff
#define BTLDR_SIZEOF_WRITE_BUFFER 0xff
#define END_OF_PACKET 0x17

/***************************************
*Global variables
***************************************/

/* UART receive data count variable  */
volatile uint16 receivedDataCount=0;

/*******************************************************************************
* Function Name: CyBtldrCommStart
********************************************************************************
*
* Summary:
*  Starts the UART communication component 
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Side Effects:
*   None  
*
*******************************************************************************/
void CyBtldrCommStart(void)
{        
	/* Start UART component and clear the TX,RX buffers */
	UART_Start();
	UART_SpiUartClearRxBuffer();
	UART_SpiUartClearTxBuffer();
}

/*******************************************************************************
* Function Name: CyBtldrCommStop
********************************************************************************
*
* Summary:
*  Disable the component.
*
* Parameters:  
*  void 
*
* Return: 
*  void 
*
*******************************************************************************/
void CyBtldrCommStop(void) 
{
    /* Stop UART */
    UART_Stop();
}

/*******************************************************************************
* Function Name: CyBtldrCommReset
********************************************************************************
*
* Summary:
*  Resets the receive and transmit communication Buffers.
*
* Parameters:  
*  void 
*
* Return: 
*  void 
*
* Side Effects:
*  
*******************************************************************************/
void CyBtldrCommReset(void)
{
    /* Clear RX and TX buffers */
    UART_SpiUartClearRxBuffer();
    UART_SpiUartClearTxBuffer();
}

/*******************************************************************************
* Function Name: CyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Allows the caller to write data to the bootloader host. This function uses 
* a block write function for writing data using UART communication component. 
*
* Parameters:
*  pData:    A pointer to the block of data to send to the device
*  size:     The number of bytes to write.
*  count:    Pointer to an unsigned short variable to write the number of
*             bytes actually written.
*  timeOut:  Number of units to wait before returning because of a timeout.
*
* Return: 
*   cystatus: This function will return CYRET_SUCCESS if data is sent successfully.
*
* Side Effects:
*  This function should be called after command was received .
*
*******************************************************************************/
cystatus CyBtldrCommWrite(uint8 * pData, uint16 Size, uint16 * Count, uint8 TimeOut)
{
    cystatus status = CYRET_EMPTY;
	
    uint8 timeOutms = TimeOut; /* This timeout is not used in this function, defined to avoid compiler warning */   
	timeOutms += 10;
	
   	receivedDataCount=0;
    
	/* Write TX data using blocking function */
	UART_SpiUartPutArray(pData, (uint8)Size);
	
	/* Return success code */
	*Count = Size;
	status = CYRET_SUCCESS;
	
    return status;
}

/*******************************************************************************
* Function Name: CyBtldrCommRead
********************************************************************************
*
* Summary:
*  Receives the command. 
*
* Parameters:  
*  pData:    A pointer to the area to store the block of data received
*             from the device.
*  size:     Maximum size of the read buffer
*  count:    Pointer to an unsigned short variable to write the number
*             of bytes actually read.
*  timeOut:  Number of units to wait before returning because of a timeOut.
*            Timeout is measured in 10s of ms.
*
* Return: 
*  cystatus: This function will return CYRET_SUCCESS if at least one byte is received
*			 successfully within the timeout interval. If no data is received this 
*			 function will return CYRET_EMPTY.
*
* Theory: 
*  'receivedDataCount' is updated with number of bytes received in the UART RX 
*  interrupt routine. This variable is used to check whether some data is received 
*  within the timeout period specified in *.cydwr. If data is received before the timeout, 
*  the control will remain in another loop waiting for more data until no data is 
*  received for a BYTE2BYTE_TIME_OUT(2 ms) interval.
*
*  Note: Increase the BYTE2BYTE_TIME_OUT to 10 ms for baud rates less than 9600.  
* 
*  BYTE2BYTE_TIME_OUT is used for detecting timeout marking end of block data from host. 
*  This has to be set to a value which is greater than the expected maximum delay 
*  between two bytes during a block/packet transmission from the host. 
*  You have to account for the delay in hardware converters while calculating this value,
*  if you are using any USB-UART bridges.   
*******************************************************************************/
cystatus CyBtldrCommRead(uint8 * pData, uint16 Size, uint16 * Count, uint8 TimeOut)
{
    uint16 cntr,dataIndexCntr;
    uint16 tempCount,oldDataCount;
	
    cystatus status = CYRET_EMPTY;

    /* Check whether data is received within the timeout period. 
	*  Timeout period is in units of 10ms.
	*  If at least one byte is received within the timeout interval, wait for more data */
	for (cntr = 0; cntr < TimeOut*10; cntr++)
    {
	    receivedDataCount = UART_SpiUartGetRxBufferSize();
		
		/* If at least one byte is received within the timeout interval enter the next loop
		* waiting for more data reception */
		if(receivedDataCount!=0) 
	   	{
			/* Wait for more data until 2 ms byte to byte time out interval receivedDataCount 
			* variable is updated in on each data reception. If no data is received during the 
			* last 2 ms (BYTE2BYTE_TIME_OUT) then it is considered as end of transmitted data 
			* block (packet) from the host and the program execution will break from the data 
			* awaiting loop with status=CYRET_SUCCESS */
			do{
			   	oldDataCount = receivedDataCount;
				CyDelay(BYTE2BYTE_TIME_OUT);
				receivedDataCount = UART_SpiUartGetRxBufferSize();			    
			}while(receivedDataCount > oldDataCount);
			status = CYRET_SUCCESS;	
			break;
		}
		/* If no data is received, give a delay of 1ms and check again until the Timeout specified in .cydwr. */
		else 
		{
			CyDelay(1);
		}
    }
	
	/* Initialize the data read indexes and Count value*/
	*Count = 0;
	dataIndexCntr = 0;
	
	/* If receivedDataCount>0 , move the received data to the pData buffer */
	while(receivedDataCount > 0)
	{
		tempCount=receivedDataCount;
		*Count  =(*Count ) + tempCount;
		
		/* Check if buffer overflow will occur before moving the data */
		if(*Count < Size)
		{
			for (cntr = 0;((cntr < tempCount) ); cntr++)
			{
				/* Read the data and move it to the pData buffer */
				pData[dataIndexCntr++] = UART_SpiUartReadRxData();   
			}
			/* Disable the interrupts before updating the receivedDataCount and 
			*  re-enable the interrupts after updating */
			CyGlobalIntDisable;
			
			/* subtract the read data count from received data count */
			receivedDataCount=receivedDataCount-tempCount;
			
			CyGlobalIntEnable;
			
			/* Check if the last data received is End of packet(0x17) 
			*  If not wait for additional 5ms */
			if(pData[dataIndexCntr-1]!= END_OF_PACKET)
			    CyDelay(5);
	 	}	
		
		/* If there is no space to move data, break from the loop */
		else
		{
			*Count=(*Count)-tempCount;
			UART_SpiUartClearRxBuffer();
			status = CYRET_EMPTY;
			break;
		}
	}
	return status;
}

/* [] END OF FILE */
