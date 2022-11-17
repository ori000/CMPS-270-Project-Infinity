typedef enum {
    empty = 0,
    player = 1,
    bot = 2
}State;
int hour = 0, minute = 0, second = 0;

#define ROWS 6
#define COLS 7

char players[2][64];    // Holds the players names
float timePerPlayer[2]; // stores the total time taken by each player

char border[] = " ----------------------------";
char guide[] = "  0   1   2   3   4   5   6  ";

int token;        // Current player Turn
int selected = 0; // Current Player Column selection

void display();
void enterNames();
void coinToss();
void selecting();
void add_token(State board[ROWS][COLS]);
int check();
int CheckHorizontal(State board[ROWS][COLS],int token);
int CheckVertical(State board[ROWS][COLS],int token);
int CheckDiagonals(State board[ROWS][COLS],int token);
int tieFull();
void tieTime();
void replaceSpaces(char p[32]);