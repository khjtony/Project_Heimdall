/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This is source code for example project of the SPI
*  (Master, Slave) components.
*  Parameters used:
*   Mode                0(CPHA ==0, CPOL ==0)                
*   Data lines          MOSI+MISO
*   Shift direction     MSB First
*   DataBits:           8 
*   Bit Rate            1Mbit/s
*   Clock source        External 
*
*  SPI communication test using software buffer. 8 bytes are transmitted
*  between SPI Master and SPI Slave.
*  Received data are displayed on LCD. 
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <device.h>


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Main function performs following functions:
*   1. Declares cycle counter variable.
*   2. Enables interrupts.
*   3. Starts all used components.
*   3. Writes values to be sent into the SPI Slave's and Master's input buffers.
*   4. Waits for the Slave's SPI Done state generation.
*   5. Displays SPI Master and Slave received data.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    uint8 i = 0u;
    
    /* Software buffers use internal interrupt functionality to interact with
    * hardware buffers. Thus global interrupt enable command should be called 
    */
    CyGlobalIntEnable;
    
    /* We need to start Character LCD, SPI Master and Slave components */
    LCD_Start();
    SPIM_Start();
    SPIS_Start();
    
    /* SPI communication is provided in Mode 0. So, the first byte
    * should be written directly to the SPI Slave 
    */
    SPIS_WriteTxDataZero(0x11u);
    
    /* Other 7 bytes are written to the SPIS Tx software buffer */
    SPIS_WriteTxData(0x22u);
    SPIS_WriteTxData(0x33u);
    SPIS_WriteTxData(0x44u);
    SPIS_WriteTxData(0x55u);
    SPIS_WriteTxData(0x66u);
    SPIS_WriteTxData(0x77u);
    SPIS_WriteTxData(0x88u);

    /* writing data to the SPIM software buffer */
    SPIM_WriteTxData(0x99u); /* initiates data transmission */
    SPIM_WriteTxData(0xAAu);
    SPIM_WriteTxData(0xBBu);
    SPIM_WriteTxData(0xCCu);
    SPIM_WriteTxData(0xDDu);
    SPIM_WriteTxData(0xEEu);
    SPIM_WriteTxData(0xFFu);
    SPIM_WriteTxData(0x12u);

    /* We need to know the moment when SPI communication is complete
    * to display received data. SPIS_SPI_DONE status should be polled. 
    */
    while(!(SPIS_ReadTxStatus() & SPIS_STS_SPI_DONE));
    
    /* SPI communication is complete so we can display received data */
    
    LCD_Position(0u,0u);
    LCD_PrintString("SPIM Rx data:");
    
    LCD_Position(1u,0u);
    for(i=0u; i<8u; i++)
    {
        /* Read from SPIM Rx software buffer */
        LCD_PrintHexUint8(SPIM_ReadRxData());        
    }
    
    CyDelay(4000u);
    LCD_ClearDisplay();
    
    LCD_Position(0u,0u);
    LCD_PrintString("SPIS Rx data:");
    
    LCD_Position(1u,0u);
    for(i=0u; i<8u; i++)
    {
        /* Read from SPIS Rx software buffer */
        LCD_PrintHexUint8(SPIS_ReadRxData());        
    }
    
    while(1u){;}
      
}    


/* [] END OF FILE */
