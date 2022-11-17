//tic-tac-toe

#include <3ds.h>
#include <stdio.h>
#include <stdbool.h>

#define BOARDSIZE 3

void printTable(int size, char board[size][size], int curX, int curY, bool cursor, char currentSymbol);
bool insertChecker(int size, char board[size][size], int CurX, int CurY, char symbol);
bool isFull(int size, char board[size][size]);
bool checkForWin(int size, char board[size][size]);

int main(int argc, char **argv)
{

    gfxInitDefault();

    PrintConsole topScreen, bottomScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    char board[BOARDSIZE][BOARDSIZE] = {{'X', ' ', 'O'}, {'O', 'X', ' '}, {' ', 'O', 'X'}};
    //char board[BOARDSIZE][BOARDSIZE] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};

    for (int i = 0; i < BOARDSIZE; i++)
    {
        for (int x = 0; x < BOARDSIZE; x++)
        {
            //Fill board with space chars
            board[i][x] = ' ';
        }
    }

    bool updateTop = true;
    bool showCursor = false;
    int curX = 1;
    int curY = 1;
    char symbol = 'O';

    consoleSelect(&bottomScreen);
    printf("Welcome to Tic-Tac-Toe!\nPress start to exit.\nPress A to place checker on board.\nPress B to hide cursor.\nUse D-pad to move cursor\n");

    while (aptMainLoop())
    {

        if (updateTop)
        {
            consoleSelect(&topScreen);
            printTable(BOARDSIZE, board, curX, curY, showCursor, symbol);
            consoleSelect(&bottomScreen);
            updateTop = false;
            showCursor = false;
        }

        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
        {
            //exit to homebrew launcher
            break;
        }

        if (kDown & KEY_B)
        {
            showCursor = false;
            updateTop = true;
        }

        if (kDown & KEY_A)
        {
            consoleSelect(&bottomScreen);
            //printf("curX:%d curY:%d\n", curX, curY);
            printf("A Pressed\n");

            if (!showCursor)
            {
                showCursor = true;
                continue;
            }

            bool isValid = insertChecker(BOARDSIZE, board, curX, curY, symbol);

            if (isValid)
            {
                printf("Inserted %c at %d,%d\n", symbol, curX, curY);

                //reset symbol
                if (symbol == 'O')
                    symbol = 'X';
                else if (symbol == 'X')
                    symbol = 'O';
            }
            else
            {
                printf("There is already a checker there.\n");
            }

            updateTop = true;
            //showCursor = false;

            if (isFull(BOARDSIZE, board))
            {
                printf("Board is full game over :(\n");
            }
        }

        //Cursor movement
        if (kDown & KEY_DUP)
        {
            if (curY > 0)
            {
                curY--;
                updateTop = true;
                showCursor = true;
            }
        }
        else if (kDown & KEY_DDOWN)
        {
            if (curY < 2)
            {
                curY++;
                updateTop = true;
                showCursor = true;
            }
        }
        else if (kDown & KEY_DLEFT)
        {
            if (curX > 0)
            {
                curX--;
                updateTop = true;
                showCursor = true;
            }
        }
        else if (kDown & KEY_DRIGHT)
        {
            if (curX < 2)
            {
                curX++;
                updateTop = true;
                showCursor = true;
            }
        }

        //Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();
    }

    // Exit services
    gfxExit();
    return 0;
}

bool insertChecker(int size, char board[size][size], int CurX, int CurY, char symbol)
{
    if (board[CurY][CurX] == ' ')
    {
        board[CurY][CurX] = symbol;
        return true;
    }

    return false;
}

bool isFull(int size, char board[size][size])
{
    int counter = 0;
    for (int row = 0; row < BOARDSIZE; row++)
    {
        for (int col = 0; col < BOARDSIZE; col++)
        {
            if (board[row][col] != ' ')
            {
                counter++;
            }
        }
    }
    if (counter == (BOARDSIZE * BOARDSIZE))
    {
        return true;
    }

    return false;
}

void printTable(int size, char board[size][size], int curX, int curY, bool cursor, char currentSymbol)
{

    consoleClear();
    printf(" =============\n");
    for (int row = 0; row < BOARDSIZE; row++)
    {
        for (int col = 0; col < BOARDSIZE; col++)
        {
            printf(" = ");

            if (cursor && (curX == col && curY == row))
            {
                printf("\x1b[47;30m"); //set white background and black text
            }

            if (board[row][col] == 'X')
            {
                //print red X
                printf("\x1b[31mX\x1b[0m");
            }
            else if (board[row][col] == 'O')
            {
                //print blue O
                printf("\x1b[34mO\x1b[0m");
            }
            else
            {
                printf(" ");
            }

            printf("\x1b[0m"); //reset color
        }
        printf(" =\n");
        printf(" =============\n");
    }
    printf("\nCurrent player: %c", currentSymbol);
}