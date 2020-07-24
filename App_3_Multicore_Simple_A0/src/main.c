// -------------------------------------------------------------------------
//
// Application 3. Use of ARM-core-private interrupts (dual core). Core A0
//
// Core 0 configures platform, sets up its timer to interrupt each second and
// attends that interrupt by printing helloworld.
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

// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------

/* CPU1 code start address (manually defined in its lscript.ld). Must be
 placed at 0xFFFFFFF0 */
#define CPU1_START_ADDR			0x07000000U
#define nop() __asm__("nop")	// arm instruction: nop instruction

// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

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

	xil_printf("\n\rCPU0 configuring platform...");

	// Initialization - Timer
	if ( zynq7000BM_setup_privTimer(ZYNQ7000BM_TIMER_COUNT_1SEC,
			&TimerHandler, (u8)1, (u8)1) != XST_SUCCESS)
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

	// Initialization - Waking up CPU1
	zynq7000BM_init_core(CPU1_START_ADDR);

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

/* ISR for timer interrupt */

static void TimerIntrHandler(void *CallBackRef)
{
	// Clear timer event flag
	XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;
	XScuTimer_ClearInterruptStatus(TimerInstancePtr);

	// Print data
	xil_printf("\n\rTimer interrupt CPU0. 1 sec");

}
