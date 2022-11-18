#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "Infinity.h"

// #include <conio.h>

/*
TEST CASES:

Horizontal & Vertical:
[1,1,1,1] starting at indices 0,1,2,3 for all rows: check
[2,2,2,2] starting at indices 0,1,2,3 for all rows: check
[1]   [2]
[1] , [2]   starting at row 0,1,2 for all columns: check
[1]   [2]
[1]   [2]

Diagonal:

[1]             starting at indeces 0,1,2,3 for rows 1,2,3
   [1]          it will check the first 3 rows and 4 columns accotdingly
      [1]       for all the lower diagonals possible in the game
         [1]


         [1]    starting at indices 0,1,2,3 for rows 4,5,6
      [1]       it will check the last 3 rows and first 4 columns
   [1]          accordingly for all the upper diagonals possible in the game
[1]

same goes to the value [2]




Out of bounds & invalid:

Handle invalid input when inserting into a column at out of bounds indices:
Propt the user to re-enter a valid column index when the user enters:
- a negative number (e.g. -1)
- a number greater than the number of columns(6)
- a non-integer value (e.g. "a")

Handle invalid(NaN) input for placing balls: check


*/


int depthChanger = 7;

int selected = 0; // Current Player Column selection


/*
REQUIRES:
 - nothing
EFFECTS:
 - Print the matrix with the current results
 - Display/Print the matrix with an indexed square design
*/

// helper functions
bool boardFull(State board[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] != EMPTY)
            {
                return false;
            }
        }
    }
    return true;
}

bool inInterval(int x, int min, int max)
{ // inclusive
    if (x <= max && x >= min)
    {
        return true;
    }
    return false;
}

void randommove(State board[ROWS][COLS])
{
    int col;
    do
    {
        col = rand() % 7;
    } while (board[0][col] != EMPTY);

    for (int i = ROWS - 1; i >= 0; i--)
    {
        if (board[i][col] == EMPTY)
        {
            board[i][col] = BOT;
            return;
        }
    }
}

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int min(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}

void legalmoves(State board[ROWS][COLS], position moves[COLS])
{
    int cnt = 0;
    for (int i = 0; i < COLS; i++)
    {
        for (int j = ROWS - 1; j >= 0; j--)
        {
            if (board[j][i] == EMPTY)
            {
                moves[cnt].row = j;
                moves[cnt].col = i;
                cnt++;
                if (cnt >= COLS)
                    return;
                break;
            }
        }
    }
}

void display(State board[ROWS][COLS])
{
    printf("%s\n", border);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("| %d ", board[i][j]);
        }
        printf("|\n");
    }
    printf("%s\n", border);
    printf("%s\n", guide);
}

/*
REQUIRES:
 - nothing
EFFECTS:
 - Prompts the players to enter their names
 - get the name the players by placing the input into char arrays
*/

void enterNames()
{
    fflush(stdin);
    char player1[32];
    printf("\nPlayer 1 - Enter your name:");
    fgets(player1, 32, stdin);
    player1[strcspn(player1, "\n")] = 0;
    replaceSpaces(player1);

    char player2[32];
    printf("\nPlayer 2 - Enter your name:");
    fgets(player2, 32, stdin);
    player2[strcspn(player2, "\n")] = 0;
    replaceSpaces(player2);

    strcpy(players[0], player1);
    strcpy(players[1], player2);
}
/*
REQUIRES:
 - nothing
EFFECTS:
 - Toss A coin Randomly to determine which player starts the match, srand seeds rand() every time since rand() does not stick to the same value.
 - Be able to have two choices to choose for both players each in order to check who starts first.
*/
State coinToss()
{
    State key;
    srand(time(NULL));
    int random = rand();
    printf("\nTossing A coin .....\n");
    if (random % 2 == 0)
        key = 2;
    else
        key = 1;
    printf("\n%s is the First to start!\n", players[key - 1]);
    return key;
}

/*
REQUIRES:
 - nothing
EFFECTS:
 - Prompts the user to select a column
 - Choose valid place to drop the ball in & get the time taken for each player
*/

