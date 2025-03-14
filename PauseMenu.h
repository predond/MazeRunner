#pragma once
#include <SFML/Graphics.hpp>

class PauseMenu {
public:
    PauseMenu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;

private:
    sf::Font font;
    sf::Text options[3]; // Wzn�w, Zapisz gr�, Wyjd�
    int selectedIndex;
    sf::RectangleShape background; // P�przezroczyste t�o
};
