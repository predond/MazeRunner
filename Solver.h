#pragma once
#include <SFML/Graphics.hpp>
#include <set>
#include <queue>

// Operator porównania dla sf::Vector2f
namespace std {
    template <>
    struct less<sf::Vector2f> {
        bool operator()(const sf::Vector2f& a, const sf::Vector2f& b) const {
            // Porównanie najpierw wspó³rzêdnej x, potem y
            if (a.x < b.x) return true;
            if (a.x > b.x) return false;
            return a.y < b.y;
        }
    };
}

class Solver {
public:
    struct WezelSciezki {
        sf::Vector2f pozycja;
        float koszt = 0;
        float heurystyka = 0;
        float calkowityKoszt = 0;
        WezelSciezki* rodzic = nullptr;

        // Odwrócone porównanie dla priority_queue (najmniejszy koszt na wierzchu)
        bool operator<(const WezelSciezki& inny) const {
            return calkowityKoszt > inny.calkowityKoszt;
        }
    };
    std::vector<sf::Vector2f> znajdzSciezkeAStar(std::vector<std::vector<int>> maze, sf::Vector2f start, sf::Vector2f meta);
    float obliczHeurystyke(sf::Vector2f a, sf::Vector2f b);
    bool czyMoznaRuszycSie(sf::Vector2f pozycja);
    void reset();

private:
    std::vector<std::vector<int>> maze;
    std::priority_queue<WezelSciezki> otwarteLista;
    std::set<sf::Vector2f> zamknieteLista;
};