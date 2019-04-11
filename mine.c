/*
    Mine game

    Description:
        This program is my remake of game Minesweeper as a console application
        written in C. User choose dimensions of board of fields and a number of
        mines randomly set in fields. Then, user can open a field specified by 
        coordinates or set flag to mark risky field. When user open field where
        mine is set, the rest of mines wiil be displayed on board and game is over.
        If user open an empty field, a count of mines surrounded by that field
        will be displayed, and if there are no mines surrounding a field, then
        all fields around will be opened. When all fields with no mines are open,
        then user solved mine board and game is over.

    Author: Cvijetinovic Dejan
    Date: 11.04.2019. (modified)
*/

#include <stdio.h>

#include "lib\minecore.h" // the core functions and global variables
#include "lib\minecli.h" // functions for CLI

// *** Constants ***

// Maximum matrix dimensions
#define MAX_ROWS 16
#define MAX_COLUMNS 76

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
