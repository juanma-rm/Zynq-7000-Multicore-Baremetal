// -------------------------------------------------------------------------
//
// Application 2. Use of ARM-core-private interruptions (single core).
// A ARM-core-private timer is set up to interrupt each one second.
// The core itself attends the interruption (TimerIntrHandler ISR) printing
// helloword and switching the LEDs status.
//
// Zybo Zynq-7010
//
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "xscutimer.h"
#include "Xil_exception.h"
#include "Xscugic.h"
#include <xgpio.h>

#include "zynq7000_baremetal.h"

// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

// GPIO

XGpio output;
int switch_data = 0;

// Timer / Interruptions

static XScuGic IntcHandler; 	// GIC handler
static XScuTimer TimerHandler;	// Timer handler

// -------------------------------------------------------------------------
// FUNCTION PROTOTYPES
// -------------------------------------------------------------------------

static void TimerIntrHandler(void *CallBackRef);

// -------------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------------

int main()
{

	// ---------------------------------------------------------------------
	// Local data
	// ---------------------------------------------------------------------

	XScuTimer_Config *TMRConfigPtr;		// Timer config

	// ---------------------------------------------------------------------
	// Initialization
	// ---------------------------------------------------------------------

	// Initialization - GPIO

	// Initialize output XGpio variable
	XGpio_Initialize(&output, XPAR_AXI_GPIO_0_DEVICE_ID);
	// Set first channel tristate buffer to output
	XGpio_SetDataDirection(&output, 1, 0x0);

	// Initialization - Timer
	if ( zynq7000BM_setup_privTimer(ZYNQ7000BM_TIMER_COUNT_1SEC,
			&TimerHandler, (u8)1, (u8)1) != XST_SUCCESS)
	{
		return -1;
	}

	// Initialization - Interruptions
	// Configuring Gic
	if ( zynq7000BM_setup_GIC(&IntcHandler) != XST_SUCCESS )
	{
		return -1;
	}
	// Configuring timer interruption
	if ( zynq7000BM_setup_interrupt(&IntcHandler, TIMER_INTERR_ID,
			(Xil_ExceptionHandler)TimerIntrHandler,
			(void *)(&TimerHandler)) != XST_SUCCESS)
	{
		return -1;
	}
	// Enabling mask interruptions
	zynq7000BM_enable_markIntr();

	// ---------------------------------------------------------------------
	// Main algorithm
	// ---------------------------------------------------------------------

	// Timer start to count
	zynq7000BM_init_privTimer(&TimerHandler);

    while (1);

	// ---------------------------------------------------------------------
	// End
	// ---------------------------------------------------------------------

    cleanup_platform();
    return 0;
}

// -------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// -------------------------------------------------------------------------

// ISR for timer interruption
static void TimerIntrHandler(void *CallBackRef)
{
	// Clear timer event flag
	XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;
	XScuTimer_ClearInterruptStatus(TimerInstancePtr);

	// Print data and switch LEDs states
	printf("Timer interrupt. 1 sec\n\r");
	switch_data =  ~switch_data;
	XGpio_DiscreteWrite(&output, 1,switch_data);
}


