// -------------------------------------------------------------------------
//
// Application 7. Use of shared memory between both cores using cache L1 and L2. Core A1
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
#include "matrix_operation.h"

// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

static XScuGic IntcHandler; 	// GIC handler

// -------------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------------

int main()
{

	// ---------------------------------------------------------------------
	// Initialization
	// ---------------------------------------------------------------------

	xil_printf("\n\rCPU1 waking up and configuring platform...");

	//Xil_L1DCacheDisable();	// Cache L1 disabled

	// Initialization - Interruptions

	// Configuring Gic
	if ( zynq7000BM_setup_GIC(&IntcHandler) != XST_SUCCESS )
	{
		return -1;
	}

	// Initialization - Waking up CPU0
	zynq7000BM_init_core_end();

	// ---------------------------------------------------------------------
	// Main algorithm
	// ---------------------------------------------------------------------

	// Flush cache to decrease variability in different executions
	Xil_L1DCacheFlush();

	// F=CxD
	matrixMultF();
	dsb();	// Wait until previous accesses are finished

	// Interrupt core 0
	int Status = XScuGic_SoftwareIntr(
			&IntcHandler, SGI_INTERR_ID_0, XSCUGIC_SPI_CPU0_MASK);

	// ---------------------------------------------------------------------
	// End
	// ---------------------------------------------------------------------

    cleanup_platform();
    return 0;
}

