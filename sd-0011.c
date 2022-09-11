#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

// for next version:
// working on time and tie case -> osama
// finishing the diagonal check -> Ayla
// work on pre-game: the toss and players names -> iyad

int hour=0,minute=0,second=0;

#define ROWS    6
#define COLS    7

int matrix[ROWS][COLS];

char border[] = " ----------------------------";
char guide[] = "  0   1   2   3   4   5   6  ";
int token = 1;
int selected = 0;

void add_token();

int timeCalc(int time)
{

}

void delay(int ms)
{
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

// int counter()
// {
//     while(!kbhit() && flag == 0)
//     {
//         if(minute > 59)
//         {
//             minute = 0;
//             ++hour;
//         }
//         if(second > 59)
//         {
//             second = 0;
//             ++minute;
//         }
//         ++second;
//     }
// }
int counter(){
    while(minute < 5){     //keep looping while the user didn't hit any key and flag is 0
                if(second > 59){         //after second is greater than 59, reset second and increase 1 minute
                    second = 0;++minute;
                    }
                    delay(1000);second += 1;
        }
        return 0;
}


//fill the array with empty values
void createMatrix(){
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++){
            matrix[i][j] = 0;
        }
    }
}
//print the matrix with the current results
void display(){
    printf("%s\n", border);
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++){
            printf("| %d ", matrix[i][j]);
        }
        printf("|\n");
    }
    printf("%s\n", border);
    printf("%s\n", guide);

}

// selecting which column
void selecting(){

    printf("\nChoose column: ");
    scanf(" %d", &selected);

    if(( selected <= 6 && selected >= 0)  && matrix[0][selected] == 0 ){
        add_token();
        token = (token == 1 )? 2 : 1; // flipping the token after adding it
    }
    else{
        printf("Invalid selection. Please select a column between 0 and 6");
        selecting();
        
    }
}
//adding the token to the last available row
void add_token(){
    int curRow;
    for(curRow=ROWS-1; curRow>=0; curRow--){
        if(matrix[curRow][selected] == 0){
            matrix[curRow][selected] = token;
            break;
        }
    }
}
int CheckHorizontal(int token){
    int counter;
    
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < 4; ++j) // 4 is the number of ways of connecting four tokens in one row
        {
            counter = 0;
            for (int k = 0; k < 4; ++k)// 4 in a row
            {
                if (matrix[i][j + k] == token) counter++;
            }
            if (counter == 4) return 1;
        }

    }
}

int CheckVertical(int token){
    int counter;
    
    for (int i = 0; i < COLS; ++i)
    {
        for (int j = 0; j < 3; ++j) // 4 is the number of ways of connecting four tokens in one cloumn
        {
            counter = 0;
            for (int k = 0; k < 4; ++k)// 4 in a row
            {
                if (matrix[i+ k][j] == token) counter++;
            }
            if (counter == 4) return 1;
        }
    }
}
// CheckDiagonals(token){

// }

int check(int token){
    return CheckHorizontal(token) || CheckVertical(token);
    // CheckDiagonals(token);
}

int main()
{
    createMatrix();
    display();
        while(counter()){

            printf("Player %d, your turn!\n", token);
            selecting();
            printf("\n\n");
            display();
            if (check(1))
            {
                printf("\n\nPlayer 1 wins!\n\n");
                break;  
            }
            if (check(2))
            {
                printf("\n\nPlayer 2 wins!\n\n");
                break;  
            }   
        }
        printf("Time's up!");
    return 0;
}
        

