#pragma once
#include "Entity.h"

class Door : public Entity {
public:
    Door(const sf::Vector2f& pos, const sf::Vector2f& size);
    void update(float dt) override;
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void setProps(const sf::Vector2f& pos, const sf::Vector2f& size);
    void unlock();
    bool isLocked() const;
    const sf::Vector2f& pos;
private:
    sf::RectangleShape shape;
    bool locked;
};
