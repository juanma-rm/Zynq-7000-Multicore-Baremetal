// -------------------------------------------------------------------------
//
// matrix_operation.h
//
// This file allows performing some matrix processing and showing the time
// taken in case the result is the expected one.
// Defines in .h file may be modified to change the behaviour parameters
//
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// HEADER GUARD - BEGIN
// -------------------------------------------------------------------------

#ifndef MATRIX_OPERATION_H
#define MATRIX_OPERATION_H

// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------

#define BASE_SHARED_MEM		0x0D000000U		// Base address of the shared
								// memory section where shared matrixes will
								// be allocated
#define MATRIX_ROWS		100		// Number of rows of matrixes
#define MATRIX_COLS		100		// Number of columns of matrixes
#define INIT_VALUE_A	1		// Init value for matrix A elements
#define INIT_VALUE_B	2		// Init value for matrix B elements
#define INIT_VALUE_C	1		// Init value for matrix C elements
#define INIT_VALUE_D	3		// Init value for matrix D elements

// -------------------------------------------------------------------------
// STRUCTS AND ENUMS
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
// EXTERNAL FUNCTION PROTOTYPES
// -------------------------------------------------------------------------

void matrixInit();
void matrixMultE();
void matrixMultF();
void matrixSum();
void matrixPrintResults(unsigned int timeDif_Int);

// -------------------------------------------------------------------------
// HEADER GUARD - END
// -------------------------------------------------------------------------

#endif /* MATRIX_OPERATION_H */