void playerSelect(State board[ROWS][COLS], State key)
{

    clock_t start = clock();
    printf("\nChoose column: ");
    char term = ' ';
    while (scanf("%d%c", &selected, &term) != 2 || term != '\n')
    {
        printf("Invalid selection. Please enter an integer: ");
        while (getchar() != '\n')
            ;
    }

    // checks if the user entered a valid position
    if ((selected <= 6 && selected >= 0) && board[0][selected] == 0)
    {
        add_token(board, key);
    }
    else
    {
        printf("Invalid selection. Please select a column between 0 and 6");
        playerSelect(board,key);
    }

    clock_t end = clock();

    // Adding the time of the total time taken by the player
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    timePerPlayer[(key == 1) ? 1 : 0] += seconds; // Take the opposite of the current token since it was flipped after selecting
    printf("%f", seconds);
}

/*
REQUIRES:
 - Defined ROWS macro and 2d matrix
EFFECTS:
 - drop the item into the matrix (gravity simulation)
 - by adding the token to the last available row
*/
void add_token(State board[ROWS][COLS], State key)
{
    int curRow;
    for (curRow = ROWS - 1; curRow >= 0; curRow--)
    {
        if (board[curRow][selected] == 0)
        {
            board[curRow][selected] = key;
            break;
        }
    }
}

/*
REQUIRES:
 - token to check the player's input

EFFECTS:
 - Be able to check if the player won horizontally via incrementing the counter in case an index had a player input.
*/
int CheckHorizontal(State board[ROWS][COLS], State key)
{
    int counter;
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < 4; ++j) // 4 is the number of ways of connecting four tokens in one row
        {
            counter = 0;
            for (int k = 0; k < 4; ++k) // 4 in a row
            {
                if (board[i][j + k] == key)
                    counter++;
            }
            if (counter == 4)
                return 1;
        }
    }
    return 0;
}
/*
REQUIRES:
 - token to check player input

EFFECTS:
 - Be able to check if the player won vertically via incrementing the counter in case an index had a player input.
*/

int CheckVertical(State board[ROWS][COLS], State key)
{
    int counter;
    for (int j = 0; j < COLS; ++j)
    {
        for (int i = 0; i < 3; ++i) // 3 is the number of ways of connecting four tokens in one cloumn
        {
            counter = 0;
            for (int k = 0; k < 4; ++k) // 4 in a row
            {
                if (board[i + k][j] == key)
                    counter++;
            }
            if (counter == 4)
                return 1;
        }
    }
    return 0;
}

/*
REQUIRES:
 - nothing

EFFECTS:
 - Check if the player (1 or 2) won diagonally by counting the lines/direct diagonal coins of the same number
*/

int CheckDiagonals(State board[ROWS][COLS], State key)
{
    int counter;

    for (int i = 0; i < ROWS; i++)
    {
        if (i < 3)
        { // this is for the lower diagonals, that will check whether there are 4 dots connected diagonally
            for (int j = 0; j < 4; j++)
            {
                counter = 0;
                for (int a = 0; a < 4; ++a)
                {
                    if (board[i + a][j + a] == key)
                        counter++;
                }
                if (counter == 4)
                    return 1;
            }
        }
        else
        { // this is for the upper diagonals, that will check whether there are 4 dots connected diagonally
            for (int j = 0; j < 4; j++)
            {
                counter = 0;
                for (int a = 0; a < 4; ++a)
                {
                    if (board[i - a][j + a] == key)
                        counter++;
                }
                if (counter == 4)
                    return 1;
            }
        }
    }
    return 0;
}

/*
REQUIRES:
 - token reference variable (through main)
EFFECTS:
 - check if any player won through a horizontal, vertical or diagonal input
*/

int check(State board[ROWS][COLS], State key)
{
    return CheckHorizontal(board, key) || CheckVertical(board, key) || CheckDiagonals(board, key);
}

