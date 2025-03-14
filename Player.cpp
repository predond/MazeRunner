#include "Player.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "Game.h"

Player::Player(int x, int y, float size)
    : position(x* size + size / 2, y* size + size / 2), cellSize(size), glowCircle(30) {
    
    // Wczytaj teksturê Boba
    if (!bobTexture.loadFromFile("assets/bob.png")) {
        throw std::runtime_error("Nie uda³o siê wczytaæ tekstury Boba!");
    }

    // Wczytaj teksturê portalu
    if (!portalTexture.loadFromFile("assets/portal.png")) {
        throw std::runtime_error("Nie uda³o siê wczytaæ tekstury portalu!");
    }

    // Ustaw sprite'y
    // Gracz Bob
    bobSprite.setTexture(bobTexture);
    bobSprite.setColor(sf::Color(255, 255, 200, 255));
    bobSprite.setOrigin(bobTexture.getSize().x / 2, bobTexture.getSize().y / 2);
    bobSprite.setPosition(position);
    bobSprite.setScale((size / bobTexture.getSize().x) * 0.8, (size / bobTexture.getSize().y) * 0.8);
    
    // Poœwiata
    glowCircle.setOrigin(glowCircle.getRadius(), glowCircle.getRadius());    // Ustawienie œrodka
    glowCircle.setFillColor(sf::Color(255, 255, 0, 75)); // Pó³przezroczysty ¿ó³ty

    // Portal
    portalSprite.setTexture(portalTexture);
    portalSprite.setOrigin(portalTexture.getSize().x / 2, portalTexture.getSize().y / 2);
    portalSprite.setScale(cellSize / portalTexture.getSize().x, (cellSize / portalTexture.getSize().y) / 2.5f);
    portalSprite.setPosition(position); // Na pocz¹tek pod nogami gracza
    portalSprite.setColor(sf::Color(255, 255, 255, 0)); // Niewidoczny

    // Wczytaj dŸwiêk kroków
    if (!stepSoundBuffer.loadFromFile("assets/step.mp3")) {
        throw std::runtime_error("Nie uda³o siê wczytaæ dŸwiêku kroków!");
    }

    stepSound.setBuffer(stepSoundBuffer);
    stepSound.setVolume(5);  // Mo¿esz ustawiæ g³oœnoœæ dŸwiêku
    stepSound.setLoop(true);   // Ustawienie pêtli, ¿eby dŸwiêk by³ odtwarzany w pêtli
    stepSound.setPitch(1.9f);  // Przyspieszenie odtwarzania dŸwiêku o 1.5x
}

void Player::move(const sf::Vector2f& direction, const std::vector<std::vector<int>>& maze) {    
    // Obs³uga kierunku lewo/prawo - odwracanie postaci gracza
    if (direction.x > 0) {
        bobSprite.setScale(-std::abs(bobSprite.getScale().x), bobSprite.getScale().y); // Lewo
    }
    else if (direction.x < 0) {
        bobSprite.setScale(std::abs(bobSprite.getScale().x), bobSprite.getScale().y); // Prawo
    }
    
    // Obliczenie mno¿nika prêdkoœci na podstawie wielkoœci komórki
    float speedMultiplier = .9f + (float)maze.size() / 1000.f;
    //std::cout << "ND.x >> " << normalizedDirection.x << ", ND.y >> " << normalizedDirection.y << ", SM >> " << speedMultiplier << ", Maze Size >> " << (float)maze.size() << std::endl;
    
    // Aktualizacja pozycji gracza
    sf::Vector2f newPosition = position;
    sf::Vector2f potentialPositionX = position + sf::Vector2f(direction.x * speedMultiplier, 0.f);
    sf::Vector2f potentialPositionY = position + sf::Vector2f(0.f, direction.y * speedMultiplier);

    if (checkCollision(potentialPositionX, maze)) {
        newPosition.x += direction.x * speedMultiplier;
    }
    if (checkCollision(potentialPositionY, maze)) {
        newPosition.y += direction.y * speedMultiplier;
    }

    // Gracz siê porusza
    if (newPosition != position) {
        if (stepSound.getStatus() != sf::Sound::Playing) {
            stepSound.play();
        }
    }
    // Gracz stoi w miejscu
    else {
        if (stepSound.getStatus() == sf::Sound::Playing) {
            stepSound.stop();
        }
    }
    
    position = newPosition;
    bobSprite.setPosition(position);
}

