#include <iostream>
#include <sstream> // arg parsing
#include <ctime>   // random seed
#include <chrono>  // milliseconds()
#include <thread>  // sleep_for()
#include <string>  // delimit board printout
#include <csignal> // handler for cleaning memory when user hits ctrl + c

using std::cout;
#define MAX_BOARD 50

bool __ctrl_c_capture = true;

void handler (int signal) { 
    __ctrl_c_capture = false;
};

void printBoard(char **board, int len) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (board[i][j] == 1){
                cout << '+' << ' ';
            } else cout << ' ' << ' ';
        }
        cout << '\n';
    }
}

char getLiveNeighbourCount(char **board, int len, int i, int j) {
    // Each return is laid out with respect to its actual layout 
    // in a matrix. This is simply for readability.
    
    len--; // to account for zero indexing/fix off-by-one error

    if ( i != 0 && j != 0 && i != len && j != len ) {
        return  board[i-1][j-1] + board[i][j-1] + board[i+1][j-1] +
                board[i-1][j]   +                 board[i+1][j]   +
                board[i-1][j+1] + board[i][j+1] + board[i+1][j+1];

    } else if ( i == 0 ) {
        if ( j == 0 ) { // i == 0 && j == 0
            return                  board[i+1][j] +
                    board[i][j+1] + board[i+1][j+1];
        
        } else if ( j == len ) { // i == 0 && j == len
            return  board[i][j-1] + board[i+1][j-1]
                                  + board[i+1][j];

        } else { // i == 0 && j != 0 && j != len
            return  board[i][j-1] + board[i+1][j-1] +
                                    board[i+1][j]   +
                    board[i][j+1] + board[i+1][j+1];
        }
    } else if ( i == len ) {
        if ( j == 0 ) { // i == len && j == 0
            return  board[i-1][j]   +
                    board[i-1][j+1] + board[i][j+1];

        } else if ( j == len ) { // i == len && j == len
            return  board[i-1][j-1] + board[i][j-1] +
                    board[i-1][j];

        } else { // i == len && j != 0 && j != len
            return  board[i-1][j-1] + board[i][j-1] + 
                    board[i-1][j]   +   
                    board[i-1][j+1] + board[i][j+1];
        }
    } else {
        if ( j == 0 ) { // j == 0 && i != 0 && i != len
            return  board[i-1][j]   +                 board[i+1][j]   +
                    board[i-1][j+1] + board[i][j+1] + board[i+1][j+1];

        } else if ( j == len ) { // j == len && i != 0 && i != len
            return  board[i-1][j-1] + board[i][j-1] + board[i+1][j-1] +
                    board[i-1][j]   +                 board[i+1][j];

        } else {
            return -1;
            // j != 0 && j != len && i != 0 && i != len
            // ! NOTICE: This case the same as the top case!
            // * At this time, this block will never be reached.
            // TODO: Test if performance is better there or here.
        }
    }
}

void printNeighbourCount(char **board, int len) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            cout << getLiveNeighbourCount(board, len, i, j) << ' ';
        }
        cout << '\n';
    }
}

int main( int argc, const char *argv[] ) {
    if (argc != 2) {
        cout << "Usage: ./GameOfLife <BoardSize>" << '\n';
        return EXIT_FAILURE;
    }
    
    int len = 0;
    std::istringstream ss(argv[1]);
    if (!(ss >> len)) {
        std::cerr << "Invalid number: " << argv[1] << '\n';
    } else if (!ss.eof()) {
        std::cerr << "Trailing characters after number: " << argv[1] << '\n';
    }

    if ( len > MAX_BOARD ) {
        cout << "Board size too big for computation... enter a size <" << MAX_BOARD << '\n';
        return EXIT_FAILURE;
    }

    // * Game Programming * //

    // Declare and Initialise Board

    enum State {
        Dead,
        Alive
    };
    
    char **board = new char *[len];
    for (int i = 0; i < len; i++)
        board[i] = new char[len];

    srand(time(NULL));
    int prob = rand() % 100;

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (prob < 10) { // 10% chance of being Alive gives us a sparse matrix
                board[i][j] = Alive;
            } else board[i][j] = Dead;
            prob = rand() % 100;
        }
    }
    // Game Loop
    
    signal(SIGINT, handler); // to handle memory cleanup when user terminates infinite loop w/ ctrl + c
    
    while (__ctrl_c_capture) {
    // for (size_t i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        printBoard(board, len);

        char numLiveNeighbours;

        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                numLiveNeighbours = getLiveNeighbourCount(board, len, i, j);
                State isAlive = (State)board[i][j];

                if ( numLiveNeighbours == 3 ) {
                    if ( !isAlive ) {
                        board[i][j] = Alive;
                    } // else {
                        // cell is already alive and remains alive
                 // }
                } else if ( isAlive && numLiveNeighbours == 2 ) {
                    board[i][j] = Alive;
                } else if ( isAlive ) {
                    board[i][j] = Dead;
                } // else {
                    // cell is already dead and remains dead
             // } 
            }
        }

        cout << '\n' << std::string((2 * len), '-') << "\n\n";
    }
    
    for (int i = 0; i < len; i++)
        delete [] board[i];
    delete [] board;

    return EXIT_SUCCESS;
}