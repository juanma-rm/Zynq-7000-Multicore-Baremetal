// -------------------------------------------------------------------------
//
// Zynq7000_baremetal.c
//
// This lib is designed to combine several functions which allow configuring
// the Zynq-7000 platform to use multicore and coherence in standalone, as
// well as different cpu-private resources. It is based on official Xilinx
// bsp for Zynq-7000
//
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------

#include "zynq7000_baremetal.h"

#include "xil_io.h"
#include "xpseudo_asm.h"
#include "xil_printf.h"
#include "xstatus.h"

// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------

// CPU1 looks for its code start address at 0xFFFFFFF0 address
#define CPU1_START_ADDR_ADDR	0xFFFFFFF0U

// Timer / Interrupt
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID

// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

static XScuTimer_Config *TMRConfigPtr;		// Timer config

// -------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// -------------------------------------------------------------------------

/**
 * @brief zynq7000BM_init_core initialises CPU 1 from CPU 0. CPU remains slept
 * while CPU 1 is booting. "multicoreBM_init_core_end()" function must be
 * called from CPU 1 after booting to wake up CPU 0.
 *
 * @param cpu_start_addr refers the address where the code of the cpu to wake up
 * is placed. This must have been defined in lscript.ld file corresponding to the
 * cpu being waken up.
 */
void zynq7000BM_init_core(u32 cpu_start_addr)
{
	// Tell CPU1 where its code starts
	Xil_Out32(CPU1_START_ADDR_ADDR, cpu_start_addr);
	// To assure the previous write goes to ram instead of being stored in cache:
	Xil_DCacheStoreLine(CPU1_START_ADDR_ADDR);
	dsb(); 					// Wait until previous data instructions are finished
	xil_printf("\n\rCPU0 waking up CPU1 and suspending itself until CPU1 boot.");
	sev();					// Wake up to CPU1 (send signal)
	// Wait after CPU1 boot:
	wfe();					// Wait for event; clear the previous CPU0 "sev()"
	wfe();					// Wait for event; CPU0 suspends and waits for sev
	xil_printf("\n\rCPU0 continues.\n\r");
}

/**
 * @brief zynq7000BM_init_core_end wakes up CPU 0 from CPU 1. Must be called
 * after booting and configuring CPU 1
 */
void zynq7000BM_init_core_end()
{
	xil_printf("\n\rCPU1 waking CPU0 up.");
	sev();					// Wake up to CPU0
	wfe();					// Clears the previous CPU1 sev()
}

/**
 * @brief zynq7000BM_setup_privTimer configures cpu private timer.
 *
 * @param timer_cycles defines the number of cycles that will be loaded to
 * the timer. The timer counts from timer_cycles down to zero. Timer clk is
 * fed from A9 clk by default (see define ZYNQ7000BM_FREQ_TIMER_HZ).
 *
 * @param TimerHandler is a XScuTimer pointer to handle the timer. The
 * XScuTimer data must be created externally to this library.
 *
 * @param autoreload: if set to 1, the counter will start the countdown
 * each time it reaches zero.
 *
 * @param intrEnabled: if set to 1, an interrupt will be generated each time
 * the counter reaches zero.
 *
 * @return XST_SUCCESS if the configuration is successful or XST_FAILURE in
 * other case.
 */
int zynq7000BM_setup_privTimer(u32 timer_cycles, XScuTimer *TimerHandler,
		u8 autoreload, u8 intrEnabled)
{
	// Get CPU-private timer parameters
	TMRConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	// Config timer handler
	XScuTimer_CfgInitialize(TimerHandler, TMRConfigPtr, TMRConfigPtr->BaseAddr);
	s32 statusTimer = XScuTimer_SelfTest(TimerHandler);
	if (statusTimer == (s32)XST_FAILURE)
	{
		xil_printf("\n\rTimer failure.\n\r");
		return -1;
	}
	// Timer will count from TIMER_COUNT_1SEC to zero
	XScuTimer_LoadTimer(TimerHandler, timer_cycles);
	XScuTimer_SetPrescaler(TimerHandler, 0);
	if (autoreload == 1)
		XScuTimer_EnableAutoReload(TimerHandler);
	if (intrEnabled == 1)
		XScuTimer_EnableInterrupt(TimerHandler);
	return XST_SUCCESS;
}

/**
 * @brief zynq7000BM_init_privTimer starts the cpu private timer.
 *
 * @param TimerHandler is a XScuTimer pointer to handle the timer. The
 * XScuTimer data must be created externally to this library.
 */
void zynq7000BM_init_privTimer(XScuTimer *TimerHandler)
{
	XScuTimer_Start(TimerHandler);
}

/**
 * @brief zynq7000BM_setup_GIC configures the General Interrupt Controller,
 * common for both A9 cores. If USE_AMP=1 is defined for core 1, this one
 * will not restart the gic (core 0 should have done it before).
 *
 * @param IntcHandler is a XScuGic pointer to handle the gic. The XScuGic
 * data must be created externally to this library.
 */
int zynq7000BM_setup_GIC(XScuGic *IntcHandler)
{

	XScuGic_Config *IntcConfig; // GIC config
	Xil_ExceptionInit();

	// Get GIC parameters
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	// Configure GIC handler
	s32 status = XScuGic_CfgInitialize(IntcHandler, IntcConfig,
			IntcConfig->CpuBaseAddress);
	if (status == (s32)XST_FAILURE)
	{
		xil_printf("\n\rError when setting up GIC.\n\r");
		return XST_FAILURE;
	}
    // Connect to the hardware
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler, IntcHandler);

	return XST_SUCCESS;
}

/**
 * @brief zynq7000BM_setup_interrupt configures the gic to attend a
 * specific interrupt.
 *
 * @param IntcHandler is a XScuGic pointer to handle the gic. The XScuGic
 * data must be  * created externally to this library.
 *
 * @param Int_Id define the identification of the interrupt.
 *
 * @param Xil_ExceptionHandler carries the ISR function that will attend
 * the interrupt.  * This must be defined externally (as static void) and be
 * passed through this parameter casted to Xil_ExceptionHandler
 *
 * @param CallBackRef is a pointer to the device that is in charge of
 * generating the interrupt. The device must be casted to (void *).
 *
 * @return XST_SUCCESS if the configuration is successful or XST_FAILURE otherwise.
 */
int zynq7000BM_setup_interrupt(XScuGic *IntcHandler, u16 Int_Id,
		Xil_ExceptionHandler Handler, void *CallBackRef)
{
	// Set up the timer interrupt
	s32 statusInt = XScuGic_Connect(IntcHandler, Int_Id, Handler, CallBackRef);
	if (statusInt == (s32)XST_FAILURE)
	{
		xil_printf("\n\rError when setting up device interrupt.\n\r");
		return XST_FAILURE;
	}
	// Enable the interrupt for the device at GIC
	XScuGic_Enable(IntcHandler, Int_Id);

	return XST_SUCCESS;
}

/**
 * @brief zynq7000BM_enable_markIntr enables the exceptions.
 */
void zynq7000BM_enable_markIntr()
{
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
}

