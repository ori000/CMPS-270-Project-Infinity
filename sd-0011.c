#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// #include <conio.h>

/*
TEST CASES:

Horizontal & Vertical: Osama
[1,1,1,1] starting at indices 0,1,2,3 for all rows: check
[2,2,2,2] starting at indices 0,1,2,3 for all rows: check
[1]   [2]
[1] , [2]   starting at row 0,1,2 for all columns: check
[1]   [2]
[1]   [2]

Diagonal: Ayla & ???
Out of bounds & invalid:

Handle invalid input at out of bounds indices: check
Handle invalid(NaN) input for placing balls: check


*/

int hour = 0, minute = 0, second = 0;

#define ROWS 6
#define COLS 7

int matrix[ROWS][COLS]; // A 2D Array representing the game board
char players[2][64];    // Holds the players names
float timePerPlayer[2]; // stores the total time taken by each player

char border[] = " ----------------------------";
char guide[] = "  0   1   2   3   4   5   6  ";

int token;        // Current player Turn
int selected = 0; // Current Player Column selection

void createMatrix();
void display();
void enterNames();
void coinToss();
void selecting();
void add_token();
int check();
int tieFull();
void tieTime();
void replaceSpaces(char p[32]);


/*
REQUIRES: nothing

EFFECTS:
Instantiate all entries to 0 (fill in the matrix)
*/

// Creating A 2D Array and initiallizing all the values to 0
void createMatrix()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

/*
REQUIRES: nothing

EFFECTS:
Display/Print the matrix with an indexed square design
*/

// print the matrix with the current results
void display()
{
    printf("%s\n", border);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("| %d ", matrix[i][j]);
        }
        printf("|\n");
    }
    printf("%s\n", border);
    printf("%s\n", guide);
}


/*
REQUIRES: nothing

EFFECTS:
get the name the players by placing the input into char arrays
*/

// Prompts the players to enter their names
void enterNames()
{
    char player1[32];
    printf("\nPlayer 1 - Enter your name:");
    fgets(player1, 32, stdin);
    replaceSpaces(player1);

    char player2[32];
    printf("\nPlayer 2 - Enter your name:");
    fgets(player2, 32, stdin);
    replaceSpaces(player2);

    strcpy(players[0], player1);
    strcpy(players[1], player2);
}
/*
REQUIRES: nothing

EFFECTS:
Be able to have two choices to choose for both players each in order to check who starts first.
*/
// Toss A coin Randomly to determine which player starts the match, srand seeds rand() every time since rand() does not stick to the same value.
void coinToss()
{
    srand(time(NULL));
    int random = rand();
    printf("\nTossing A coin .....\n");
    if (random % 2 == 0)
        token = 2;
    else
        token = 1;
    printf("\n%s is the First to start!\n", players[token - 1]);
}


/*
REQUIRES: nothing

EFFECTS:
choose valid place to drop the ball in & get the time taken for each player
*/

// Prompts the user to select a column
void selecting()
{

    clock_t start = clock();

    printf("\nChoose column: ");
    while (scanf(" %d", &selected) != 1)
    {
        printf("Invalid selection. Please enter an integer: ");
        while (getchar() != '\n')
            ;
    }

    // checks if the user entered a valid position
    if ((selected <= 6 && selected >= 0) && matrix[0][selected] == 0)
    {
        add_token();
        token = (token == 1) ? 2 : 1; // flipping the token after adding it
    }
    else
    {
        printf("Invalid selection. Please select a column between 0 and 6");
        selecting();
    }

    clock_t end = clock();

    // Adding the time of the total time taken by the player
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    timePerPlayer[(token == 1) ? 1 : 0] += seconds; // Take the opposite of the current token since it was flipped after selecting
    printf("%f", seconds);
}

/*
REQUIRES:

Defined ROWS macro and 2d matrix
EFFECTS:

drop the item into the matrix (gravity simulation)

*/

// adding the token to the last available row
void add_token()
{
    int curRow;
    for (curRow = ROWS - 1; curRow >= 0; curRow--)
    {
        if (matrix[curRow][selected] == 0)
        {
            matrix[curRow][selected] = token;
            break;
        }
    }
}
/*
REQUIRES: token to check the player's input

EFFECTS:
Be able to check if the player won horizontally via incrementing the counter in case an index had a player input.
*/

int CheckHorizontal(int token)
{
    int counter;
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < 4; ++j) // 4 is the number of ways of connecting four tokens in one row
        {
            counter = 0;
            for (int k = 0; k < 4; ++k) // 4 in a row
            {
                if (matrix[i][j + k] == token)
                    counter++;
            }
            if (counter == 4)
                return 1;
        }
    }
    return 0;
}
/*
REQUIRES: token to check player input

EFFECTS:
Be able to check if the player won vertically via incrementing the counter in case an index had a player input.
*/

int CheckVertical(int token)
{
    int counter;
    for (int j = 0; j < COLS; ++j)
    {
        for (int i = 0; i < 3; ++i) // 3 is the number of ways of connecting four tokens in one cloumn
        {
            counter = 0;
            for (int k = 0; k < 4; ++k) // 4 in a row
            {
                if (matrix[i + k][j] == token)
                    counter++;
            }
            if (counter == 4)
                return 1;
        }
    }
    return 0;
}

/*
REQUIRES: nothing

EFFECTS:

check if the player (1 or 2) won diagonally by counting the lines/direct diagonal coins of the same number
*/

int CheckDiagonals(int token)
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
                    if (matrix[i + a][j + a] == token)
                        counter++;
                }
                if (counter == 4)
                    return 1;
            }
        }
        else
        { // this is for the lower diagonals, that will check whether there are 4 dots connected diagonally
            for (int j = 0; j < 4; j++)
            {
                counter = 0;
                for (int a = 0; a < 4; ++a)
                {
                    if (matrix[i - a][j + a] == token)
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
token reference variable (through main)
EFFECTS:
check if any player won through a horizontal, vertical or diagonal input
*/

int check(int token)
{
    return CheckHorizontal(token) || CheckVertical(token) || CheckDiagonals(token);
}

/*
REQUIRES: nothing
EFFECTS:
in case of a tie (full matrix), print the winner based on time taken (less -> win, more -> lose)
*/

// Compare the time taken by each player to determine the winner in the case of a tie
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
REQUIRES: nothing

EFFECTS:
Determine whether the matrix is full or not by counting the number of entries that are not equal to 0 each.
*/
int tieFull()
{
    int countEntries = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
            if (matrix[i][j] != 0)
                countEntries++;
    }
    if (countEntries == 42)
        return 1;
    else
        return 0;
}
/*
REQUIRES: char array of size 32

EFFECTS: replace any spaces in the input with _
*/
void replaceSpaces(char p[32])
{
    for(int i = 0; i < 32; i++)
    {
        if(p[i] == ' ')
            p[i] = '_';
    }
}
int main()
{
    createMatrix();
    display();
    enterNames();
    coinToss();

    while (!(check(1) || check(2) || tieFull()))
    {
        printf("%s, your turn!\n", players[token - 1]);

        selecting();

        printf("\n\n");

        display();
    }
    if (check(1))
    {
        printf("\n\n%s wins!\n\n", players[0]);
    }
    if (check(2))
    {
        printf("\n\n%s wins!\n\n", players[1]);
    }
    if (tieFull())
    {
        tieTime();
    }
    return 0;
}