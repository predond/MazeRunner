#include "Door.h"

Door::Door(const sf::Vector2f& pos, const sf::Vector2f& size)
    : locked(true), pos(pos) {
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Red); // pocz¹tkowo czerwone
}

void Door::update(float dt) {
    // Drzwi nie posiadaj¹ animacji – nic nie robimy
}

sf::Vector2f Door::getPosition() {
    return shape.getPosition();
}

void Door::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Door::getBounds() const {
    return shape.getGlobalBounds();
}

void Door::setProps(const sf::Vector2f& pos, const sf::Vector2f& size) {
    Door(pos, size);
}

void Door::unlock() {
    locked = false;
    shape.setFillColor(sf::Color::Green); // po odblokowaniu – zielone
}

bool Door::isLocked() const {
    return locked;
}
