#include "PauseMenu.h"

PauseMenu::PauseMenu(float width, float height) : selectedIndex(0) {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error("Nie uda³o siê za³adowaæ czcionki!");
    }

    const std::wstring menuOptions[] = { L"Wznów", L"Zapisz grê", L"WyjdŸ" };
    for (int i = 0; i < 3; i++) {
        options[i].setFont(font);
        options[i].setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        options[i].setString(menuOptions[i]);
        options[i].setPosition(sf::Vector2f(width / 2 - 100, height / 2 + i * 50));
    }

    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(0, 0, 0, 150)); // Pó³przezroczyste t³o
}

void PauseMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    for (const auto& option : options) {
        window.draw(option);
    }
}

void PauseMenu::moveUp() {
    if (selectedIndex > 0) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void PauseMenu::moveDown() {
    if (selectedIndex < 2) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

int PauseMenu::getSelectedIndex() const {
    return selectedIndex;
}
