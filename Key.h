#pragma once
#include "Entity.h"

class Key : public Entity {
public:
    Key(const sf::Vector2f& pos, float size, float cellSize);
    void update(float dt) override;
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    bool isCollected() const;
    void collect();
    const sf::Vector2f& pos;
private:
    sf::RectangleShape shape;
    bool collected;

    // Assets
    sf::Texture keyTexture;
    sf::Sprite keySprite;
};
