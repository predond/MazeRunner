#include "Solver.h"
#include <vector>
#include <queue>
#include <set>
#include <iostream>

// Metoda g��wna solvera A*
std::vector<sf::Vector2f> Solver::znajdzSciezkeAStar(std::vector<std::vector<int>> maze, sf::Vector2f start, sf::Vector2f meta) {
    this->maze = maze;
    
    std::cout << "X: " << start.x << ", Y: " << start.y << std::endl;
    sf::Vector2f startPos = { (float)((int)start.x), (float)((int)start.y) };
    std::cout << "X: " << startPos.x << ", Y: " << startPos.y << std::endl;

    std::vector<sf::Vector2f> kierunki = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    WezelSciezki startowy;
    startowy.pozycja = startPos;
    otwarteLista.push(startowy);

    while (!otwarteLista.empty()) {
        WezelSciezki aktualny = otwarteLista.top();
        otwarteLista.pop();

        // Dotarcie do celu
        if (aktualny.pozycja.x == meta.x && aktualny.pozycja.y == meta.y) {
            std::vector<sf::Vector2f> sciezka;
            while (aktualny.rodzic != nullptr) {
                sciezka.push_back(aktualny.pozycja);
                aktualny = *aktualny.rodzic;
            }
            std::reverse(sciezka.begin(), sciezka.end());
            return sciezka;
        }

        zamknieteLista.insert(aktualny.pozycja);

        // Sprawdzanie s�siednich kom�rek
        for (const auto& kierunek : kierunki) {
            sf::Vector2f sasiad = aktualny.pozycja + kierunek;

            // Sprawdzenie poprawno�ci ruchu
            if (!czyMoznaRuszycSie(sasiad)) continue;
            if (zamknieteLista.find(sasiad) != zamknieteLista.end()) continue;

            WezelSciezki sasiedniWezel;
            sasiedniWezel.pozycja = sasiad;
            sasiedniWezel.koszt = aktualny.koszt + 1;
            sasiedniWezel.heurystyka = obliczHeurystyke(sasiad, meta);
            sasiedniWezel.calkowityKoszt = sasiedniWezel.koszt + sasiedniWezel.heurystyka;
            sasiedniWezel.rodzic = new WezelSciezki(aktualny);

            otwarteLista.push(sasiedniWezel);
        }
    }

    return {}; // Brak �cie�ki
}

// Funkcja heurystyczna - odleg�o�� Manhattan
float Solver::obliczHeurystyke(sf::Vector2f a, sf::Vector2f b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Sprawdzenie, czy mo�liwy jest ruch
bool Solver::czyMoznaRuszycSie(sf::Vector2f pozycja) {
    int x = static_cast<int>(pozycja.x);
    int y = static_cast<int>(pozycja.y);

    // Sprawdzenie granic labiryntu i wolnych kom�rek
    if (x < 0 || y < 0 || x >= maze[0].size() || y >= maze.size())
        return false;

    return maze[y][x] == 1;
}

void Solver::reset() {
    while (!otwarteLista.empty()) otwarteLista.pop();
    zamknieteLista.clear();
    maze.clear();
}