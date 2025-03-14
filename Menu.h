#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;

private:
    sf::Font font;
    sf::Text menu[4]; // Rozpocznij, Zapisz, Wczytaj, Wyjdź
    int selectedIndex;
};
