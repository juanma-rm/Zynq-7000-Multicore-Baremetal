// -------------------------------------------------------------------------
//
// Application 3. Use of ARM-core-private interruptions (dual core). Core A0
//
// Core 0 enables platform, sets up its timer to interrupt each second and
// attends that interruption by printing helloworld.
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

// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------

// Timer / Interruptions

#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_INTERR_ID		XPAR_SCUTIMER_INTR
#define TIMER_COUNT_MAX		0xFFFFFFFFU
#define FREQ_TIMER_HZ		( XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2 )
	// FREQ_TIMER_HZ -> For Zynq-7000, private timer clk = Cortex A9 CPU clk / 2
#define TIMER_COUNT_1SEC	FREQ_TIMER_HZ		// For prescaler = 0

// -------------------------------------------------------------------------
// STRUCTS AND ENUMS
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

// Timer / Interruptions

static XScuGic IntcHandler; 	// GIC handler
static XScuTimer TimerHandler;	// Timer handler

// -------------------------------------------------------------------------
// FUNCTION PROTOTYPES
// -------------------------------------------------------------------------

static void SetupInterruptSystem(XScuGic *GicInstancePtr, XScuTimer *TimerInstancePtr, u16 TimerIntrId);
static void TimerIntrHandler(void *CallBackRef);

// -------------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------------

int main()
{

	// Local data

	XScuTimer_Config *TMRConfigPtr;		// Timer config

	// Initialization - Platform

	init_platform();

	// Initialization - Timer

    TMRConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);	// Get CPU-private timer parameters
    XScuTimer_CfgInitialize(&TimerHandler, TMRConfigPtr, TMRConfigPtr->BaseAddr);	// Config timer handler
    XScuTimer_SelfTest(&TimerHandler);
	XScuTimer_LoadTimer(&TimerHandler, TIMER_COUNT_1SEC);	// Timer will count from TIMER_COUNT_1SEC to zero
	XScuTimer_SetPrescaler(&TimerHandler, 0);
	XScuTimer_EnableAutoReload(&TimerHandler);
	XScuTimer_Start(&TimerHandler);							// Timer start to count

	// Initialization - Interruptions

	SetupInterruptSystem(&IntcHandler, &TimerHandler, TIMER_INTERR_ID);

    // Main algorithm

    while (1)
    {

    }

    // End

    cleanup_platform();
    return 0;
}

// -------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// -------------------------------------------------------------------------

// Set up GIC and timer interruption and enable interruptions
static void SetupInterruptSystem(XScuGic *GicInstancePtr, XScuTimer *TimerInstancePtr, u16 TimerIntrId)
{
	XScuGic_Config *IntcConfig; // GIC config
	Xil_ExceptionInit();

	// Get GIC parameters
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	// Configure GIC handler
	XScuGic_CfgInitialize(GicInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
    // Connect to the hardware
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler, GicInstancePtr);

	// Set up the timer interruption
	XScuGic_Connect(GicInstancePtr, TimerIntrId,
			(Xil_ExceptionHandler)TimerIntrHandler, (void *)TimerInstancePtr);
	// Enable the interruption for timer at GIC
	XScuGic_Enable(GicInstancePtr, TimerIntrId);
	// Enable interruption on the timer
	XScuTimer_EnableInterrupt(TimerInstancePtr);

	// Enable interruptions in the processor.
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
}

// ISR for timer interruption
static void TimerIntrHandler(void *CallBackRef)
{
	// Clear timer event flag
	XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;
	XScuTimer_ClearInterruptStatus(TimerInstancePtr);

	// Print data
	xil_printf("Timer interrupt CPU0. 1 sec\n\r");

}


