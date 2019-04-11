/*
    Mine game

    Description:
        This program is my remake of game Minesweeper, is a console application
        written in C. User choose dimensions of board of fields and a number of
        mines randomly set in fields. Then, user can open a field specified by 
        coordinates or set flag to mark risky field. When user open field where
        mine is set, the rest of mines wiil be displayed on board and game is over.
        If user open an empty field, a count of mines surrounded by that field
        will be displayed, and if there are no mines surrounding a field, then
        all fields around will be opened. When all fields with no mines are open,
        then user solved mine board and game is over.

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

    Author: Cvijetinovic Dejan
    Date: 11.04.2019. (modified)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// *** Constants ***

// Maximum matrix dimensions
#define MAX_ROWS 16
#define MAX_COLUMNS 76

// Bitwise filtering constants
#define MASK_OPENED 0x0f // filter number of surrounding mines for a field
#define MASK_MINE 0x10 // filter mine
#define MASK_FLAG 0x20 // filter flag

// Drawing fields constants, used in CLI
#define SIGN_EMPTY_NOT_OPENED '?' // unopened field
#define SIGN_FLAG '#' // flag covering mine
#define SIGN_WRONG_FLAG 'x' // flag not covering mine
#define SIGN_MINE '*' // not stepped mine
#define SIGN_OPENED_MINE '%' // stepped mine

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

// *** Command Line Interface functions ***

// Display board with fields in CLI, draw rulers, read and draw fields from matrix
void display_matrix() {
    printf("\n   ");
    for (int i = 1; i <= n_columns; ++i) // horizontal ruler
        if (!(i % 5)) printf("%5d", i);
    printf("\n   ");
    for (int i = 1; i <= n_columns; ++i)
        if (i % 5) putchar(' ');
        else putchar('|');
    for (int i = 0; i < n_rows; ++i) {
        printf("\n%2d ", i + 1); // vertical ruler
        for (int j = 0; j < n_columns; ++j) { // draw fields
            if (found_flag(i, j)) { // draw flag
                if (mina && !found_mine(i, j)) putchar(SIGN_WRONG_FLAG);
                else putchar(SIGN_FLAG);
            } else if (field_opened(i, j))
                // Draw number of surrounding mines for fields
                putchar((char)read_n_field(i, j) + 0x30);
            else { // draw unopened fields without flags
                if (mina && found_mine(i, j)) // draw mines
                    if (read_n_field(i, j) == 9) putchar(SIGN_OPENED_MINE);
                    else putchar(SIGN_MINE);
                else putchar(SIGN_EMPTY_NOT_OPENED); // draw unopened fields
            }
        }
    }
    putchar('\n');
}

// Read coordinates entered by user
void input_coordinates(int *row, int *col) {
    do {
        printf("Enter coordinates (0 < row <= %d, 0 < col <= %d): ", n_rows, n_columns);
        scanf("%d%d", row, col);
    } while (*row < 1 || *row > n_rows || *col < 1 || *col > n_columns);
    --(*row); --(*col);
}

// Show game menu and handle menu options
void console() {
    char c;
    int row, col;
    printf("\no-open field f-set/remove flag l-legend q-quit ");
    if ((c = getchar()) == '\n')
        c = getchar();
    if (c == 'o' || c == 'O') { // open field
        input_coordinates(&row, &col);
        if (found_flag(row, col))
            printf("That field cannot be opened because it is marked with a flag!\n");
        else
            open_field(row, col);
    } else if (c == 'f' || c == 'F') { // set/remove flag
        input_coordinates(&row, &col);
        if (field_opened(row, col))
            printf("That field is already opened. Flag cannot be set there!\n");
        else toggle_flag(row, col);
    } else if (c == 'q' || c == 'Q') { // quit game
        printf("Quit command entered. End of program.\n"); exit(0);
    } else if (c == 'l' || c == 'L') { // show legend
        printf("\nLegend\n\n");
        printf("%c\t-unopened empty field\n", SIGN_EMPTY_NOT_OPENED);
        printf("0-8\t-opened empty field\n");
        printf("%c\t-flag\n", SIGN_FLAG);
        printf("%c\t-flag on wrong field\n", SIGN_WRONG_FLAG);
        printf("%c\t-mine\n", SIGN_MINE);
        printf("%c\t-opened mine\n\n", SIGN_OPENED_MINE);
        printf("Press ENTER to continue");
        getchar(); getchar();
    } else printf("Wrong command entered!\n");
}

// *** Program execution ***

int main() {
    char c;
    do {
        mina = 0; n_opened_fields = 0; n_flags = 0;
        putchar('\n');
        do {
            printf("Enter board dimensions (1 < row <= %d, 1 < col <= %d) and number of mines (0 < n < row * col): ", MAX_ROWS, MAX_COLUMNS);
            scanf("%d%d%d", &n_rows, &n_columns, &n_mines);
        } while (n_rows <= 1 || n_rows > MAX_ROWS || n_columns <= 1 || n_columns > MAX_COLUMNS || n_mines < 1 || n_mines >= n_rows * n_columns);
        create_matrix();
        initialization_matrix();
        set_mines();
        do {
            display_matrix();
            printf("\nStill %d unmarked mines remain.\n", n_mines - n_flags);
            console();
        } while (!mina && n_opened_fields + n_mines != n_rows * n_columns);
        display_matrix();
        destroy_matrix();
        if (mina) printf("\nStepped on mine. Game over.\n");
        else printf("\nCongratulations. Mine field solved.\n");
        printf("Do you want to continue (y/n)? ");
        if ((c = getchar()) == '\n')
            c = getchar();
    } while (c == 'y' || c == 'Y');
    return 0;
}
