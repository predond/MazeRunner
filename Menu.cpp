#include "Menu.h"

Menu::Menu(float width, float height) : selectedIndex(0) {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error("Nie udało się załadować czcionki!");
    }

    const std::wstring options[] = { L"Rozpocznij nową grę", L"Zapisz grę", L"Wczytaj grę", L"Wyjdź" };
    for (int i = 0; i < 4; i++) {
        menu[i].setFont(font);
        menu[i].setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        menu[i].setString(options[i]);
        menu[i].setPosition(sf::Vector2f(width / 2 - menu[i].getGlobalBounds().width / 2, height / 2 + i * 50));
    }
}

void Menu::draw(sf::RenderWindow& window) {
    for (const auto& item : menu) {
        window.draw(item);
    }
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        menu[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        menu[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (selectedIndex < 3) {
        menu[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        menu[selectedIndex].setFillColor(sf::Color::Red);
    }
}

int Menu::getSelectedIndex() const {
    return selectedIndex;
}
