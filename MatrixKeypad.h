#ifndef MATRIX_KEYPAD_H
#define	MATRIX_KEYPAD_H

/* 
 * File:        MatrixKeypad.h
 * Author:      Mike Boldin <mboldin@niagaracollege.ca>
 * Comments:    Matrix Keypad routines for the Niagara College
 *               PIC18F TRAINER board (4x4 matrix)
 * Revision history: 
 * 2020.02.09   Created from 2019 code
 */


// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/**************************************************************************
 *  Function:	  MatrixKeypadInit     
 *  Description:  I/O initialization for the matrix keypad
 *  Parameters:   none	
 *  Returns:      none
 *  Notes:        Uses RB2..RB5 and RJ0..RJ3
 *                
 **************************************************************************/
    
    void    MatrixKeypadInit    (void) ;

/**************************************************************************
 *  Function:	  getch     
 *  Description:  Read a character from standard input -- remapped to the
 *                 matrix keypad
 *  Parameters:   none	
 *  Returns:      ASCII character -- one of:  '0'-'9', 'A', 'B', 'C', 'D',
 *      		   'E', or 'F'
 *  Notes:        Incomplete implementation.  No way to indicate EOF.
 *                See the C Standard Library documentation.           
 *                
 **************************************************************************/

    int		getch           (void) ;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* MATRIX_KEYPAD_H */
