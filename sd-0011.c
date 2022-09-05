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

void delay(ms)
{
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
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

int main()
{
    int input[2];
    int matrix[5][6];
    for(int i = 0; i <= 5; i++)
    {
        for (int j = 0; j <=6; j++)
        {
            matrix[i][j] = 0;
        }
    }

    while(minute < 20)
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
                matrix[i][j] = 1;
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
                matrix[i][j] = 2;
        }
    }
    }
    return 0;
}