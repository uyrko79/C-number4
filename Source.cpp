#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
using namespace std;

void clearScreen()
{
    cout << "\033[2J\033[1;1H" << endl;
}

void delay(int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void playMatchSound()
{
    cout << "\a";
    delay(500);
}

void playMismatchSound()
{
    cout << "\a";
    delay(1000);
}

class MemoryGame
{
private:
    int gridSize;
    vector<char> grid;
    vector<bool> flipped;
    int numFlipped;
    int numMatches;
    int numTurns;
    chrono::steady_clock::time_point startTime;

public:
    MemoryGame(int size) : gridSize(size), numFlipped(0), numMatches(0), numTurns(0)
    {
        grid.resize(gridSize);
        flipped.resize(gridSize, false);
        startTime = chrono::steady_clock::now();
    }

    void initializeGrid()
    {
        vector<char> values;
        for (int i = 0; i < gridSize / 2; ++i)
        {
            values.push_back('A' + i);
            values.push_back('A' + i);
        }

        srand(time(0));
        random_shuffle(values.begin(), values.end());

        for (int i = 0; i < gridSize; ++i)
        {
            grid[i] = values[i];
        }
    }

    void displayGrid()
    {
        clearScreen();
        for (int i = 0; i < gridSize; ++i)
        {
            if (flipped[i])
            {
                cout << grid[i] << " ";
            }
            else
            {
                cout << "* ";
            }

            if ((i + 1) % 4 == 0)
            {
                cout << endl;
            }
        }
    }

    void flipCard(int index)
    {
        if (!flipped[index])
        {
            flipped[index] = true;
            ++numFlipped;
        }
    }

    void hideFlippedCards() {
        for (int i = 0; i < gridSize; ++i)
        {
            if (flipped[i])
            {
                flipped[i] = false;
                --numFlipped;
            }
        }
    }

    void checkMatch() {
        vector<int> flippedIndices;
        for (int i = 0; i < gridSize; ++i)
        {
            if (flipped[i])
            {
                flippedIndices.push_back(i);
            }
        }

        if (flippedIndices.size() == 2) {
            ++numTurns;
            if (grid[flippedIndices[0]] == grid[flippedIndices[1]]) {
                playMatchSound();
                ++numMatches;
                flipped[flippedIndices[0]] = false;
                flipped[flippedIndices[1]] = false;
                numFlipped -= 2;
            }
            else {
                playMismatchSound();
                delay(1000);
                hideFlippedCards();
            }
        }
    }

    bool isGameOver()
    {
        return numMatches == gridSize / 2;
    }

    void getStats()
    {
        setlocale(LC_ALL, "rus");
        chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
        chrono::duration<double> duration = chrono::duration_cast<chrono::duration<double>>(endTime - startTime);

        cout << endl;
        cout << "======== ���� ��������! ========" << endl;
        cout << "�����: " << duration.count() << " seconds" << endl;
        cout << "���������� �����: " << numTurns << endl;
        cout << "============================" << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");
    int gridSize;
    cout << "������� ������ ����� (������ ����� �� 4 �� 16): " << endl;
    cin >> gridSize;

    if (gridSize < 4 || gridSize > 16 || gridSize % 2 != 0)
    {
        cout << "�������� ������ �����. �����..." << endl;
        return 0;
    }

    MemoryGame game(gridSize);
    game.initializeGrid();

    while (!game.isGameOver())
    {
        game.displayGrid();

        int index;
        cout << "������� ����� �����, ������� ����� ����������� (�� 0 ��" << gridSize - 1 << "): ";
        cin >> index;

        if (index < 0 || index >= gridSize)
        {
            cout << "�������� ������. ���������� ��� ���." << endl;
            continue;
        }

        game.flipCard(index);
        game.checkMatch();
    }

    game.displayGrid();
    game.getStats();

    return 0;
}