/*
REQUIRES:
 - nothing
EFFECTS:
 - Compare the time taken by each player to determine the winner in the case of a tie
 - in case of a tie (full matrix), print the winner based on time taken (less -> win, more -> lose)
*/
void tieTime()
{
    printf("\n%s Time was %f seconds and %s Time was %f seconds.", players[0], timePerPlayer[0], players[1], timePerPlayer[1]);
    if (timePerPlayer[0] > timePerPlayer[1])
    {
        printf("\n\n%s wins!\n\n", players[1]);
    }
    else
    {
        printf("\n\n%s wins!\n\n", players[2]);
    }
}
/*
REQUIRES:
 - nothing

EFFECTS:
 - Determine whether the matrix is full or not by counting the number of entries that are not equal to 0 each.
*/
int tieFull(State board[ROWS][COLS])
{
    int countEntries = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
            if (board[i][j] != 0)
                countEntries++;
    }
    if (countEntries == 42)
        return 1;
    else
        return 0;
}
/*
REQUIRES:
 - char array of size 32
EFFECTS:
 - replace any spaces in the input with _
*/
void replaceSpaces(char p[32])
{
    for (int i = 0; i < 32; i++)
    {
        if (p[i] == ' ')
            p[i] = '_';
    }
}

/*
    REQUIRES:
    -Nothing

    EFFECTS:
    -print the rules of the game
*/
void print_rules()
{
    printf("Rules:\n");
    printf("1. The game board has seven columns and six rows.\n");
    printf("2. There are 21 red (number 1) and 21 yellow (number 2) tokens.\n");
    printf("3. One player plays with red tokens, the other with yellow tokens.\n");
    printf("4. The tokens are inserted at the top of a column, and they will fall down and land on the ground (if the column was empty) or on top of a previously inserted token.\n");
    printf("5. Red starts (number 1) .\n");
    printf("6. Red and yellow take turns.\n");
    printf("7. One can only insert tokens in one of the seven columns.\n");
    printf("8. One cannot insert a token into a column that is full.\n");
    printf("9. A line consists of several tokens, either in vertical, horizontal, or diagonal form, which contain only tokens of the same color.\n");
    printf("10. A player wins if they manage to form a line of four tokens of their color.\n");
    printf("11. The game ends if one of the players wins.\n");
    printf("12. There will be no ties. In case of a tie on the board, the player that took less over-all time wins.\n\n");
    printf("Let's start playing\n");
}

int minmax(State arr[ROWS][COLS], int isMaximizing, int depth, int alpha, int beta)
{
    int score = evaluate(arr);
    if (score == -100 || score == 100 || depth > depthChanger)
        return score;
    else if (boardFull(arr))
        return 0;

    position moves[COLS];
    memset(moves, -1, sizeof(moves));

    if (isMaximizing)
    {
        int best = -10000;
        legalmoves(arr, moves);
        for (int i = 0; i < COLS; i++)
        {
            if (moves[i].row != -1)
            {
                arr[moves[i].row][moves[i].col] = BOT;
                int eval = minmax(arr, 0, depth + 1, alpha, beta);
                best = max(best, eval);
                arr[moves[i].row][moves[i].col] = EMPTY;
                alpha = max(alpha, eval);
                if (beta <= alpha)
                    break;
            }
            else
                break;
        }
        return best - depth;
    }
    else
    {
        int best = 10000;
        legalmoves(arr, moves);
        for (int i = 0; i < COLS; i++)
        {
            if (moves[i].row != -1)
            {
                arr[moves[i].row][moves[i].col] = PLAYER;
                int eval = minmax(arr, 1, depth + 1, alpha, beta);
                best = min(best, eval);
                arr[moves[i].row][moves[i].col] = EMPTY;
                beta = min(beta, eval);
                if (beta <= alpha)
                    break;
            }
            else
                break;
        }
        return best + depth;
    }
}

