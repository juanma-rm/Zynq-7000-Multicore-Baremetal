// -------------------------------------------------------------------------
//
// Application 3. Use of ARM-core-private interrupts (dual core). Core A1
//
// Core 1 sets up its timer to interrupt each half second and attends that
// interrupt by switching the LEDs state.
//
// Zybo Zynq-7010
//
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------

#include "platform.h"
#include "xil_printf.h"
#include "zynq7000_baremetal.h"
#include "xscutimer.h"
#include "Xil_exception.h"
#include "Xscugic.h"
#include <xgpio.h>

// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------

#define nop() __asm__("nop")	// arm instruction: nop instruction

// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

// GPIO

XGpio output;
int switch_data = 0;

// Timer / interrupts

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
	// Initialization
	// ---------------------------------------------------------------------

	xil_printf("\n\rCPU1 waking up and configuring platform...");

	// Initialization - GPIO
	// Initialize output XGpio variable
	XGpio_Initialize(&output, XPAR_AXI_GPIO_0_DEVICE_ID);
	// Set first channel tristate buffer to output
	XGpio_SetDataDirection(&output, 1, 0x0);

	// Initialization - Timer
	if ( zynq7000BM_setup_privTimer(ZYNQ7000BM_TIMER_COUNT_1SEC/2, &TimerHandler,
			(u8)1, (u8)1) != XST_SUCCESS)
	{
		return -1;
	}

	// Initialization - interrupts
	// Configuring Gic
	if ( zynq7000BM_setup_GIC(&IntcHandler) != XST_SUCCESS )
	{
		return -1;
	}
	// Configuring timer interrupt
	if ( zynq7000BM_setup_interrupt(&IntcHandler, TIMER_INTERR_ID,
			(Xil_ExceptionHandler)TimerIntrHandler,
			(void *)(&TimerHandler)) != XST_SUCCESS)
	{
		return -1;
	}
	// Enabling mask interrupts
	zynq7000BM_enable_markIntr();

	// Initialization - Waking up CPU0
	zynq7000BM_init_core_end();

	// Timer start to count
	zynq7000BM_init_privTimer(&TimerHandler);

	// ---------------------------------------------------------------------
	// Main algorithm
	// ---------------------------------------------------------------------

    while (1)
    {
    	nop();
    }

	// ---------------------------------------------------------------------
	// End
	// ---------------------------------------------------------------------

    cleanup_platform();
    return 0;
}

// -------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// -------------------------------------------------------------------------

// ISR for timer interrupt
static void TimerIntrHandler(void *CallBackRef)
{
	// Clear timer event flag
	XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;
	XScuTimer_ClearInterruptStatus(TimerInstancePtr);

	// Switch LEDs states
	switch_data =  ~switch_data;
	XGpio_DiscreteWrite(&output, 1,switch_data);

}
