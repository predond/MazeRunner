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
    sf::Text options[3]; // Wznów, Zapisz grê, WyjdŸ
    int selectedIndex;
    sf::RectangleShape background; // Pó³przezroczyste t³o
};