int evaluate(State arr[ROWS][COLS])
{
    int tmp = win(arr);
    int scoreP = 0;
    int scoreC = 0;
    if (tmp == 2)
        return 100;
    else if (tmp == 1)
        return -100;
    else if (tmp == -1)
        return 0;
    else
    {
        for (int i = ROWS - 1; i >= 0; i--)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (arr[i][j] == PLAYER)
                {
                    scoreP += countDiagb(arr, i, j);
                    scoreP += countCol(arr, i, j);
                    scoreP += countRow(arr, i, j);
                    scoreP += countDiag(arr, i, j);
                }
                else if (arr[i][j] == BOT)
                {
                    scoreC += countDiagb(arr, i, j);
                    scoreC += countCol(arr, i, j);
                    scoreC += countRow(arr, i, j);
                    scoreC += countDiag(arr, i, j);
                }
            }
        }
    }
    return scoreC - scoreP;
}

position findBestmove(State arr[ROWS][COLS])
{
    int bestVal = -10000;
    position bestMove;
    position moves[COLS];
    memset(moves, -1, sizeof(moves));

    legalmoves(arr, moves);
    for (int i = 0; i < COLS; i++)
    {
        if (moves[i].row != -1)
        {
            arr[moves[i].row][moves[i].col] = BOT;
            int moveVal = minmax(arr, 0, 0, -10000000, 10000000);
            if (moveVal > bestVal)
            {
                bestMove.row = moves[i].row;
                bestMove.col = moves[i].col;
                bestVal = moveVal;
            }
            arr[moves[i].row][moves[i].col] = EMPTY;
        }
    }
    return bestMove;
}
int countCol(State board[ROWS][COLS], int startRow, int startCol)
{
    int cnt = 0;
    int i = startRow;
    while (board[i][startCol] == board[startRow][startCol])
    {
        cnt++;
        i--;
        if (i < 0)
            return cnt;
    }
    return cnt;
}

int countRow(State board[ROWS][COLS], int startRow, int startCol)
{
    int cnt = 0;
    int i = startCol;
    while (board[startRow][i] == board[startRow][startCol])
    {
        cnt++;
        i++;
        if (i > COLS - 1)
            return cnt;
    }
    return cnt;
}

int countDiag(State board[ROWS][COLS], int startRow, int startCol)
{
    int cnt = 0;
    int row = startRow;
    int col = startCol;
    while (board[row][col] == board[startRow][startCol])
    {
        cnt++;
        row--;
        col++;
        if (row < 0 || col > COLS - 1)
            return cnt;
    }
    return cnt;
}

int countDiagb(State board[ROWS][COLS], int startRow, int startCol)
{
    int cnt = 0;
    int row = startRow;
    int col = startCol;
    while (board[row][col] == board[startRow][startCol])
    {
        cnt++;
        row--;
        col--;
        if (row < 0 || col < 0)
            return cnt;
    }
    return cnt;
}

