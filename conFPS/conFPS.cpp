#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

// Initialize the variables
int sWidth = 120;
int sHeight = 40;
float playerX = 8.0f;
float playerY = 8.0f;
float playerA = 0.0f;

// Intialize map sze
int mHeight = 16;
int mWidth = 16;

// Viewpoint and depth
float fov = 3.14159 / 4.0;
float fdepth = 16.0f;

// Prototype of the map
wstring generateMap() {
    wstring map;
    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#...#.....#....#";
    map += L"#.........#....#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#...#..........#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.....##########";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";
    return map;
}

// Controls player movement, A and D changes angle and view point simulating turning left/right. W and S simulates going forward and backward by playing with x, y values, also implemented a simple collission
// where players can't move if hit wall
void handleControls(float fElapsedTime, wstring& map) {
    if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        playerA -= (0.8f) * fElapsedTime;

    if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        playerA += (0.8f) * fElapsedTime;

    if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
    {
        playerX += sinf(playerA) * 5.0f * fElapsedTime;
        playerY += cosf(playerA) * 5.0f * fElapsedTime;

        if (map[(int)playerY * mWidth + (int)playerX] == '#')
        {
            playerX -= sinf(playerA) * 5.0f * fElapsedTime;
            playerY -= cosf(playerA) * 5.0f * fElapsedTime;
        }
    }

    if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
        playerX -= sinf(playerA) * 5.0f * fElapsedTime;
        playerY -= cosf(playerA) * 5.0f * fElapsedTime;

        if (map[(int)playerY * mWidth + (int)playerX] == '#')
        {
            playerX += sinf(playerA) * 5.0f * fElapsedTime;
            playerY += cosf(playerA) * 5.0f * fElapsedTime;
        }
    }
}

// Simply recurse and render the map
void renderMap(wchar_t* screenGame, wstring& map) {
    for (int nx = 0; nx < mWidth; nx++)
        for (int ny = 0; ny < mWidth; ny++)
        {
            screenGame[(ny + 1) * sWidth + nx] = map[ny * mWidth + nx];
        }




    screenGame[((int)playerX + 1) * sWidth + (int)playerY] = 'P';
}

// Render the main game with the view of wall and walking around
void renderMain(wchar_t* screenGame, wstring& map, float fElapsedTime, HANDLE hConsole, DWORD& dwBytesWritten) {
    for (int x = 0; x < sWidth; x++) {
        float fRayAngle = (playerA - fov / 2.0f) + ((float)x / (float)sWidth) * fov;
        bool bHitWall = false;
        float fEyeX = sinf(fRayAngle);
        float fEyeY = cosf(fRayAngle);
        float fDistanceToWall = 0.1f;

        while (!bHitWall && fDistanceToWall < fdepth)
        {
            fDistanceToWall += 0.1f;

            int nTestX = (int)(playerX + fEyeX * fDistanceToWall);
            int nTestY = (int)(playerY + fEyeY * fDistanceToWall);

            if (nTestX < 0 || nTestX >= mWidth || nTestY < 0 || nTestY >= mHeight) {
                bHitWall = true;
                fDistanceToWall = fdepth;
            }
            else if (map[nTestY * mWidth + nTestX] == '#') {
                bHitWall = true;
            }
        }

        //renders the wall, adds shading based fDistanceToWall and fdepth
        int ceilingGame = (float)(sHeight / 2.0) - sHeight / ((float)fDistanceToWall);
        int floorGame = sHeight - ceilingGame;
        short shadeGame = ' ';

        if (fDistanceToWall <= fdepth / 4.0f)      shadeGame = 0x2588;
        else if (fDistanceToWall < fdepth / 3.0f)  shadeGame = 0x2593;
        else if (fDistanceToWall < fdepth / 2.0f)  shadeGame = 0x2592;
        else if (fDistanceToWall < fdepth)         shadeGame = 0x2591;
        else                                       shadeGame = ' ';

        //implements similiar logic to shading of the floor
        for (int y = 0; y < sHeight; y++) {
            if (y <= ceilingGame)
                screenGame[y * sWidth + x] = ' ';
            else if (y > ceilingGame && y <= floorGame)
                screenGame[y * sWidth + x] = shadeGame;
            else {
                float b = 1.0f - (((float)y - sHeight / 2.0f) / ((float)sHeight / 2.0f));
                if (b < 0.25)       shadeGame = '#';
                else if (b < 0.5)   shadeGame = 'x';
                else if (b < 0.75)  shadeGame = '.';
                else if (b < 0.9)   shadeGame = '-';
                else                shadeGame = ' ';
                screenGame[y * sWidth + x] = shadeGame;
            }
        }
    }

    renderMap(screenGame, map);

    screenGame[sHeight * sWidth - 1] = '\0';
    WriteConsoleOutputCharacter(hConsole, screenGame, sHeight * sWidth, { 0, 0 }, &dwBytesWritten);
}

//calls all function cohesively
int main() {
    wchar_t* screenGame = new wchar_t[sWidth * sHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    wstring map = generateMap();
    auto chr1 = chrono::system_clock::now();
    auto chr2 = chrono::system_clock::now();

    // Looping main game
    while (true) {
        chr2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = chr2 - chr1;
        chr1 = chr2;
        float fElapsedTime = elapsedTime.count();

        //handle all control and logic and render the game
        handleControls(fElapsedTime, map);
        renderMain(screenGame, map, fElapsedTime, hConsole, dwBytesWritten);
    }

    //cleanup
    delete[] screenGame;
    return 0;
}
