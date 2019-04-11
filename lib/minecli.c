/*
    minecli library

    Description:
        Command Line Interface functions

    Author: Dejan Cvijetinovic
    Date: 11.04.2019
*/

#include <stdio.h>
#include <stdlib.h>

#include "minecore.h" // the core functions and global variables
#include "minecli.h"

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
