#include<iostream>
#include<string>
#include<cstdlib>
#include<Windows.h>

using namespace std;

enum enGameChoice { x = 1, o = 2 };
enum enWinner { player = 1, computer = 2, draw = 3 };
enum enMove {
    pos1 = 1, pos2 = 2, pos3 = 3, pos4 = 4, pos5 = 5,
    pos6 = 6, pos7 = 7, pos8 = 8, pos9 = 9
};

struct stGame {
    char board[3][3];
    enGameChoice playerChoice;
    enGameChoice computerChoice;
    enMove playerMove;
    enMove computerMove;
    enWinner winner;
    bool isDraw = false;
    bool isGameOver = false;
};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// ====================== INPUT VALIDATION ======================
int readIntInRange(int min, int max, const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(10000, '\n'); 
            return value;
        }
        else {
            cin.clear();
            cin.ignore(10000, '\n'); 
            setColor(4);
            cout << "\t\t\t\tInvalid input! Please enter a number between "
                << min << " and " << max << ".\n";
            setColor(6);
        }
    }
}


// ====================== GAME LOGIC ======================
enGameChoice readPlayerChoice() {
    setColor(6);
    int choice = readIntInRange(1, 2, "\n\t\t\t\tPLZ, Enter your choice? [1-2]? ");
    return (enGameChoice)choice;
}

enGameChoice getComputerChoice(const stGame& game) {
    return (game.playerChoice == enGameChoice::x) ? enGameChoice::o : enGameChoice::x;
}

void initializeBoard(stGame& game) {
    char counter = '1';
    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            game.board[i][j] = counter++;
        }
    }
}

int randomNumber(int from, int to) {
    return rand() % (to - from + 1) + from;
}

void getRowCol(enMove move, short& row, short& col) {
    short index = move - 1;
    row = index / 3;
    col = index % 3;
}

bool isBoardFull(const stGame& game) {
    for (short i = 0; i < 3; i++)
        for (short j = 0; j < 3; j++)
            if (game.board[i][j] != 'X' && game.board[i][j] != 'O')
                return false;
    return true;
}

bool checkWinner(const stGame& game, char symbol) {
    for (int i = 0; i < 3; i++) {
        if ((game.board[i][0] == symbol && game.board[i][1] == symbol && game.board[i][2] == symbol) ||
            (game.board[0][i] == symbol && game.board[1][i] == symbol && game.board[2][i] == symbol))
            return true;
    }
    if ((game.board[0][0] == symbol && game.board[1][1] == symbol && game.board[2][2] == symbol) ||
        (game.board[0][2] == symbol && game.board[1][1] == symbol && game.board[2][0] == symbol))
        return true;

    return false;
}

void updateGameStatus(stGame& game) {
    if (checkWinner(game, game.playerChoice == x ? 'X' : 'O')) {
        game.winner = player;
        game.isGameOver = true;
        return;
    }
    if (checkWinner(game, game.computerChoice == x ? 'X' : 'O')) {
        game.winner = computer;
        game.isGameOver = true;
        return;
    }
    if (isBoardFull(game)) {
        game.winner = draw;
        game.isDraw = true;
        game.isGameOver = true;
    }
}

// ====================== PLAYER & COMPUTER MOVES ======================
enMove readPlayerMove() {
    setColor(6);
    int move = readIntInRange(1, 9, "\n\t\t\t\tPLZ, Enter position you want to play? [1-9]? ");
    return (enMove)move;
}

void playerTurn(stGame& game) {
    short row, col;
    do {
        game.playerMove = readPlayerMove();
        getRowCol(game.playerMove, row, col);
        if (game.board[row][col] == 'X' || game.board[row][col] == 'O') {
            setColor(4);
            cout << "\t\t\t\tThis cell is already occupied! Try another one\n";
            setColor(6);
        }
    } while (game.board[row][col] == 'X' || game.board[row][col] == 'O');
    game.board[row][col] = (game.playerChoice == enGameChoice::x) ? 'X' : 'O';
}

enMove getSmartComputerMove(stGame& game) {
    char computerSymbol = (game.computerChoice == enGameChoice::x) ? 'X' : 'O';
    char playerSymbol = (game.playerChoice == enGameChoice::x) ? 'X' : 'O';

    // 1. try to won
    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            if (game.board[i][j] != 'X' && game.board[i][j] != 'O') {
                char backup = game.board[i][j];
                game.board[i][j] = computerSymbol;
                if (checkWinner(game, computerSymbol)) {
                    game.board[i][j] = backup; 
                    return (enMove)(i * 3 + j + 1);
                }
                game.board[i][j] = backup;
            }
        }
    }

    // 2. prevent player to won
    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            if (game.board[i][j] != 'X' && game.board[i][j] != 'O') {
                char backup = game.board[i][j];
                game.board[i][j] = playerSymbol;
                if (checkWinner(game, playerSymbol)) {
                    game.board[i][j] = backup;
                    return (enMove)(i * 3 + j + 1);
                }
                game.board[i][j] = backup;
            }
        }
    }

    // 3.Random Choice
    short row, col;
    enMove move;
    do {
        move = (enMove)randomNumber(1, 9);
        getRowCol(move, row, col);
    } while (game.board[row][col] == 'X' || game.board[row][col] == 'O');

    return move;
}

