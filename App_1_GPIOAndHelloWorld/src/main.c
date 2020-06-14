// -------------------------------------------------------------------------
//
// Application 1. Basic test of PS (Processing System) and PL (Programmable
// Logic): using one ARM core, print helloworld and control a GPIO core implemented
// in PL to manipulate on-board LED state using Xilinx drivers for GPIO.
//
// Zybo Zynq-7010
//
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------

#include <stdio.h>
#include "platform.h"
#include <xgpio.h>
#include "xparameters.h"
#include "sleep.h"


// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
// STRUCTS AND ENUMS
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
// FUNCTION PROTOTYPES
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------------

int main()
{
	// Local data

	XGpio output;
	int switch_data = 0;

	// Initialization

	XGpio_Initialize(&output, XPAR_AXI_GPIO_0_DEVICE_ID);	// Initialize output XGpio variable
	XGpio_SetDataDirection(&output, 1, 0x0);				// Set first channel tristate buffer to output
    init_platform();

    // Functional algorithm

    print("Hello World\n\r");

    int dataRead;
    Xil_Out32(0x10000000U, 0xF0F0F0F0);
    dataRead = Xil_In32(0x10000000U);
    xil_printf("DDR: %d\n\r", dataRead);
    Xil_Out32(0xFFFC0000U, 0xF0F0F0F0);
    dataRead = Xil_In32(0xFFFC0000U);
    xil_printf("OCM: %d\n\r", dataRead);

    while(1){
       XGpio_DiscreteWrite(&output, 1, switch_data);	// Update LEDs state
       switch_data++;
       usleep(1000000);									// Delay 1s
    }

    // End

    cleanup_platform();
    return 0;
}

// -------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// -------------------------------------------------------------------------

