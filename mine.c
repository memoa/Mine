/*
    Program:    Mine
    Autor:  Cvijetinovic Dejan
    Datum:  13.11.2011.

    Ovaj program je uradjen prema igri minesweeper
    korisnik otvara neistrazena polja zadate matrice
    cilj je da otvori sva polja na kojima nisu postavljene mine
    kada korisnik otvori neistrazeno polje moze se desiti da
    otvori minu i time zavrsi dalje trazenje neuspesno
    ako nema mine na zadatom polju, polje ispisuje broj koji oznacava
    koliko ima mina oko tog polja
    korisnik takodje moze postavljati zastavice na neistrazena polja
    na kojima
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROWS 16
#define MAX_COLUMNS 76
#define MASK_OPENED 0x0f
#define MASK_MINE 0x10
#define MASK_FLAG 0x20
#define MASK_OPENING 0x40
#define SIGN_EMPTY_NOT_OPENED '?'
#define SIGN_FLAG '#'
#define SIGN_WRONG_FLAG 'x'
#define SIGN_MINE '*'
#define SIGN_OPENED_MINE '%'

char **matrix; /* fields matrix */
int n_rows, n_columns, n_mines, n_opened_fields, n_flags, mina;

void create_matrix() {
    int i;
    if (!(matrix = malloc(n_rows * sizeof(char*)))) {
        printf("*** Error! Memory Allocation Problem! ***\n"); exit(1);
    }
    for (i = 0; i < n_rows; i++)
        if (!(matrix[i] = malloc(n_columns * sizeof(char)))) {
            printf("*** Error! Memory Allocation Problem! ***\n"); exit(1);
        }
}

void destroy_matrix() {
    int i;
    for (i = 0; i < n_rows; i++)
        free(matrix[i]);
    free(matrix);
}

void initialization_matrix() {
    int i, j;
    for (i = 0; i < n_rows; i++)
        for (j = 0; j < n_columns; j++)
            matrix[i][j] = MASK_OPENED;
}

int found_mine(int row, int col) {
    if ((matrix[row][col]) & MASK_MINE) return 1;
    else return 0;
}

void set_mine(int row, int col) {
    matrix[row][col] |= MASK_MINE;
}

int found_flag(int row, int col) {
    if ((matrix[row][col]) & MASK_FLAG) return 1;
    else return 0;
}

void toggle_flag(int row, int col) {
    if (found_flag(row, col)) {
        matrix[row][col] &= ~MASK_FLAG; n_flags--;
    } else {
        matrix[row][col] |= MASK_FLAG; n_flags++;
    }
}

int found_opening(int row, int col) {
    if (matrix[row][col] & MASK_OPENING) return 1;
    else return 0;
}

void set_opening(int row, int col) {
    matrix[row][col] |= MASK_OPENING;
}

void remove_opening(int row, int col) {
    matrix[row][col] &= ~MASK_OPENING;
}

int field_opened(int row, int col) {
    if ((matrix[row][col] & MASK_OPENED) < 9) return 1;
    else return 0;
}

void mark_field(int row, int col, char broj) {
    broj &= MASK_OPENED;
    matrix[row][col] &= ~MASK_OPENED;
    matrix[row][col] |= broj;
}

int read_n_field(int row, int col) {
    return matrix[row][col] & MASK_OPENED;
}

void set_mines() {
    int i, j, r, rj;
    srand((unsigned)time(NULL));
    for (i = 0; i < n_mines; i++) {
        r = rand() % (n_rows * n_columns);
        for (j = 0; j < n_rows * n_columns; j++) {
            rj = (r + j) % (n_rows * n_columns);
            if (!found_mine(rj / n_columns, rj % n_columns)) {
                set_mine(rj / n_columns, rj % n_columns); break;
            }
        }
    }
}

int count_mines (int row, int col) {
    int count = 0;
    if (row + 1 < n_rows) {
        if (found_mine(row + 1, col)) count++;
        if (col + 1 < n_columns)
            if (found_mine(row + 1, col + 1)) count++;
    }
    if (col + 1 < n_columns) {
        if (found_mine(row, col + 1)) count++;
        if (row - 1 >= 0)
            if (found_mine(row - 1, col + 1)) count++;
    }
    if (row - 1 >= 0) {
        if (found_mine(row - 1, col)) count++;
        if (col - 1 >= 0)
            if (found_mine(row - 1, col - 1)) count++;
    }
    if (col - 1 >= 0) {
        if (found_mine(row, col - 1)) count++;
        if (row + 1 < n_rows)
            if (found_mine(row + 1, col - 1)) count++;
    }
    return count;
}

int count_flags (int row, int col) {
    int count = 0;
    if (row + 1 < n_rows) {
        if (found_flag(row + 1, col)) count++;
        if (col + 1 < n_columns)
            if (found_flag(row + 1, col + 1)) count++;
    }
    if (col + 1 < n_columns) {
        if (found_flag(row, col + 1)) count++;
        if (row - 1 >= 0)
            if (found_flag(row - 1, col + 1)) count++;
    }
    if (row - 1 >= 0) {
        if (found_flag(row - 1, col)) count++;
        if (col - 1 >= 0)
            if (found_flag(row - 1, col - 1)) count++;
    }
    if (col - 1 >= 0) {
        if (found_flag(row, col - 1)) count++;
        if (row + 1 < n_rows)
            if (found_flag(row + 1, col - 1)) count++;
    }
    return count;
}

