#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// #include <conio.h>


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

void add_token();

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

// Prompts the user to select a column
void selecting()
{

    clock_t start = clock();

    printf("\nChoose column: ");
    scanf(" %d", &selected);

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

int check(int token)
{
    return CheckHorizontal(token) || CheckVertical(token) || CheckDiagonals(token);
}

// Prompts the players to enter their names
void enterNames()
{
    char player1[32];
    printf("\nPlayer 1 - Enter your name:");
    scanf("%s", player1);

    char player2[32];
    printf("\nPlayer 2 - Enter your name:");
    scanf("%s", player2);

    strcpy(players[0], player1);
    strcpy(players[1], player2);
}

// Toss A coin Randomly to determine which player starts the match, srand seeds rand() every time such that rand() does not stick to the same value.
int coinToss()
{
    srand(time(NULL));
    int random = rand();
    printf("\nTossing A coin .....\n");
    if(random % 2 == 0)
        token = 2;
    else token = 1;
    printf("\n%s is the First to start!\n", players[token - 1]);
}

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
int tieFull()
{
    int countEntries = 0;
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
            if(matrix[i][j] != 0)
                countEntries++;
    }
    if(countEntries == 42)
        return 1;
    else return 0;

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
