# Mine
This program is my remake of game Minesweeper as a console application
written in C. User choose dimensions of board of fields and a number of
mines randomly set in fields. Then, user can open a field specified by 
coordinates or set flag to mark risky field. When user open field where
mine is set, the rest of mines wiil be displayed on board and game is over.
If user open an empty field, a count of mines surrounded by that field
will be displayed, and if there are no mines surrounding a field, then
all fields around will be opened. When all fields with no mines are open,
then user solved mine board and game is over.

## User interface
Game starts with entering dimensions of board (number of rows and columns) and number of mines

    Enter board dimensions (1 < row <= 16, 1 < col <= 76) and number of mines (0 < n < row * col): 6 6 4
*Example: Creating a board of 6 rows, 6 columns and 4 mines inside*
    
Then, game is created and program display board with horizontal ruler (number 5 in first row, shows current column number) and vertical ruler (numbers on left side, show current row number) and fields.

          5
          |
    1 ??????
    2 ??????
    3 ??????
    4 ??????
    5 ??????
    6 ??????
*A board with fields*

`0`..`8` - number of mines in surrounding fields
`?` - field is not open yet
`*` - there is a mine set in a field
`%` - stepped on mine (shows at game over)
`#` - flag set in a field
`x` - flag set on a field without mine (shows at game over)

    Still 4 unmarked mines remain.
*Message of remaining mines (number of mines - number of flags)*

    o-open field f-set/remove flag l-legend q-quit
*Game menu*
    
### Menu commands
`o` - open field

    o-open field f-set/remove flag l-legend q-quit o 1 1
*Open field command example: Open field with coordinates row 1 column 1*

`f` - set/remove flag
Flag can be set on field to protect from opening that field, or removed so that field is not protected from opening.

    o-open field f-set/remove flag l-legend q-quit f 6 6
*Set/remove flag command example: Set or remove flag from field with coordinates row 6 column 6*

`l` - legend
Shows legend of symbols used in board

`q` - quit game

### Game over
If all fields with no mines inside are open, then board is solved and message *Congratulations. Mine field solved* will appear on the bottom of the board.

          5
          |
    1 0001?1
    2 110111
    3 ?10011
    4 11001?
    5 111011
    6 1?1000

    Congratulations. Mine field solved.
*Example of solved board*

Also if field with mine is opened, then rest of mines will be displayed and the message *Stepped on mine. Game over* will appear.

           5
           |
     1 1%????
     2 11????
     3 ??????
     4 ?*????
     5 ????*?
     6 ??*???

    Stepped on mine. Game over.
*Example of open field on row 1, column 2 which contains mine*

When game is over, a message on the bottom will appear

    Do you want to continue (y/n)?

Press `y` or `Y` for new game, otherwise program will be terminated.
