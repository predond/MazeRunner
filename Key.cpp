#include "Key.h"
#include <iostream>

Key::Key(const sf::Vector2f& pos, float size, float cellSize)
    : collected(false), pos(pos) {
    if (!keyTexture.loadFromFile("assets/key.png")) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstury dla klucza!" << std::endl;
    }
    keySprite.setTexture(keyTexture);
    keySprite.setColor(sf::Color(255, 255, 200, 255));
    keySprite.setOrigin(keyTexture.getSize().x / 2, keyTexture.getSize().y / 2);
    keySprite.setPosition(pos.x + cellSize / 2, pos.y + cellSize / 2);
    keySprite.setScale(cellSize / keyTexture.getSize().x * 0.7, cellSize / keyTexture.getSize().y * 0.7);
    
    //shape.setSize(sf::Vector2f(size, size));
    //shape.setFillColor(sf::Color::Blue);
}

void Key::update(float dt) {
}

sf::Vector2f Key::getPosition() {
    return keySprite.getPosition();
}

void Key::draw(sf::RenderWindow& window) {
    if (!collected) 
        window.draw(keySprite);
}

sf::FloatRect Key::getBounds() const {
    return keySprite.getGlobalBounds();
}

bool Key::isCollected() const {
    return collected;
}

void Key::collect() {
    collected = true;
}



