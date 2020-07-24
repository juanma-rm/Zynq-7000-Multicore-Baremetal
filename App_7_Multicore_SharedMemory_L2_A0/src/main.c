// -------------------------------------------------------------------------
//
// Application 7. Use of shared memory between both cores using cache L1 and L2. Core A0
//
// Square matrix multiplication. Parameters customizable in "matrix_operation.h"
// 	- Core 0 performs: E=AxB and O=E+F
//	- Core 1 performs: F=CxD
//  - 	A[i]=1 for each i within A
//  - 	B[i]=2 for each i within B
//  - 	C[i]=1 for each i within C
//  - 	D[i]=3 for each i within D
//  - 	E[i] = AxB = 20000 for each i within E
//  - 	F[i] = AxB = 30000 for each i within F
//	-	O[i] = E+F = 50000 for each i within O
//  - O = AxB + CxD = ExF
//  - All the matrixes are stored in shared mem
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
#include "xtime_l.h"
#include "matrix_operation.h"

// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------


/* CPU1 code start address (manually defined in its lscript.ld). Must be
	placed at 0xFFFFFFF0 */
#define CPU1_START_ADDR			0x07000000U
#define nop() __asm__("nop")	// arm instruction: nop instruction
#define MULTICORE				// Uncomment for multicore

// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

static XScuGic IntcHandler; 	// GIC handler
volatile u32 wait = 0;

// -------------------------------------------------------------------------
// FUNCTION PROTOTYPES
// -------------------------------------------------------------------------

static void SGIHandler(void *CallBackRef);

// -------------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------------

int main()
{
	// ---------------------------------------------------------------------
	// Local data
	// ---------------------------------------------------------------------

	XTime timeIni=0, timeFin=0, timeDif=0;
	unsigned int timeDif_Int=0;

	// ---------------------------------------------------------------------
	// Initialization
	// ---------------------------------------------------------------------

	xil_printf("\n\rCPU0 configuring platform...");

	//Xil_L1DCacheDisable();	// Cache L1 disabled
	//Xil_L2CacheDisable();	// Cache L2 disabled

	// Initialization - Interruptions

	// Configuring Gic
	if ( zynq7000BM_setup_GIC(&IntcHandler) != XST_SUCCESS )
	{
		return -1;
	}
	// Configuring SGI interruption
	if ( zynq7000BM_setup_interrupt(&IntcHandler, SGI_INTERR_ID_0,
				(Xil_ExceptionHandler)SGIHandler,
				(void *)(&IntcHandler)) != XST_SUCCESS)
	{
		return -1;
	}
	// Enabling mask interruptions
	zynq7000BM_enable_markIntr();

	// Initialization - input data for application

	matrixInit();

	// Initialization - Waking up CPU1 (multicore)
#ifdef	MULTICORE
	zynq7000BM_init_core(CPU1_START_ADDR);
#endif


	// ---------------------------------------------------------------------
	// Main algorithm
	// ---------------------------------------------------------------------

	// Flush cache to decrease variability in different executions
	Xil_L1DCacheFlush();
	Xil_L2CacheFlush();

	XTime_GetTime(&timeIni);

	// E = AxB
	matrixMultE();

	// Wait F = CxD from cpu1 (Multi core)
#ifdef	MULTICORE
    while (wait != 1)
    {
    	nop();
    }
#else
	// F = CxD (Single core)
    matrixMultF();
#endif

	// Calculate O=E+F
	matrixSum();

	XTime_GetTime(&timeFin);
	timeDif = timeFin - timeIni;
	timeDif_Int = timeDif;

	// Print results
	matrixPrintResults(timeDif_Int);

	// ---------------------------------------------------------------------
	// End
	// ---------------------------------------------------------------------

    cleanup_platform();
    return 0;
}

// -------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// -------------------------------------------------------------------------

/* ISR for SGI interruption */
static void SGIHandler(void *CallBackRef)
{
	u32 intFlag = Xil_In32(SGI_ICCIAR);	// Clear interrupt flag
	wait = (u32)1;
}

