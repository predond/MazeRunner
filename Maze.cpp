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
    std::stack<std::pair<int, int>> stack; // Stos przechowuj¹cy wspó³rzêdne komórek do odwiedzenia

    stack.push({ x, y }); // Dodajemy pocz¹tkow¹ komórkê do stosu
    maze[x][y] = 1; // Oznaczamy pocz¹tkow¹ komórkê jako odwiedzon¹

    // Tablice okreœlaj¹ce kierunki ruchu (prawo, lewo, dó³, góra)
    const int dx[] = { 1, -1, 0, 0 };
    const int dy[] = { 0, 0, 1, -1 };

    // Pêtla dzia³a, dopóki stos nie jest pusty
    while (!stack.empty()) {
        int x = stack.top().first; // Pobieramy wspó³rzêdn¹ x obecnej komórki
        int y = stack.top().second; // Pobieramy wspó³rzêdn¹ y obecnej komórki

        // Tworzymy listê kierunków i losowo j¹ mieszamy
        std::vector<int> directions = { 0, 1, 2, 3 };
        std::random_shuffle(directions.begin(), directions.end());

        bool moved = false; // Flaga okreœlaj¹ca, czy wykonano ruch
        bool createExtraBranch = (rand() % 10 < 3); // Okreœlenie, czy utworzyæ dodatkow¹ ga³¹Ÿ

        // Przechodzimy przez ka¿dy losowy kierunek
        for (int dir : directions) {
            int nx = x + dx[dir] * 2; // Wspó³rzêdna x s¹siedniej komórki
            int ny = y + dy[dir] * 2; // Wspó³rzêdna y s¹siedniej komórki

            // Sprawdzamy, czy s¹siednia komórka jest w granicach labiryntu i nie zosta³a jeszcze odwiedzona
            if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1 && maze[ny][nx] == 0) {
                maze[y + dy[dir]][x + dx[dir]] = 1; // Tworzymy œcie¿kê miêdzy obecn¹ komórk¹ a s¹siedni¹
                maze[ny][nx] = 1; // Oznaczamy s¹siedni¹ komórkê jako odwiedzon¹

                // Jeœli zdecydowano o utworzeniu dodatkowej ga³êzi
                if (createExtraBranch && rand() % 5 == 0) {
                    int extraDir = rand() % 4; // Losujemy kierunek dla dodatkowej ga³êzi
                    int extraNx = nx + dx[extraDir]; // Wspó³rzêdna x dla dodatkowej ga³êzi
                    int extraNy = ny + dy[extraDir]; // Wspó³rzêdna y dla dodatkowej ga³êzi
                    if (extraNx > 0 && extraNy > 0 && extraNx < width - 1 && extraNy < height - 1 && maze[extraNy][extraNx] == 0) {
                        maze[ny + dy[extraDir]][nx + dx[extraDir]] = 1; // Tworzymy dodatkow¹ ga³¹Ÿ
                    }
                }

                stack.push({ nx, ny }); // Dodajemy now¹ komórkê do stosu
                moved = true; // Oznaczamy, ¿e wykonano ruch
                break; // Przerywamy pêtlê, aby kontynuowaæ z now¹ komórk¹
            }
        }

        if (!moved) {
            stack.pop(); // Jeœli nie wykonano ruchu, cofamy siê (usuwamy komórkê ze stosu)
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
