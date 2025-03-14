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
    float opacity = 255.f; // Warto�� przezroczysto�ci
    bool isTeleporting = false; // Flaga animacji teleportacji
    bool isAppearing = false;  // Flaga animacji pojawiania si�

private:
    sf::Texture bobTexture; // Tekstura dla awatara
    sf::Sprite bobSprite;   // Sprite reprezentuj�cy gracza
    sf::Texture portalTexture;
    sf::Sprite portalSprite;
    sf::CircleShape glowCircle;  // Promie� po�wiaty
    sf::Vector2f position;
    float cellSize;
    sf::SoundBuffer stepSoundBuffer;  // Bufor d�wi�ku krok�w
    sf::Sound stepSound;  // D�wi�k krok�w
    bool isPortalTriggered = false;
    bool checkCollision(const sf::Vector2f& newPosition, const std::vector<std::vector<int>>& maze) const;
};
