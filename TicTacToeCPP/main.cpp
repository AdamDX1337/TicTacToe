#include <windows.h>
//#include <tchar.h>

const int GRID_SIZE = 3;
char board[GRID_SIZE][GRID_SIZE] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
bool playerTurn = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawBoard(HDC hdc);
void ResetBoard();
bool CheckWin(char player);
bool IsDraw();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = ("TicTacToe");

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, ("TicTacToe"), ("Tic-Tac-Toe Game"),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL
    );

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_LBUTTONDOWN: {
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);
        int cellWidth = 100;
        int cellHeight = 100;

        int row = yPos / cellHeight;
        int col = xPos / cellWidth;

        if (row < GRID_SIZE && col < GRID_SIZE && board[row][col] == ' ') {
            board[row][col] = playerTurn ? 'X' : 'O';
            playerTurn = !playerTurn;

            InvalidateRect(hwnd, NULL, TRUE);

            char currentPlayer = playerTurn ? 'O' : 'X';
            if (CheckWin(currentPlayer)) {
                MessageBox(hwnd, currentPlayer == 'X' ? ("X Wins!") : ("O Wins!"), ("Game Over"), MB_OK);
                ResetBoard();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (IsDraw()) {
                MessageBox(hwnd, ("It's a Draw!"), ("Game Over"), MB_OK);
                ResetBoard();
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawBoard(hdc);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void DrawBoard(HDC hdc) {
    int cellWidth = 100;
    int cellHeight = 100;

    // Draw grid lines
    for (int i = 1; i < GRID_SIZE; ++i) {
        MoveToEx(hdc, i * cellWidth, 0, NULL);
        LineTo(hdc, i * cellWidth, GRID_SIZE * cellHeight);

        MoveToEx(hdc, 0, i * cellHeight, NULL);
        LineTo(hdc, GRID_SIZE * cellHeight, i * cellHeight);
    }

    // Draw X and O
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            RECT rect = { col * cellWidth, row * cellHeight, (col + 1) * cellWidth, (row + 1) * cellHeight };
            if (board[row][col] == 'X') {
                DrawText(hdc, ("X"), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            else if (board[row][col] == 'O') {
                DrawText(hdc, ("O"), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
    }
}

void ResetBoard() {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            board[row][col] = ' ';
        }
    }
}

bool CheckWin(char player) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        // Check rows and columns
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }

    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }

    return false;
}

bool IsDraw() {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            if (board[row][col] == ' ') {
                return false;
            }
        }
    }
    return true;
}