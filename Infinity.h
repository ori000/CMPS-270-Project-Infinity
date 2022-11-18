typedef enum {
    empty = 0,
    player = 1,
    bot = 2
}State;
typedef struct
{
    int row;
    int col;
} position;
int hour = 0, minute = 0, second = 0, depthChanger = 7;;

#define ROWS 6
#define COLS 7

char players[2][64];    // Holds the players names
float timePerPlayer[2]; // stores the total time taken by each player

char border[] = " ----------------------------";
char guide[] = "  0   1   2   3   4   5   6  ";

// int token;        // Current player Turn
int selected = 0; // Current Player Column selection

void display(State board[ROWS][COLS]);
void enterNames();
State coinToss();
void selecting();
void add_token(State board[ROWS][COLS], State key);
int check();
int CheckHorizontal(State board[ROWS][COLS],State key);
int CheckVertical(State board[ROWS][COLS],State key);
int CheckDiagonals(State board[ROWS][COLS],State Key);
int tieFull(State board[ROWS][COLS]);
void tieTime();
void replaceSpaces(char p[32]);
bool boardFull(State board[ROWS][COLS]);
bool inInterval(int x, int min, int max);
void randommove(State board[ROWS][COLS]);
int max(int a, int b);
int min(int a, int b);
void legalmoves(State board[ROWS][COLS], position moves[COLS]);
void playerSelect(State board[ROWS][COLS], State key);
void print_rules();
int minmax(State arr[ROWS][COLS], int isMaximizing, int depth, int alpha, int beta);
int evaluate(State arr[ROWS][COLS]);
position findBestmove(State arr[ROWS][COLS]);
int countCol(State board[ROWS][COLS], int startRow, int startCol);
int countRow(State board[ROWS][COLS], int startRow, int startCol);
int countDiag(State board[ROWS][COLS], int startRow, int startCol);
int countDiagb(State board[ROWS][COLS], int startRow, int startCol);
int win(State board[ROWS][COLS]);
void computermove(State arr[ROWS][COLS], position move);
