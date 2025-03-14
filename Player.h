#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Player {
public:
    Player(int x, int y, float size);
    void move(const sf::Vector2f& direction, const std::vector<std::vector<int>>& maze);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    void updateCellSize(float newSize);
    void appearPlayer();
    friend void breakSound(Player& player);
    float opacity = 255.f; // Wartoœæ przezroczystoœci
    bool isTeleporting = false; // Flaga animacji teleportacji
    bool isAppearing = false;  // Flaga animacji pojawiania siê

private:
    sf::Texture bobTexture; // Tekstura dla awatara
    sf::Sprite bobSprite;   // Sprite reprezentuj¹cy gracza
    sf::Texture portalTexture;
    sf::Sprite portalSprite;
    sf::CircleShape glowCircle;  // Promieñ poœwiaty
    sf::Vector2f position;
    float cellSize;
    sf::SoundBuffer stepSoundBuffer;  // Bufor dŸwiêku kroków
    sf::Sound stepSound;  // DŸwiêk kroków
    bool isPortalTriggered = false;
    bool checkCollision(const sf::Vector2f& newPosition, const std::vector<std::vector<int>>& maze) const;
};