int win(State board[ROWS][COLS])
{
    if (boardFull(board))
        return -1;
    for (int i = ROWS; i >= 0; i--)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] != EMPTY)
            {
                if (countCol(board, i, j) == 4 || countRow(board, i, j) == 4 || countDiag(board, i, j) == 4 || countDiagb(board, i, j) == 4)
                {
                    switch (board[i][j])
                    {
                    case PLAYER:
                        return PLAYER;
                        break;
                    case BOT:
                        return BOT;
                        break;
                    case EMPTY:
                        continue;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

void computermove(State arr[ROWS][COLS], position move)
{
    arr[move.row][move.col] = BOT;
}
int run()
{
    printf("For player vs player type 0\nFor player vs computer type 1\n");

    int gameType = 0;

    scanf("%d", &gameType);

    print_rules();
    State matrix[ROWS][COLS] = {0};
    State key = 1;
    State ONE =1;
    State TWO = 2;
    
    if (gameType == 0)
    {
        display(matrix);
        enterNames();
        key = coinToss();
        while (!(check(matrix, ONE) || check(matrix, TWO) || tieFull(matrix)))
        {
            printf("%s, your turn!\n", players[key- 1]);
            printf("Token: %d \n", key);

            playerSelect(matrix,key);

            printf("\n\n");

            display(matrix);

            if(key == PLAYER) key = BOT;
            else key = PLAYER;
        }
        if (check(matrix, ONE))
        {
            printf("\n\n%s wins!\n\n", players[0]);
        }
        if (check(matrix, TWO))
        {
            printf("\n\n%s wins!\n\n", players[1]);
        }
        if (tieFull(matrix))
        {
            tieTime();
        }

        system("pause");
    }
    else
    {
         printf("\nDifficulty:\n\nFor easy type 0\nFor medium type 1\nFor hard type 2\n");

        int difficulty = 0;
        scanf("%d", &difficulty);

        switch (difficulty)
        {
        case 0:
            depthChanger = -1;
            break;
        case 1:
            depthChanger = 0;
            break;
        case 2:
            depthChanger = 7;
            break;
        
        default:
            break;
        }
        srand(time(NULL));
        int run = 1;
        while (run)
        {
            playerSelect(matrix,key);
            display(matrix);
            if (win(matrix))
            {
                run = 0;
                break;
            }
            computermove(matrix, findBestmove(matrix));
            display(matrix);
            if (win(matrix))
            {
                run = 0;
                break;
            }
        }
        if (win(matrix) == PLAYER)
        {
            printf("PLAYER WON!\n");
        }
        else if (win(matrix) == BOT)
            printf("COMPUTER WON!\n");
        else if (win(matrix) == -1)
            printf("DRAW\n");
    }

    return 0;
}

int main()
{
    run();
    // printf("For player vs player type 0\nFor player vs computer type 1\n");

    // int gameType = 0;

    // scanf("%d", &gameType);

    // print_rules();
    // State matrix[ROWS][COLS] = {0};
    // State key = 1;
    // State ONE =1;
    // State TWO = 2;
    
    // if (gameType == 0)
    // {
    //     display(matrix);
    //     enterNames();
    //     key = coinToss();
    //     while (!(check(matrix, ONE) || check(matrix, TWO) || tieFull(matrix)))
    //     {
    //         printf("%s, your turn!\n", players[key- 1]);
    //         printf("Token: %d \n", key);

    //         playerSelect(matrix,key);

    //         printf("\n\n");

    //         display(matrix);

    //         if(key == PLAYER) key = BOT;
    //         else key = PLAYER;
    //     }
    //     if (check(matrix, ONE))
    //     {
    //         printf("\n\n%s wins!\n\n", players[0]);
    //     }
    //     if (check(matrix, TWO))
    //     {
    //         printf("\n\n%s wins!\n\n", players[1]);
    //     }
    //     if (tieFull(matrix))
    //     {
    //         tieTime();
    //     }

    //     system("pause");
    // }
    // else
    // {
    //      printf("\nDifficulty:\n\nFor easy type 0\nFor medium type 1\nFor hard type 2\n");

    //     int difficulty = 0;
    //     scanf("%d", &difficulty);

    //     switch (difficulty)
    //     {
    //     case 0:
    //         depthChanger = -1;
    //         break;
    //     case 1:
    //         depthChanger = 0;
    //         break;
    //     case 2:
    //         depthChanger = 7;
    //         break;
        
    //     default:
    //         break;
    //     }
    //     srand(time(NULL));
    //     int run = 1;
    //     while (run)
    //     {
    //         playerSelect(matrix,key);
    //         display(matrix);
    //         if (win(matrix))
    //         {
    //             run = 0;
    //             break;
    //         }
    //         computermove(matrix, findBestmove(matrix));
    //         display(matrix);
    //         if (win(matrix))
    //         {
    //             run = 0;
    //             break;
    //         }
    //     }
    //     if (win(matrix) == PLAYER)
    //     {
    //         printf("PLAYER WON!\n");
    //     }
    //     else if (win(matrix) == BOT)
    //         printf("COMPUTER WON!\n");
    //     else if (win(matrix) == -1)
    //         printf("DRAW\n");
    // }

    // return 0;
}
