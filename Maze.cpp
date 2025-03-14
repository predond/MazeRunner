#include "Maze.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stack>

Maze::Maze(int width, int height) : width(width), height(height) {
    maze.resize(height, std::vector<int>(width, 0));
}

void Maze::generate() {
    Maze(width, height);
    std::srand(std::time(nullptr));
    generateDFS(1, 1);
}

    // generatePrim();
void Maze::generateDFS(int x, int y) {
    std::stack<std::pair<int, int>> stack; // Stos przechowuj�cy wsp�rz�dne kom�rek do odwiedzenia

    stack.push({ x, y }); // Dodajemy pocz�tkow� kom�rk� do stosu
    maze[x][y] = 1; // Oznaczamy pocz�tkow� kom�rk� jako odwiedzon�

    // Tablice okre�laj�ce kierunki ruchu (prawo, lewo, d�, g�ra)
    const int dx[] = { 1, -1, 0, 0 };
    const int dy[] = { 0, 0, 1, -1 };

    // P�tla dzia�a, dop�ki stos nie jest pusty
    while (!stack.empty()) {
        int x = stack.top().first; // Pobieramy wsp�rz�dn� x obecnej kom�rki
        int y = stack.top().second; // Pobieramy wsp�rz�dn� y obecnej kom�rki

        // Tworzymy list� kierunk�w i losowo j� mieszamy
        std::vector<int> directions = { 0, 1, 2, 3 };
        std::random_shuffle(directions.begin(), directions.end());

        bool moved = false; // Flaga okre�laj�ca, czy wykonano ruch
        bool createExtraBranch = (rand() % 10 < 3); // Okre�lenie, czy utworzy� dodatkow� ga���

        // Przechodzimy przez ka�dy losowy kierunek
        for (int dir : directions) {
            int nx = x + dx[dir] * 2; // Wsp�rz�dna x s�siedniej kom�rki
            int ny = y + dy[dir] * 2; // Wsp�rz�dna y s�siedniej kom�rki

            // Sprawdzamy, czy s�siednia kom�rka jest w granicach labiryntu i nie zosta�a jeszcze odwiedzona
            if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1 && maze[ny][nx] == 0) {
                maze[y + dy[dir]][x + dx[dir]] = 1; // Tworzymy �cie�k� mi�dzy obecn� kom�rk� a s�siedni�
                maze[ny][nx] = 1; // Oznaczamy s�siedni� kom�rk� jako odwiedzon�

                // Je�li zdecydowano o utworzeniu dodatkowej ga��zi
                if (createExtraBranch && rand() % 5 == 0) {
                    int extraDir = rand() % 4; // Losujemy kierunek dla dodatkowej ga��zi
                    int extraNx = nx + dx[extraDir]; // Wsp�rz�dna x dla dodatkowej ga��zi
                    int extraNy = ny + dy[extraDir]; // Wsp�rz�dna y dla dodatkowej ga��zi
                    if (extraNx > 0 && extraNy > 0 && extraNx < width - 1 && extraNy < height - 1 && maze[extraNy][extraNx] == 0) {
                        maze[ny + dy[extraDir]][nx + dx[extraDir]] = 1; // Tworzymy dodatkow� ga���
                    }
                }

                stack.push({ nx, ny }); // Dodajemy now� kom�rk� do stosu
                moved = true; // Oznaczamy, �e wykonano ruch
                break; // Przerywamy p�tl�, aby kontynuowa� z now� kom�rk�
            }
        }

        if (!moved) {
            stack.pop(); // Je�li nie wykonano ruchu, cofamy si� (usuwamy kom�rk� ze stosu)
        }
    }
}


void Maze::generatePrim() {
    // Initialize maze with all walls
    maze = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));

    // Starting point (ensure odd coordinates for grid structure)
    int startX = 1, startY = 1;
    maze[startY][startX] = 1;

    // Walls to consider
    std::vector<std::pair<int, int>> walls;
    const int dx[] = { 1, -1, 0, 0 };
    const int dy[] = { 0, 0, 1, -1 };

    // Add initial walls
    for (int i = 0; i < 4; i++) {
        int wx = startX + dx[i] * 2;
        int wy = startY + dy[i] * 2;
        if (wx > 0 && wy > 0 && wx < width - 1 && wy < height - 1) {
            walls.push_back({ wx, wy });
        }
    }

    while (!walls.empty()) {
        // Randomly select a wall
        int wallIndex = rand() % walls.size();
        int x = walls[wallIndex].first;
        int y = walls[wallIndex].second;

        // Remove wall from consideration
        walls.erase(walls.begin() + wallIndex);

        // Find adjacent cells
        int visitedCells = 0;
        int unvisitedX = -1, unvisitedY = -1;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i] * 2;
            int ny = y + dy[i] * 2;

            // Check if cell is within bounds
            if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1) {
                if (maze[ny][nx] == 1) visitedCells++;
                else {
                    unvisitedX = nx;
                    unvisitedY = ny;
                }
            }
        }

        // If only one cell is visited, connect the unvisited cell
        if (visitedCells == 1) {
            // Carve a path through the wall
            maze[y][x] = 1;
            maze[unvisitedY][unvisitedX] = 1;

            // Add new walls
            for (int i = 0; i < 4; i++) {
                int wx = unvisitedX + dx[i] * 2;
                int wy = unvisitedY + dy[i] * 2;

                if (wx > 0 && wy > 0 && wx < width - 1 && wy < height - 1 &&
                    maze[wy][wx] == 0) {
                    walls.push_back({ wx, wy });
                }
            }
        }
    }
}

const std::vector<std::vector<int>>& Maze::getMaze() const {
    return maze;
}
