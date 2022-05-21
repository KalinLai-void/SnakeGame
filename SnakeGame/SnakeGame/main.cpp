#include "ImportAudio.h"
#include "Audio.h"
#include "Game.h"
#include <iostream>

#define ESC 27

DWORD WINAPI BGM_Play(LPVOID lpParameter)
{
    while (true)
    {
        ((Audio*)lpParameter)->Play();
    }
    return 0;
}

void GameOver(HANDLE* bgmHandle, Audio* bgm)
{
    std::cout << std::endl << std::endl << std::endl << "GAME OVER!" << std::endl;

    if (*bgmHandle)
    {
        CloseHandle(*bgmHandle);
        bgm->End();
    }

    // Game Over
    Audio gameover_audio(GAMEOVER_AUDIO_FILE);
    gameover_audio.Play();
}

int main(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15); // white
    std::cout << "Press any key to start!" << std::endl;

Start:
    Game game(&hConsole);
    Audio bgm(BGM_FILE);

    HANDLE bgmHandle = NULL;

    bool isStarted = false; // avoid enter twice
    while (!game.isGameOver)
    {
        if (game.isStarted && !isStarted)
        { // waiting for starting to play BGM
            system("cls");
            game.DrawBorder();
            bgmHandle = CreateThread(0, 0, BGM_Play, &bgm, 0, 0);
            isStarted = true;
        }

        game.UpdateMap();
        game.GetInput();
        game.Logic();
        Sleep(game.speed);
    }
    
    GameOver(&bgmHandle, &bgm);
    std::cout << "Press any key to restart. (press Esc to exit.)" << std::endl;
    char input;
    while (input = _getch()) // waiting for input
        if (input) break;

    if (input == ESC) return 0; // get ESC key, exit program
    else goto Start; // restart
}