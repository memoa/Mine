/*
    minecore library

    Desctiprion:
        The board of mines is organized in matrix of unsigned chars where each
        matrix element represent a field in a board. A few data about field is
        compressed in a single character:
        - reserved (2 bits), not used
        - flag set or not (1 bit)
        - mine set or not (1 bit)
        - number of mines surrounding a field (4 bits)

        Example, matrix element value: 
            x x 0 0 0 1 0 1
            |-| | | | <-> | <-- number of mines surrounding a field - 0 1 0 1 = 5
            |   | | <-- mine set (1) or not (0)
            |   | <-- flag set (1) or not (0)
            | <-- reserved (not used)
        
        Number of mines surrounding a field
        If field is open:
            0 0 0 0 (0) .. 1 0 0 0 (8): 0..8 mines found around a field
            1 0 0 1 (9): mine has been stepped on this field
        If field is not open:
            1 1 1 1 (f): undiscovered field (initially set)

        For extraction of data from a field character, bitwise filtering functions
        are used.
    
    Author: Dejan Cvijetinovic
    Date: 11.04.2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "minecore.h"

// *** Matrix ***

void create_matrix() {
    if (!(matrix = malloc(n_rows * sizeof(char*)))) {
        printf("*** Error! Memory Allocation Problem! ***\n"); exit(1);
    }
    for (int row = 0; row < n_rows; ++row)
        if (!(matrix[row] = malloc(n_columns * sizeof(char)))) {
            printf("*** Error! Memory Allocation Problem! ***\n"); exit(1);
        }
}

void destroy_matrix() {
    for (int row = 0; row < n_rows; ++row)
        free(matrix[row]);
    free(matrix);
}

void initialization_matrix() {
    for (int row = 0; row < n_rows; ++row)
        for (int col = 0; col < n_columns; ++col)
            matrix[row][col] = MASK_OPENED;
}

// *** Bitwise filtering functions ***

// Check if field contains mine
int found_mine(int row, int col) {
    if ((matrix[row][col]) & MASK_MINE) return 1;
    else return 0;
}

// Set mine in a field specified by coordinates
void set_mine(int row, int col) {
    matrix[row][col] |= MASK_MINE;
}

// Check if flag is set in a field
int found_flag(int row, int col) {
    if ((matrix[row][col]) & MASK_FLAG) return 1;
    else return 0;
}

// Set/remove flag in a field
void toggle_flag(int row, int col) {
    if (found_flag(row, col)) {
        matrix[row][col] &= ~MASK_FLAG; --n_flags;
    } else {
        matrix[row][col] |= MASK_FLAG; ++n_flags;
    }
}

// Check if field is opened (if have number of surroundig mines)
int field_opened(int row, int col) {
    if ((matrix[row][col] & MASK_OPENED) < 9) return 1;
    else return 0;
}

// Set number of surrounding mines in a field specified by coordinates
void mark_field(int row, int col, unsigned char num) {
    num &= MASK_OPENED;
    matrix[row][col] &= ~MASK_OPENED;
    matrix[row][col] |= num;
}

// Read number of mines surrounding field specified by coordinates
int read_n_field(int row, int col) {
    return matrix[row][col] & MASK_OPENED;
}

// *** Core of the game ***

/*
    Randomly setting mines on fields using user defined global variable n_mines
    n_mines stands for amonut of mines to be set.
    It generates randomized field number 'r' where:
    r / n_columns -> is a row coordinate and
    r % n_columns -> is a col coordinate of field where mine will be set
*/
void set_mines() {
    int r;
    srand((unsigned)time(NULL));
    for (int i = 0; i < n_mines; ++i) {
        r = rand() % (n_rows * n_columns);
        if (!found_mine(r / n_columns, r % n_columns))
            set_mine(r / n_columns, r % n_columns);
        else {
            --i; continue;
        }
    }
}

/*
    Count mines in fields surrounding specified field by coordinates
    Returns number of counted mines
*/
int count_mines (int row, int col) {
    int count = 0;
    if (row + 1 < n_rows) {
        if (found_mine(row + 1, col)) ++count; // down
        if (col + 1 < n_columns)
            if (found_mine(row + 1, col + 1)) ++count; // down left
    }
    if (col + 1 < n_columns) {
        if (found_mine(row, col + 1)) ++count; // left
        if (row - 1 >= 0)
            if (found_mine(row - 1, col + 1)) ++count; // up left
    }
    if (row - 1 >= 0) {
        if (found_mine(row - 1, col)) ++count; // up
        if (col - 1 >= 0)
            if (found_mine(row - 1, col - 1)) ++count; // up right
    }
    if (col - 1 >= 0) {
        if (found_mine(row, col - 1)) ++count; // right
        if (row + 1 < n_rows)
            if (found_mine(row + 1, col - 1)) ++count; // down right
    }
    return count;
}

/*
    Defining this function for use in 'open_field' function
    There are recursive calls of 'open_field' function
*/
void open_around (int, int);

/*
    Open field, check if there is flag set, check if there is mine, 
    count mines in surrounding fields. If there are no mines in surrounding field, 
    then call function 'open_around' to open surrounding fields.
    Parameters: coordinates, row and column number
    Returns: 0 if field is opened, otherwise returns 1
*/
int open_field(int row, int col) {
    int n_mines;
    if (!found_flag(row, col) && !field_opened(row, col)) {
        if (found_mine(row, col)) {
            mark_field(row, col, 9); mina = 1; return 1;
        }
        n_mines = count_mines(row, col);
        mark_field(row, col, n_mines);
        ++n_opened_fields;
        if (n_mines == 0)
            open_around(row, col); 
        return 1;
    }
    return 0;
}

// Open all fields surrounding field specified by coordinates
void open_around (int row, int col) {
    if (row + 1 < n_rows) {
        open_field(row + 1, col); // down
        if (col + 1 < n_columns)
            open_field(row + 1, col + 1); // down left
    }
    if (col + 1 < n_columns) {
        open_field(row, col + 1); // left
        if (row - 1 >= 0)
            open_field(row - 1, col + 1); // up left
    }
    if (row - 1 >= 0) {
        open_field(row - 1, col); // up
        if (col - 1 >= 0)
            open_field(row - 1, col - 1); // up right
    }
    if (col - 1 >= 0) {
        open_field(row, col - 1); // right
        if (row + 1 < n_rows)
            open_field(row + 1, col - 1); // down right
    }
}