bool Player::checkCollision(const sf::Vector2f& newPosition, const std::vector<std::vector<int>>& maze) const {
    sf::FloatRect spriteBounds = bobSprite.getGlobalBounds();

    // Aktualizuj granice hitboxa dla nowej pozycji
    sf::FloatRect futureBounds(newPosition.x - spriteBounds.width / 2,
        newPosition.y - spriteBounds.height / 2,
        spriteBounds.width, spriteBounds.height);

    // Sprawdzanie kolizji dla kluczowych punktów (cztery rogi hitboxa)
    sf::Vector2f checkPoints[4] = {
        { futureBounds.left, futureBounds.top },
        { futureBounds.left + futureBounds.width, futureBounds.top },
        { futureBounds.left, futureBounds.top + futureBounds.height },
        { futureBounds.left + futureBounds.width, futureBounds.top + futureBounds.height }
    };

    for (const auto& point : checkPoints) {
        int gridX = static_cast<int>(point.x / cellSize);
        int gridY = static_cast<int>(point.y / cellSize);

        if (gridX < 0 || gridY < 0 || gridX >= maze[0].size() || gridY >= maze.size() || maze[gridY][gridX] == 0) {
            return false; // Kolizja wykryta
        }
    }

    return true; // Brak kolizji
}


void Player::setPosition(float x, float y) {
    position = { x, y };
    bobSprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    // Jeœli animacja pojawiania siê, zmniejsz przezroczystoœæ
    if (isAppearing) {
        isPortalTriggered = true;
        isTeleporting = false;
        opacity += 6.f; // Przyspieszamy animacjê pojawiania
        if (opacity > 255.f) {
            opacity = 255.f; // Zapewniamy, ¿e nie przekroczy 255
            isAppearing = false;
        }
        bobSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(opacity)));
    }
    
    // Jeœli teleportacja w toku, zmniejsz przezroczystoœæ
    if (isTeleporting) {
        isAppearing = false;
        opacity -= 5.f; // Przyspieszamy znikanie
        if (opacity <= 0.f) {
            opacity = 0.f;
            isTeleporting = false;  // Zakoñcz teleportacjê
        }
        bobSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(opacity)));
    }
    
    // Rysowanie poœwiaty
    if (false && currentLevel > 30) {
        glowCircle.setPosition({ position.x, position.y });
        window.draw(glowCircle);
    }
    
    window.draw(bobSprite);

    if (isPortalTriggered) {
        sf::Color portalColor = portalSprite.getColor();
        portalColor.a -= 3.5f; // Stopniowe zwiêkszanie przezroczystoœci
        if (portalColor.a <= 0) {
            portalColor.a = 0;
            isPortalTriggered = false;
        }
        portalSprite.setColor(portalColor);
    }
    window.draw(portalSprite);
}

void Player::appearPlayer() {
    isAppearing = true;
    opacity = 0.f;
    
    portalSprite.setPosition({ position.x, position.y + 14});
}

sf::Vector2f Player::getPosition() const {
    return { position.x / cellSize, position.y / cellSize };
}

void Player::updateCellSize(float newSize) {
    cellSize = newSize;

    // Skalowanie portalu
    sf::Vector2u textureSize = portalTexture.getSize();
    float scaleX = cellSize / textureSize.x;
    float scaleY = cellSize / textureSize.y / 2.5f;
    
    portalSprite.setScale(scaleX, scaleY);

    // Skalowanie Boba
    textureSize = bobTexture.getSize();
    scaleX = (cellSize / textureSize.x) * 0.8;
    scaleY = (cellSize / textureSize.y) * 0.8;

    bobSprite.setScale(scaleX, scaleY);

    // Ustawienie pozycji z nowym rozmiarem
    bobSprite.setPosition(position);
}