// ====================== PRINT / VIEW ======================
void printStartScreen() {
    cout << "\n\n\n\n";
    setColor(5);
    cout << "\t\t\t\t=================================\n";
    setColor(4);
    cout << "\t\t\t\t           START GAME\n";
    setColor(5);
    cout << "\t\t\t\t=================================\n";
    setColor(2);
    cout << "\t\t\t\t\t1- Playing by (X)\n";
    cout << "\t\t\t\t\t2- Playing by (O)\n";
    setColor(5);
    cout << "\t\t\t\t=================================\n";
}

void printBoard(const stGame& game) {
    setColor(5);
    cout << "\n\t\t\t\tBoard:\n";
    cout << "\t\t\t\t-------------\n";
    for (short i = 0; i < 3; i++) {
        cout << "\t\t\t\t| ";
        for (short j = 0; j < 3; j++) {
            char cell = game.board[i][j];
            if (cell == 'X') setColor(2);
            else if (cell == 'O') setColor(4);
            else setColor(7);
            cout << cell;
            setColor(5);
            cout << " | ";
        }
        cout << "\n\t\t\t\t-------------\n";
    }
}

void printRoundInfo(const stGame& game) {
    setColor(3);
    cout << "\n\t\t\t\t========================================================";
    cout << "\n\t\t\t\tPlayer choose position: " << game.playerMove;
    cout << " | Computer choose position: " << game.computerMove;
    cout << "\n\t\t\t\t========================================================\n";
}

void setWinnerScreenColor(enWinner winner) {
    switch (winner) {
    case enWinner::player:
        system("color 2F"); 
        break;
    case enWinner::computer:
        system("color 4F"); 
        break;
    case enWinner::draw:
        system("color 6F"); 
        break;
    }
}

void playResultSound(enWinner winner) {
    switch (winner) {
    case player:       
        Beep(1000, 300);
        Beep(1200, 300);
        break;
    case computer:     
        Beep(500, 400);
        break;
    case draw:        
        Beep(800, 200);
        Beep(800, 200);
        break;
    }
}

void printResultScreen(const stGame& game) {
    system("cls");

    cout << "\n\n\n\n\n\n\t\t\t\t===================================================\n";
    cout << "\t\t\t\t\t\t  [Game Result]\n";
    cout << "\t\t\t\t===================================================\n";
    setWinnerScreenColor(game.winner);
    playResultSound(game.winner);

    if (game.winner == player) cout << "\t\t\t\t\t    Congratulations! Player Won! \n\t\t\t\t\t+++++++++++++++++++++++++++++++++++\n";
    else if (game.winner == computer) cout << "\t\t\t\t\t  Computer Won! Better Luck Next Time \n\t\t\t\t\t+++++++++++++++++++++++++++++++++++++++\n";
    else cout << "\t\t\t\t\t\t   It's a Draw! \n\t\t\t\t\t+++++++++++++++++++++++++++++++++++\n";

    cout << "\t\t\t\tPlayer choose: " << (game.playerChoice == x ? 'X' : 'O') << endl;
    cout << "\t\t\t\tComputer choose: " << (game.computerChoice == x ? 'X' : 'O') << endl;

    cout << "\t\t\t\tWinner name: ";
    if (game.winner == player) cout << "Player!" << endl;
    else if (game.winner == computer) cout << "Computer!" << endl;
    else cout << "Draw!" << endl;

    cout << "\t\t\t\t===================================================\n";
}

void goContinueToShowFinalResult() {
    setColor(8);
    cout << "\n\n\t\t\t\tPress any key to show final result ...";
    system("pause>0");
}

bool askPlayAgain() {
    char choice;
    setColor(6);
    cout << "\n\t\t\t\tDo you want to play again? (Y/N): ";
    cin >> choice;
    cin.ignore(10000, '\n');
    return (choice == 'Y' || choice == 'y');
}

// ====================== CONTROLLER ======================
void playGame(stGame& game) {
    initializeBoard(game);
    game.isGameOver = false;

    while (!game.isGameOver) {
        printBoard(game);
        //player role
        playerTurn(game);
        updateGameStatus(game);
        if (game.isGameOver) break;

        //computer role
        game.computerMove = getSmartComputerMove(game);
        short row, col;
        getRowCol(game.computerMove, row, col);
        game.board[row][col] = (game.computerChoice == enGameChoice::x) ? 'X' : 'O';
        updateGameStatus(game);
        if (game.isGameOver) break;

        printRoundInfo(game);
    }
}

void startGame() {
    stGame game;
    bool playAgain = true;

    while (playAgain) {
        system("cls");
        printStartScreen();
        game.playerChoice = readPlayerChoice();
        game.computerChoice = getComputerChoice(game);

        system("cls");
        playGame(game);
        printBoard(game);
        goContinueToShowFinalResult();
        printResultScreen(game);

        playAgain = askPlayAgain();
    }
}


int main() {

    srand((unsigned)time(NULL));
    startGame();
    system("pause>0");

    return 0;
}
