#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Maze.h"
#include "Player.h"
#include "Menu.h"
#include "PauseMenu.h"
#include "Solver.h"

static float menuHeight = 50.0f; // Wysokoœæ paska menu (np. 50px)
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
    sf::Time totalPausedTime; // Zmienna do przechowywania ca³kowitego czasu pauzy
    float cellSize;
    struct TrailMark {
        sf::Vector2f position; // Pozycja œrodka komórki
        float rotation;        // K¹t obrotu w stopniach
        float timeElapsed;     // Czas od pozostawienia œladu
    };

    std::set<std::pair<int, int>> visitedCells; // Unikalne identyfikatory komórek
    std::vector<TrailMark> trailMarks; // Lista aktywnych œladów
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
    sf::Text levelText;              // Tekst wyœwietlaj¹cy poziom
    sf::Text timerText;              // Tekst wyœwietlaj¹cy czas
    sf::Clock levelClock;            // Zegar do œledzenia czasu poziomu
    sf::Font font;

    // Fog of War
    sf::RenderTexture fogTexture;      // Bufor renderowania mg³y
    sf::RectangleShape fogRectangle;  // Prostok¹t na pe³ny ekran
    sf::CircleShape fogHole;          // Otwór wokó³ gracza

    bool exiting;

    void processInput();
    void render();
    bool checkWinCondition();
    void resetGame();
};