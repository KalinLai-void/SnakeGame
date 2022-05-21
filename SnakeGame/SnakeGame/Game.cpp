#include "Game.h"
#include "ImportAudio.h"
#include "string"

DWORD WINAPI CoinAudio_Play(LPVOID lpParameter)
{
    Audio coinAudio(COIN_AUDIO);
    coinAudio.Play();
    coinAudio.End();
    return 0;
}

Game::Game(HANDLE* hConsole)
{
    this->hConsole = *hConsole;
    Setup();
}

void Game::Setup()
{
    isGameOver = false;
    dir = STOP;

    srand(time(NULL));
    x = rand() % (WIDTH - 1) + 1;
    y = rand() % (HEIGHT - 1) + 1;

    do
    {
        coinX = rand() % (WIDTH - 1) + 1;
        coinY = rand() % (HEIGHT - 1) + 1;
    } while (coinX == x && coinY == y); // avoid coin's position as same as snake's position

    score = 0;
    speed = INIT_SPEED;    
}

void Game::DrawBorder()
{
    SetConsoleTextAttribute(hConsole, 15); // white
    for (int i = 0; i < WIDTH + 2; i++)  std::cout << "#"; // top border
    std::cout << " Score: " << score << std::endl;

    for (int i = 0; i < HEIGHT - 1; i++)
    {
        for (int j = 0; j < WIDTH + 2; j++)
        {
            if (j == 0) // left border
            {
                std::cout << "#";
                continue;
            }

            if (j == WIDTH + 1) std::cout << "#"; // right border
            else std::cout << " ";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < WIDTH + 2; i++) std::cout << "#"; // bottom border
}

void Game::UpdateMap()
{
    if (!isStarted) return;

    for (int i = 1; i < HEIGHT; i++)
    {
        for (int j = 1; j < WIDTH; j++)
        {
            COORD coord = { (short)j, (short)i };
            SetConsoleCursorPosition(hConsole, coord);
            std::wstring ch;

            if (i == y && j == x) // snake
            {
                SetConsoleTextAttribute(hConsole, 13); // purple
                ch = L"O";
            }
            else if (i == coinY && j == coinX) // coin
            {
                SetConsoleTextAttribute(hConsole, 12); // red
                ch = L"x";
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 13); // purple
                bool isPrinted = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        ch = L"o";
                        isPrinted = true;
                    }
                }
                if (!isPrinted) ch = L" ";
            }

            WriteConsole(hConsole, ch.c_str(), ch.size(), NULL, NULL);
        }
    }
    SetConsoleTextAttribute(hConsole, 15); // white
}

void Game::GetInput()
{
    if (_kbhit()) // record recent keystroke
    {
        switch (_getch())
        {
        case 72: dir = UP; break;
        case 80: dir = DOWN; break;
        case 75: dir = LEFT; break;
        case 77: dir = RIGHT; break;
        case 27: isGameOver = true; break;
        }
        isStarted = true;
    }
}

void Game::Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    }

    if (IS_GAMEOVER_WHEN_TOUCH_BOARD)
    {
        if (x > WIDTH || x < 0 || y > HEIGHT || y < 0)
            isGameOver = true;
    }
    else
    {
        if (x >= WIDTH) x = 0; else if (x <= 0) x = WIDTH - 1;
        if (y >= HEIGHT) y = 0; else if (y <= 0) y = HEIGHT - 1;
    }

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            isGameOver = true;

    if (x == coinX && y == coinY) // eat coin
    {
        HANDLE coinAudioHandle = CreateThread(0, 0, CoinAudio_Play, 0, 0, 0);
        score += 10;
        UpdateScoreText();
        coinX = rand() % (WIDTH - 1) + 1;
        coinY = rand() % (HEIGHT - 1) + 1;
        nTail++;
        speed -= DEC_SPEED_PER_SCORE;
        speed = (speed <= 0 ? 0 : speed);
        CloseHandle(coinAudioHandle);
    }
}

void Game::UpdateScoreText() 
{
    COORD coord = { (short)(WIDTH + 10), (short)0 };
    SetConsoleCursorPosition(hConsole, coord);
    SetConsoleTextAttribute(hConsole, 15); // white
    std::wstring str = std::to_wstring(score);
    WriteConsole(hConsole, str.c_str(), str.size(), NULL, NULL);
}