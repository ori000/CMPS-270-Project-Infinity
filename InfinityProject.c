#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
USE EXPONENTIAL DISTRIBUTION IN ORDER TO PREDICT WHEN TO BLOCK, PLACE IN SMART WAYS DEPENDING ON DIFFICULTY

Predict block vertically
    If above == null , below == 1 and in bound => place
    If below == null , above == 1 and in bound => place
Predict block horizontally
    If left == null , right == 1 and in bound => place
    If right == null , left == 1 and in bound => place
Predict block oblique 1
    If uLeft == null , lRight == 1 and in bound => place
    If lRight == null , uLeft == 1 and in bound => place
Predict block oblique 2
    If uRight == null , lLeft == 1 and in bound => place
    If lLeft == null , uRight == 1 and in bound => place

Predict win vertically
    If 
Predict win horizontally
Predict win oblique 1
Predict win oblique 2


*/

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
int difficulty = 0;

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
REQUIRES: non-zero lambda

EFFECTS: return exponential random variable
*/
double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}
/*
REQUIRES: non-zero lambda

EFFECTS: return difficulty level based on exponential random variable value
*/
void difficulty(double lambda)
{
    double x = ran_expo(lambda);
    if(x > 100 && x < 200)
        difficulty = 1;
    else if(x > 200)
        difficulty = 2;
    else
        difficulty = 3;
}

/*
REQUIRES:
 - nothing
EFFECTS:
 - Creating A 2D Array and initiallizing all the values to 0
 - Instantiate all entries to 0 (fill in the matrix)
*/

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
REQUIRES:
 - nothing
EFFECTS:
 - Print the matrix with the current results
 - Display/Print the matrix with an indexed square design
*/

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
REQUIRES:
 - nothing
EFFECTS:
 - Prompts the players to enter their names
 - get the name the players by placing the input into char arrays
*/

