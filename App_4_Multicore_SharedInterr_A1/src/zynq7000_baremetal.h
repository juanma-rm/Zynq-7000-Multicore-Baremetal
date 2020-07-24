// -------------------------------------------------------------------------
//
// Zynq7000_baremetal.h
//
// This lib is designed to combine several functions which allow configuring
// the Zynq-7000 platform to use multicore and coherence in standalone, as
// well as different cpu-private resources. It is based on official Xilinx
// bsp for Zynq-7000
//
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// HEADER GUARD - BEGIN
// -------------------------------------------------------------------------

#ifndef ZYNQ7000_BAREMETAL_H_
#define ZYNQ7000_BAREMETAL_H_

// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------

#include "xil_types.h"
#include "xscutimer.h"
#include "xil_exception.h"
#include "Xscugic.h"
#include "xil_cache_l.h"

// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------

// Timer

#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_INTERR_ID		XPAR_SCUTIMER_INTR
#if XPAR_CPU_ID==0
	#define ZYNQ7000BM_FREQ_CPU_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ
#elif XPAR_CPU_ID==1
	#define ZYNQ7000BM_FREQ_CPU_HZ	XPAR_PS7_CORTEXA9_1_CPU_CLK_FREQ_HZ
#endif
// For Zynq-7000, private timer clk = Cortex A9 CPU clk / 2
#define ZYNQ7000BM_FREQ_TIMER_HZ		( ZYNQ7000BM_FREQ_CPU_HZ / 2 )
#define ZYNQ7000BM_TIMER_COUNT_MAX		0xFFFFFFFFU
#define ZYNQ7000BM_TIMER_COUNT_1SEC		ZYNQ7000BM_FREQ_TIMER_HZ

// SGI Interrupts

#define SGI_ICCIAR			0xF8F0010CU	// Interrupt Acknowledge Register.
										// Read it to clear intr flag
#define SGI_INTERR_ID_0		0
#define SGI_INTERR_ID_1		1
#define SGI_INTERR_ID_2		2
#define SGI_INTERR_ID_3		3
#define SGI_INTERR_ID_4		4
#define SGI_INTERR_ID_5		5
#define SGI_INTERR_ID_6		6
#define SGI_INTERR_ID_7		7
#define SGI_INTERR_ID_8		8
#define SGI_INTERR_ID_9		9
#define SGI_INTERR_ID_10	10
#define SGI_INTERR_ID_11	11
#define SGI_INTERR_ID_12	12
#define SGI_INTERR_ID_13	13
#define SGI_INTERR_ID_14	14
#define SGI_INTERR_ID_15	15

// ARM assembly instructions
#define sev() __asm__("sev")	// arm instruction: cause event to all cores
#define wfe() __asm__("wfe")	// arm instruction: suspend until receiving
								// event (sev)
// -------------------------------------------------------------------------
// EXTERNAL FUNCTION PROTOTYPES
// -------------------------------------------------------------------------

void zynq7000BM_init_core(u32 cpu_start_addr);
void zynq7000BM_init_core_end();
int zynq7000BM_setup_privTimer(u32 timer_cycles, XScuTimer *TimerHandler,
		u8 autoreload, u8 intrEnabled);
void zynq7000BM_init_privTimer(XScuTimer *TimerHandler);
int zynq7000BM_setup_GIC(XScuGic *IntcHandler);
int zynq7000BM_setup_interrupt(XScuGic *IntcHandler, u16 Int_Id,
		Xil_ExceptionHandler Handler, void *CallBackRef);
void zynq7000BM_enable_markIntr();

// -------------------------------------------------------------------------
// HEADER GUARD - END
// -------------------------------------------------------------------------

#endif /* ZYNQ7000_MULTICORE_BM_H_ */
