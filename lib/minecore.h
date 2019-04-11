#ifndef _minecore_h_
#define _minecore_h_

// Bitwise filtering constants
#define MASK_OPENED 0x0f // filter number of surrounding mines for a field
#define MASK_MINE 0x10 // filter mine
#define MASK_FLAG 0x20 // filter flag

// *** Global variables ***

unsigned char **matrix; // matrix of fields
int 
    // User defined
    n_rows, // number of rows in matrix
    n_columns, // number of columns in matrix
    n_mines, // number of mines in matrix
    // Internal
    n_opened_fields, // number of opened fields
    n_flags, // number of flags setted
    mina; // mine is stepped (1) or not (0)

// *** Matrix ***

void create_matrix();

void destroy_matrix();

void initialization_matrix();

// *** Bitwise filtering functions ***

// Check if field contains mine
int found_mine(int row, int col);

// Set mine in a field specified by coordinates
void set_mine(int row, int col);

// Check if flag is set in a field
int found_flag(int row, int col);

// Set/remove flag in a field
void toggle_flag(int row, int col);

// Check if field is opened (if have number of surroundig mines)
int field_opened(int row, int col);

// Set number of surrounding mines in a field specified by coordinates
void mark_field(int row, int col, unsigned char num);

// Read number of mines surrounding field specified by coordinates
int read_n_field(int row, int col);

// *** Core of the game ***

/*
    Randomly setting mines on fields using user defined global variable n_mines
    n_mines stands for amonut of mines to be set.
    It generates randomized field number 'r' where:
    r / n_columns -> is a row coordinate and
    r % n_columns -> is a col coordinate of field where mine will be set
*/
void set_mines();

/*
    Count mines in fields surrounding specified field by coordinates
    Returns number of counted mines
*/
int count_mines (int row, int col);

/*
    Open field, check if there is flag set, check if there is mine, 
    count mines in surrounding fields. If there are no mines in surrounding field, 
    then call function 'open_around' to open surrounding fields.
    Parameters: coordinates, row and column number
    Returns: 0 if field is opened, otherwise returns 1
*/
int open_field(int row, int col);

// Open all fields surrounding field specified by coordinates
void open_around (int row, int col);

#endif