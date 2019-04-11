#ifndef _minecli_h_
#define _minecli_h_

// Drawing fields constants, used in CLI
#define SIGN_EMPTY_NOT_OPENED '?' // unopened field
#define SIGN_FLAG '#' // flag covering mine
#define SIGN_WRONG_FLAG 'x' // flag not covering mine
#define SIGN_MINE '*' // not stepped mine
#define SIGN_OPENED_MINE '%' // stepped mine

// *** Command Line Interface functions ***

// Display board with fields in CLI, draw rulers, read and draw fields from matrix
void display_matrix();

// Read coordinates entered by user
void input_coordinates(int *row, int *col);

// Show game menu and handle menu options
void console();

#endif