int open_around (int, int);
/*
    Open field, check if there is flag set, check if there is mine, 
    count mines in surrounding fields. If there are no mines in surrounding field, 
    then call function 'open_around' to open surrounding fields.
    Parameters: coordinates, row and column number
    Returns: 0 if there is flag set or 
*/
int open_field(int row, int col) {
    int n_mines;
    if (!found_flag(row, col)) {
        if (!field_opened(row, col)) {
            if (found_mine(row, col)) {
                mark_field(row, col, 9); mina = 1; return 1;
            }
            n_mines = count_mines(row, col);
            mark_field(row, col, n_mines);
            n_opened_fields++;
            if (n_mines == 0)
                if (open_around(row, col)) return 1;
        } else if (field_opened(row, col) && found_opening(row, col)) { /* vec otvoreno polje se moze otvoriti samo ako je naredio korisnik, a ne i rekurzivni poziv ove funkcije */
            remove_opening(row, col);
            if (read_n_field(row, col) == count_flags(row, col))
                if (open_around(row, col)) return 1;
        }
    }
    return 0;
}

/*
    Open all fields surrounding field specified by coordinates

*/
int open_around (int row, int col) {
    if (row + 1 < n_rows) {
        if (open_field(row + 1, col)) return 1;
        if (col + 1 < n_columns)
            if (open_field(row + 1, col + 1)) return 1;
    }
    if (col + 1 < n_columns) {
        if (open_field(row, col + 1)) return 1;
        if (row - 1 >= 0)
            if (open_field(row - 1, col + 1)) return 1;
    }
    if (row - 1 >= 0) {
        if (open_field(row - 1, col)) return 1;
        if (col - 1 >= 0)
            if (open_field(row - 1, col - 1)) return 1;
    }
    if (col - 1 >= 0) {
        if (open_field(row, col - 1)) return 1;
        if (row + 1 < n_rows)
            if (open_field(row + 1, col - 1)) return 1;
    }
    return 0;
}

void display_matrix() {
    int i, j;
    printf("\n  y");
    for (i = 1; i <= n_columns; i++) /* horizontal ruler */
        if (!(i % 5)) printf("%5d", i);
    printf("\n x ");
    for (i = 1; i <= n_columns; i++)
        if (i % 5) putchar(' ');
        else putchar('|');
    for (i = 0; i < n_rows; i++) {
        printf("\n%2d ", i + 1); /* vertical ruler */
        for (j = 0; j < n_columns; j++) {
            if (found_flag(i, j)) {
                if (mina && !found_mine(i, j)) putchar(SIGN_WRONG_FLAG);
                else putchar(SIGN_FLAG);
            } else if (field_opened(i, j))
                putchar((char)read_n_field(i, j) + 0x30);
            else { /* unopened fields without flags */
                if (mina && found_mine(i, j))
                    if (read_n_field(i, j) == 9) putchar(SIGN_OPENED_MINE);
                    else putchar(SIGN_MINE);
                else putchar(SIGN_EMPTY_NOT_OPENED);
            }
        }
    }
    putchar('\n');
}

void input_coordinates(int *row, int *col) {
    do {
        printf("Enter coordinates (0 < row <= %d, 0 < col <= %d): ", n_rows, n_columns);
        scanf("%d%d", row, col);
    } while (*row < 1 || *row > n_rows || *col < 1 || *col > n_columns);
    (*row)--; (*col)--;
}

void console() {
    char c;
    int row, col;
    printf("\no-open field z-set/remove flag l-legend q-quit ");
    if ((c = getchar()) == '\n')
        c = getchar();
    if (c == 'o' || c == 'O') { /* opening */
        input_coordinates(&row, &col);
        if (found_flag(row, col))
            printf("That field cannot be opened because it is marked with a flag!\n");
        else {
            if (field_opened(row, col) && (!found_flag(row, col)))
                set_opening(row, col);
            open_field(row, col);
        }
    } else if (c == 'z' || c == 'Z') { /* set/remove flag */
        input_coordinates(&row, &col);
        if (field_opened(row, col))
            printf("That field is already opened. Flag cannot be set there!\n");
        else toggle_flag(row, col);
    } else if (c == 'q' || c == 'Q') {
        printf("Quit command entered. End of program.\n"); exit(0);
    } else if (c == 'l' || c == 'L') {
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

int main() {
    char c;
    do {
        mina = 0; n_opened_fields = 0; n_flags = 0;
        putchar('\n');
        do {
            printf("Enter matrix dimensions (1 < m <= %d, 1 < n <= %d) and number of mines (0 < b < m * n): ", MAX_ROWS, MAX_COLUMNS);
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
        printf("Do you want to continue (d/n)? ");
        if ((c = getchar()) == '\n')
            c = getchar();
    } while (c == 'd' || c == 'D');
    return 0;
}
