#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Maze.h"
#include "Player.h"
#include "Menu.h"
#include "PauseMenu.h"
#include "Solver.h"

static float menuHeight = 50.0f; // Wysoko�� paska menu (np. 50px)
extern int currentLevel;

enum class GameState { MENU, PLAYING, PAUSED };

class Game {
public:
    struct Resolution {
        float width;
        float height;
    };
    Game();
    Game(int mazeWidth, int mazeHeight);
    void generateFog();
    void prepareLevel(int mazeWidth, int mazeHeight);
    void run();
    sf::Clock logClock;
    void update();
    void DrawStepTrail(sf::Vector2f& direction);
    void updateDifficulty();
    void saveGame();
    void loadGame();
    
private:
    sf::RenderWindow window;
    GameState gameState;
    PauseMenu pauseMenu;
    Menu menu;
    Maze maze;
    Player player;
    Solver solver;
    sf::RectangleShape cell;
    sf::Vector2f exitPosition;
    sf::Time totalPausedTime; // Zmienna do przechowywania ca�kowitego czasu pauzy
    float cellSize;
    struct TrailMark {
        sf::Vector2f position; // Pozycja �rodka kom�rki
        float rotation;        // K�t obrotu w stopniach
        float timeElapsed;     // Czas od pozostawienia �ladu
    };

    std::set<std::pair<int, int>> visitedCells; // Unikalne identyfikatory kom�rek
    std::vector<TrailMark> trailMarks; // Lista aktywnych �lad�w
    sf::Clock clock;
    float deltaTime = 0.0f;

    // A* Auto Solver
    bool isAStarActive = false;
    std::vector<sf::Vector2f> sciezkaLabiryntu;

    // Assets
    sf::Texture wallTexture;
    sf::Texture pathTexture;
    sf::Texture trailTexture;

    sf::Sprite wallSprite;
    sf::Sprite pathSprite;
    sf::Sprite trailSprite;

    sf::Music backgroundMusic;

    // UI
    sf::RectangleShape menuBar;      // Pasek menu
    sf::Text levelText;              // Tekst wy�wietlaj�cy poziom
    sf::Text timerText;              // Tekst wy�wietlaj�cy czas
    sf::Clock levelClock;            // Zegar do �ledzenia czasu poziomu
    sf::Font font;

    // Fog of War
    sf::RenderTexture fogTexture;      // Bufor renderowania mg�y
    sf::RectangleShape fogRectangle;  // Prostok�t na pe�ny ekran
    sf::CircleShape fogHole;          // Otw�r wok� gracza

    bool exiting;

    void processInput();
    void render();
    bool checkWinCondition();
    void resetGame();
};