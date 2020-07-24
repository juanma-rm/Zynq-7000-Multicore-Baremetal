// -------------------------------------------------------------------------
//
// Application 1. Basic test of PS (Processing System) and PL (Programmable
// Logic): using one ARM core, printing helloworld and controling a GPIO core
// implemented in PL to manipulate on-board LED state using Xilinx drivers
// for GPIO.
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
// MAIN
// -------------------------------------------------------------------------

int main()
{
	// ---------------------------------------------------------------------
	// Local data
	// ---------------------------------------------------------------------

	XGpio output;
	int switch_data = 0;

	// ---------------------------------------------------------------------
	// Initialization
	// ---------------------------------------------------------------------

	init_platform();
	// Initialize output XGpio variable
	XGpio_Initialize(&output, XPAR_AXI_GPIO_0_DEVICE_ID);
	// Set first channel tristate buffer to output
	XGpio_SetDataDirection(&output, 1, 0x0);

	// ---------------------------------------------------------------------
	// Main algorithm
	// ---------------------------------------------------------------------

    print("Hello World\n\r");

    while(1){
       XGpio_DiscreteWrite(&output, 1, switch_data);	// Update LEDs state
       switch_data++;
       usleep(1000000);									// Delay 1s
    }

	// ---------------------------------------------------------------------
	// End
	// ---------------------------------------------------------------------

    cleanup_platform();
    return 0;
}

