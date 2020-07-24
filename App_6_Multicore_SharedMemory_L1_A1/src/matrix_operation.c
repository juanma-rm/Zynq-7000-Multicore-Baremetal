// -------------------------------------------------------------------------
//
// matrix_operation.c
//
// This file allows performing some matrix processing and showing the results
// and the time taken.
//
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------

#include "matrix_operation.h"
#include "xil_printf.h"

// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------

#define BASE_SHARED_MEM_A (BASE_SHARED_MEM+0*MATRIX_ROWS*MATRIX_COLS*sizeof(int))
#define BASE_SHARED_MEM_B (BASE_SHARED_MEM+1*MATRIX_ROWS*MATRIX_COLS*sizeof(int))
#define BASE_SHARED_MEM_C (BASE_SHARED_MEM+2*MATRIX_ROWS*MATRIX_COLS*sizeof(int))
#define BASE_SHARED_MEM_D (BASE_SHARED_MEM+3*MATRIX_ROWS*MATRIX_COLS*sizeof(int))
#define BASE_SHARED_MEM_E (BASE_SHARED_MEM+4*MATRIX_ROWS*MATRIX_COLS*sizeof(int))
#define BASE_SHARED_MEM_F (BASE_SHARED_MEM+5*MATRIX_ROWS*MATRIX_COLS*sizeof(int))
#define BASE_SHARED_MEM_O (BASE_SHARED_MEM+6*MATRIX_ROWS*MATRIX_COLS*sizeof(int))

// -------------------------------------------------------------------------
// GLOBAL DATA
// -------------------------------------------------------------------------

// Pointers for matrix operations, pointing shared memory

int (*A)[MATRIX_ROWS][MATRIX_COLS] =
		(int (*)[MATRIX_ROWS][MATRIX_COLS]) (BASE_SHARED_MEM_A);
int (*B)[MATRIX_ROWS][MATRIX_COLS] =
		(int (*)[MATRIX_ROWS][MATRIX_COLS]) (BASE_SHARED_MEM_B);
int (*C)[MATRIX_ROWS][MATRIX_COLS] =
		(int (*)[MATRIX_ROWS][MATRIX_COLS]) (BASE_SHARED_MEM_C);
int (*D)[MATRIX_ROWS][MATRIX_COLS] =
		(int (*)[MATRIX_ROWS][MATRIX_COLS]) (BASE_SHARED_MEM_D);
int (*E)[MATRIX_ROWS][MATRIX_COLS] =
		(int (*)[MATRIX_ROWS][MATRIX_COLS]) (BASE_SHARED_MEM_E);
int (*F)[MATRIX_ROWS][MATRIX_COLS] =
		(int (*)[MATRIX_ROWS][MATRIX_COLS]) (BASE_SHARED_MEM_F);
int (*O)[MATRIX_ROWS][MATRIX_COLS] =
		(int (*)[MATRIX_ROWS][MATRIX_COLS]) (BASE_SHARED_MEM_O);

// -------------------------------------------------------------------------
// FUNCTION PROTOTYPES
// -------------------------------------------------------------------------

static int matrixCheckResult();

// -------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// -------------------------------------------------------------------------

/**
 * @brief matrixInit initalizes the values of the different matrixes.
 *
 */

void matrixInit()
{
	for (unsigned int i=0; i<MATRIX_ROWS; i++)
	{
		for (unsigned int j=0; j<MATRIX_COLS; j++)
		{
			(*A)[i][j]=INIT_VALUE_A;
			(*B)[i][j]=INIT_VALUE_B;
			(*C)[i][j]=INIT_VALUE_C;
			(*D)[i][j]=INIT_VALUE_D;
			(*E)[i][j]=0;
			(*F)[i][j]=0;
			(*O)[i][j]=0;
		}
	}
}

/**
 * @brief matrixMultE performs the matrix operation E=AxB.
 */

void matrixMultE()
{
	for (unsigned int i=0; i<MATRIX_ROWS; i++)
	{
		for (unsigned int j=0; j<MATRIX_COLS; j++)
		{
			(*E)[i][j]=0;
			for (unsigned int k=0; k<MATRIX_ROWS; k++)
			{
				(*E)[i][j] += (*A)[i][k]*(*B)[k][j];
			}
		}

	}
}

/**
 * @brief matrixMultF performs the matrix operation F=CxD.
 */

void matrixMultF()
{
	for (unsigned int i=0; i<MATRIX_ROWS; i++)
	{
		for (unsigned int j=0; j<MATRIX_COLS; j++)
		{
			(*F)[i][j]=0;
			for (unsigned int k=0; k<MATRIX_ROWS; k++)
			{
				(*F)[i][j] += (*C)[i][k]*(*D)[k][j];
			}
		}

	}
}

/**
 * @brief matrixSum performs the matrix operation O=ExF.
 */

void matrixSum()
{
	for (unsigned int i=0; i<MATRIX_ROWS; i++)
	{
		for (unsigned int j=0; j<MATRIX_COLS; j++)
		{
			(*O)[i][j] = (*E)[i][j] + (*F)[i][j];
		}
	}
}

/**
 * @brief matrixPrintResults shows by serial port the time indicated
 * if the results are the expected. Otherwise, it shows an error message.
 *
 * @param timeDif_Int is the time externally measured (user must take charge
 * of meausuring this time).
 */

void matrixPrintResults(unsigned int timeDif_Int)
{
	if (matrixCheckResult() == 0)
	{
		xil_printf("\n\rError in results.\n\r");
	}
	else
	{
		xil_printf("\n\rTime elapsed: %d.\n\r", timeDif_Int);
	}
}

/**
 * @brief matrixCheckResult check whether or not the results of O matrix is
 * the expected one.
 *
 * @return 1 if the checking process is successful or 0 in other case.
 */

static int matrixCheckResult()
{
	for (unsigned int i=0; i<MATRIX_ROWS; i++)
	{
		for (unsigned int j=0; j<MATRIX_COLS; j++)
		{
			if ( (*O)[i][j] !=
				(MATRIX_ROWS*(INIT_VALUE_A*INIT_VALUE_B +
						INIT_VALUE_C*INIT_VALUE_D) ))
			{
				return 0;
			}
		}
	}
	return 1;
}