void enterNames()
{
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
REQUIRES:
 - nothing
EFFECTS:
 - Prompts the user to select a column
 - Choose valid place to drop the ball in & get the time taken for each player
*/

void selecting()
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
 - Defined ROWS macro and 2d matrix
EFFECTS:
 - drop the item into the matrix (gravity simulation)
 - by adding the token to the last available row
*/
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
REQUIRES:
 - token to check the player's input

EFFECTS:
 - Be able to check if the player won horizontally via incrementing the counter in case an index had a player input.
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
REQUIRES: nothing

EFFECTS: block human input on the horizontal axis using exponential distribution
*/
int blockHorizontal()
{
    if(difficulty == 1)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.075);
        if(e > 300)
        {
            for (int i = 0; i < ROWS; ++i)
            {
                for (int j = 0; j < 4; ++j) // 4 is the number of ways of connecting four tokens in one row
                {
                    if(i < ROWS - 1 && matrix[i][j] == 1 && matrix[i+1][j] == NULL)
                        matrix[i+1][j] = 2;
                    else if(i > 0 && matrix[i][j] == 1 && matrix[i-1][j] == NULL)
                        matrix[i-1][j] = 2;
                }
            }
        }
    }
    else if(difficulty == 2)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.005);
        if(e > 300)
        {
            for (int i = 0; i < ROWS; ++i)
            {
                for (int j = 0; j < 4; ++j) // 4 is the number of ways of connecting four tokens in one row
                {
                    if(i < ROWS - 1 && matrix[i][j] == 1 && matrix[i+1][j] == NULL)
                        matrix[i+1][j] = 2;
                    else if(i > 0 && matrix[i][j] == 1 && matrix[i-1][j] == NULL)
                        matrix[i-1][j] = 2;
                }
            }
        }
    }
    else if(difficulty == 3)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.0005);
        if(e > 300)
        {
            for (int i = 0; i < ROWS; ++i)
            {
                for (int j = 0; j < 4; ++j) // 4 is the number of ways of connecting four tokens in one row
                {
                    if(i < ROWS - 1 && matrix[i][j] == 1 && matrix[i+1][j] == NULL)
                        matrix[i+1][j] = 2;
                    else if(i > 0 && matrix[i][j] == 1 && matrix[i-1][j] == NULL)
                        matrix[i-1][j] = 2;
                }
            }
        }
    }
}
/*
REQUIRES: nothing

EFFECTS: block human input on the vertical axis using exponential distribution
*/
int blockVertical()
{
    if(difficulty == 1)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.075);
        if(e > 300)
        {
            for (int j = 0; j < COLS; ++i)
            {
                for (int i = 0; i < 3; ++j) // 4 is the number of ways of connecting four tokens in one row
                {
                    if(j < 3 && matrix[i][j] == 1 && matrix[i][j+1] == NULL)
                        matrix[i][j+1] = 2;
                    else if(j > 0 && matrix[i][j] == 1 && matrix[i][j-1] == NULL)
                        matrix[i][j-1] = 2;
                }
            } 
        }
    }
    else if(difficulty == 2)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.005);
        if(e > 300)
        {
            for (int j = 0; j < COLS; ++i)
            {
                for (int i = 0; i < 3; ++j) // 4 is the number of ways of connecting four tokens in one row
                {
                    if(j < 3 && matrix[i][j] == 1 && matrix[i][j+1] == NULL)
                        matrix[i][j+1] = 2;
                    else if(j > 0 && matrix[i][j] == 1 && matrix[i][j-1] == NULL)
                        matrix[i][j-1] = 2;
                }
            } 
        }
    }
    else if(difficulty == 3)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.0005);
        if(e > 300)
        {
            for (int j = 0; j < COLS; ++i)
            {
                for (int i = 0; i < 3; ++j) // 4 is the number of ways of connecting four tokens in one row
                {
                    if(j < 3 && matrix[i][j] == 1 && matrix[i][j+1] == NULL)
                        matrix[i][j+1] = 2;
                    else if(j > 0 && matrix[i][j] == 1 && matrix[i][j-1] == NULL)
                        matrix[i][j-1] = 2;
                }
            } 
        }
    }
}
/*
REQUIRES: nothing

EFFECTS: block human input on the oblique axis using exponential distribution
*/
int blockOblique()
{
    if (difficulty == 1)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.075);
        if(e > 300)
        {
            for (int i = 0; i < ROWS; i++)
            {
                    for (int j = 0; j < 4; j++)
                    {
                        if (j < COLS - 1 && i < ROWS - 1 && matrix[i][j] == 1 && matrix[i+1][j+1] == NULL)
                            matrix[i+1][j+1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i > 0 && j < COLS - 1 && matrix[i][j] == 1 && matrix[i-1][j+1] == NULL)
                            matrix[i-1][j+1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i > 0 && j > 0 && matrix[i][j] == 1 && matrix[i-1][j-1] == NULL)
                            matrix[i-1][j-1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i < ROWS - 1 && j < > 0 && matrix[i][j] == 1 && matrix[i+1][j-1] == NULL)
                            matrix[i+1][j-1] = 2;
                    }
            }
        }
    }
    else if (difficulty == 2)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.005);
        if(e > 300)
        {
            for (int i = 0; i < ROWS; i++)
            {
                    for (int j = 0; j < 4; j++)
                    {
                        if (j < COLS - 1 && i < ROWS - 1 && matrix[i][j] == 1 && matrix[i+1][j+1] == NULL)
                            matrix[i+1][j+1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i > 0 && j < COLS - 1 && matrix[i][j] == 1 && matrix[i-1][j+1] == NULL)
                            matrix[i-1][j+1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i > 0 && j > 0 && matrix[i][j] == 1 && matrix[i-1][j-1] == NULL)
                            matrix[i-1][j-1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i < ROWS - 1 && j < > 0 && matrix[i][j] == 1 && matrix[i+1][j-1] == NULL)
                            matrix[i+1][j-1] = 2;
                    }
            }
        }
    }
    else if (difficulty == 3)
    {
        srand((unsigned)time(NULL));
        double e;
        for(int x = 0; x < 5; x++)
            e += ran_expo(0.0005);
        if(e > 300)
        {
            for (int i = 0; i < ROWS; i++)
            {
                    for (int j = 0; j < 4; j++)
                    {
                        if (j < COLS - 1 && i < ROWS - 1 && matrix[i][j] == 1 && matrix[i+1][j+1] == NULL)
                            matrix[i+1][j+1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i > 0 && j < COLS - 1 && matrix[i][j] == 1 && matrix[i-1][j+1] == NULL)
                            matrix[i-1][j+1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i > 0 && j > 0 && matrix[i][j] == 1 && matrix[i-1][j-1] == NULL)
                            matrix[i-1][j-1] = 2;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (i < ROWS - 1 && j < > 0 && matrix[i][j] == 1 && matrix[i+1][j-1] == NULL)
                            matrix[i+1][j-1] = 2;
                    }
            }
        }
    }
}
/*
REQUIRES:
 - token to check player input

EFFECTS:
 - Be able to check if the player won vertically via incrementing the counter in case an index had a player input.
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
REQUIRES:
 - nothing

EFFECTS:
 - Check if the player (1 or 2) won diagonally by counting the lines/direct diagonal coins of the same number
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
        { // this is for the upper diagonals, that will check whether there are 4 dots connected diagonally
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
 - token reference variable (through main)
EFFECTS:
 - check if any player won through a horizontal, vertical or diagonal input
*/

int check(int token)
{
    return CheckHorizontal(token) || CheckVertical(token) || CheckDiagonals(token);
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
void print_rules() {
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

int main()
{
    createMatrix();
    print_rules();
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

    system("pause");
    return 0;
}
