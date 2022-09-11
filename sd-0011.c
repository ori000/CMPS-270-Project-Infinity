#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

// struct variables
// {
//     int v00;
//     int v01;
//     int v02;
//     int v03;
//     int v04;
//     int v05;
//     int v06;
//     int v10;
//     int v11;
//     int v12;
//     int v13;
//     int v14;
//     int v15;
//     int v16;
//     int v20;
//     int v21;
//     int v22;
//     int v23;
//     int v24;
//     int v25;
//     int v26;
//     int v30;
//     int v31;
//     int v32;
//     int v33;
//     int v34;
//     int v35;
//     int v36;
//     int v40;
//     int v41;
//     int v42;
//     int v43;
//     int v44;
//     int v45;
//     int v46;
//     int v50;
//     int v51;
//     int v52;
//     int v53;
//     int v54;
//     int v55;
//     int v56;
// };

int hour=0;minute=0;second=0;flag=0;
int input[2];
int matrix[6][7];

void delay(ms)
{
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

void printMatrix(int matrix[6][7])
{
    for(int i = 0; i <= 5; i++)
    {
        printf("|");
        for(int j = 0; j <= 6; j++)
        {
            printf("%d",matrix[i][j]);
            printf("|");
        }
    }
}

int counter()
{
    while(!kbhit() && flag == 0)
    {
        if(minute > 59)
        {
            minute = 0;
            ++hour;
        }
        if(second > 59)
        {
            second = 0;
            ++minute;
        }
        ++second;
    }
}
//Check Horizontal
void checkH(int r, int c)
{
    for(int i = 0; i <= 5; i++)
    {
        for(int j = 0; j <= 3; j++)
        {
            if(matrix[i][j] == 1 && matrix[i][j+1] == 1 && matrix[i][j+2] == 1 && matrix[i][j+3] == 1)
            {
                printf("Player 1 won!");
                checkWinPlayer1 = 1;
                break;
            }
            else if(matrix[i][j] == 2 && matrix[i][j+1] == 2 && matrix[i][j+2] == 2 && matrix[i][j+3] == 2)
            {
                printf("Player 2 won!");
                checkWinPlayer2 = 1;
                break;
            }
        }
    }
}
//Check Vertical
void checkV(int r, int c)
{
    for(int j = 0; j <= 6; j++)
    {
        for(int i = 0; i <= 2; i++)
        {
            if(matrix[i][j] == 1 && matrix[i+1][j] == 1 && matrix[i+2][j] == 1 && matrix[i+3][j] == 1)
            {
                printf("Player 1 won!");
                checkWinPlayer1 = 1;
                break;
            }
            else if(matrix[i][j] == 2 && matrix[i+1][j] == 2 && matrix[i+2][j] == 2 && matrix[i+3][j] == 2)
            {    
                printf("Player 2 won!");
                checkWinPlayer2 = 1;
                break;
            }
        }
    }
}

/*
    ASC

    i=3j=0, i=2j=1, i=1j=2, i=0j=3
    i=4j=0, i=3j=1, i=2j=2, i=1j=3, i=0j=4
    i=5j=0, i=4j=1, i=3j=2, i=2j=3, i=1j=4, i=0j=5
    i=5j=1, i=4j=2, i=3j=3, i=2j=4, i=1j=5, i=0j=6
    i=5j=2, i=4j=3, i=3j=4, i=2j=5, i=1j=6
    i=5j=3, i=4j=2, i=3j=1, i=2j=0

    DESC

    i=0j=3, i=1j=4, i=2j=5, i=3j=6
    i=0j=2, i=1j=3, i=2j=4, i=3j=5, i=4j=6
    i=0j=1, i=1j=2, i=2j=3, i=3j=4, i=4j=5, i=5j=6
    i=0j=0, i=1j=1, i=2j=2, i=3j=3, i=4j=4, i=5j=5, i=6j=6
    i=1j=0, i=2j=1, i=3j=2, i=4j=3, i=5j=4
    i=2j=0, i=3j=1, i=4j=2,i=5,j=3
*/

//Check Oblique (ascending & descending) for Winning
void checkO_Asc(int r, int c)
{
    for(int i = 5; i >= 0; i--)
    {
        for(int j = 0; j <= 6; j++)
        {
            if(matrix[i][j] == 1 && matrix[i-1][j+1] == 1 && matrix[i-2][j+2] == 1 && matrix[i-3][j+3] == 1)
            {
                printf("Player 1 won!");
                checkWinPlayer1 = 1;
                break;
            }
            else if(matrix[i][j] == 2 && matrix[i-1][j+1] == 2 && matrix[i-2][j+2] == 2 && matrix[i-3][j+3] == 2)
            {
                printf("Player 2 won!");
                checkWinPlayer2 = 1;
                break;
            }
        }
    }
}
void checkO_Desc(int r, int c)
{
    for(int i = 5; i >= 0; i--)
    {
        for(int j = 6; j >= 0; j--)
        {
            if(matrix[i][j] == 1 && matrix[i-1][j-1] == 1 && matrix[i-2][j-2] == 1 && matrix[i-3][j-3] == 1)
            {
                printf("Player 1 won!");
                checkWinPlayer1 = 1;
                break;
            }
            else if(matrix[i][j] == 2 && matrix[i-1][j-1] == 2 && matrix[i-2][j-2] == 2 && matrix[i-3][j-3] == 2)
            {
                printf("Player 2 won!");
                checkWinPlayer2 = 1;
                break;
            }
        }
    }
}

int main()
{
    for(int i = 0; i <= 5; i++)
    {
        for (int j = 0; j <=6; j++)
        {
            matrix[i][j] = 0;
        }
    }

    while(minute < 20 && checkWinPlayer1 == 0 && checkWinPlayer2 == 0)
    counter();
    {
    printf("Player 1, where do you want to place your chip? (Insert row,column)" );
    scanf("%d", input);
    for(int i = 0; i <= 5; i++)
    {
        if(i == input[0])
        for(int j = 0; j <= 6; j++)
        {
            if(j == input[1])
            {
                matrix[i][j] = 1;
                printMatrix(matrix[i][j]);
                checkH(i,j);
                checkV(i,j);
                checkO_Asc(i,j);
                checkO_Desc(i,j);
                if(checkWinPlayer1 == 1)
                break;
                else if(checkWinPlayer2 == 1)
                break;
            }
        }
    }
    printf("Player 2, where do you want to place your chip? (Insert row,column)" );
    scanf("%d", input);
    for(int i = 0; i <= 5; i++)
    {
        if(i == input[0])
        for(int j = 0; j <= 6; j++)
        {
            if(j == input[1])
            {
                matrix[i][j] = 2;
                printMatrix(matrix[i][j]);
                checkH(i,j);
                checkV(i,j);
                checkO_Asc(i,j);
                checkO_Desc(i,j);
                if(checkWinPlayer1 == 1)
                break;
                else if(checkWinPlayer2 == 1)
                break;
            }
        }
    }
    }
    return 0;
}
