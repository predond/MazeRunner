#include "Game.h"
#include <iostream>
#include <time.h>
#include <thread>
#include <chrono>
#include <fstream>

#define M_PI 3.14159265358979323846

#define MEDIUM_LVL 20
#define HIGH_LVL 60

std::vector<std::string> v = { };
Game::Resolution res = {800.f, 800.f};

int currentLevel = 1;

Game::Game() : Game(21, 21) { }

Game::Game(int mazeWidth, int mazeHeight)
    : window(sf::VideoMode(res.width, res.height), "Maze Runner"),
    maze(mazeWidth, mazeHeight),
    player(0, 0, res.width / mazeWidth),
    solver(),
    cellSize(res.width / mazeWidth),
    menu(res.width, res.height),
    gameState(GameState::MENU),
    pauseMenu(res.width, res.height) {
    
    currentLevel = 1;
    // Ładowanie czcionki
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Nie udało się załadować czcionki!" << std::endl;
        // Obsługa błędów
    }

    if (!wallTexture.loadFromFile("assets/crate_small.png")) {
        std::cerr << "Nie udało się załadować tekstury dla ściany!" << std::endl;
        // Obsługa błędów
    }

    if (!pathTexture.loadFromFile("assets/floor.png")) {
        std::cerr << "Nie udało się załadować tekstury dla ścieżki!" << std::endl;
        // Obsługa błędów
    }

    if (!trailTexture.loadFromFile("assets/trail.png")) {
        std::cerr << "Nie udało się załadować tekstury dla śladów!" << std::endl;
        // Obsługa błędów
    }
    
    // Wczytaj muzykę
    if (!backgroundMusic.openFromFile("assets/back_music.mp3")) {
        throw std::runtime_error("Nie udało się wczytać muzyki!");
    }

    // Ustaw muzykę na pętlę (aby grała cały czas)
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(50);

    // Rozpocznij odtwarzanie
    //backgroundMusic.play();

    // Ustawienie sprite'ów
    wallSprite.setTexture(wallTexture);
    pathSprite.setTexture(pathTexture);
    trailSprite.setTexture(trailTexture);
    
    wallSprite.setScale(cellSize / wallTexture.getSize().x, cellSize / wallTexture.getSize().y);
    pathSprite.setScale(cellSize / pathTexture.getSize().x, cellSize / pathTexture.getSize().y);
    trailSprite.setScale(cellSize / trailTexture.getSize().x * 0.8, cellSize / trailTexture.getSize().y * 0.8);
    
    // Pasek menu
    menuBar.setSize(sf::Vector2f(window.getSize().x, 30));  // Wysokość paska
    menuBar.setFillColor(sf::Color(100, 100, 100, 100));  // Kolor paska
    
    // Tekst wyświetlający poziom
    levelText.setFont(font);
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(window.getSize().x - 200.f, 4.f);  // Pozycja po prawej

    // Tekst wyświetlający czas
    timerText.setFont(font);
    timerText.setCharacterSize(20);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(window.getSize().x / 2 - 100.f, 4.f);  // Pozycja na środku
    
    // Mgła wojny
    fogTexture.create(res.width, res.height); // Bufor dla mgły
    fogRectangle.setSize(sf::Vector2f(res.width, res.height));
    fogRectangle.setTexture(&fogTexture.getTexture());
    
    fogHole.setFillColor(sf::Color::Transparent);
    
    prepareLevel(mazeWidth, mazeHeight);
}

void Game::run() {
    while (window.isOpen()) {
        processInput();
        if (gameState == GameState::PLAYING) {
            update();
        }
        render();
    }
}

void breakSound(Player& _player) {
    _player.stepSound.stop();
}

void Game::processInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (gameState == GameState::PLAYING || gameState == GameState::PAUSED) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (gameState == GameState::PLAYING) {
                        gameState = GameState::PAUSED;
                        backgroundMusic.pause();
                        breakSound(player);
                        totalPausedTime += levelClock.getElapsedTime();
                    }
                    else if (gameState == GameState::PAUSED) {
                        gameState = GameState::PLAYING;
                        backgroundMusic.play();
                        levelClock.restart();
                    }
                }
            }

            if (gameState == GameState::MENU) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    int selected = menu.getSelectedIndex();
                    switch (selected) {
                    case 0: // Rozpocznij nową grę
                        gameState = GameState::PLAYING;
                        if (backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
                            backgroundMusic.play();
                        prepareLevel(21, 21);
                        saveGame();
                        break;
                    case 1: // Zapisz grę
                        saveGame();
                        break;
                    case 2: // Wczytaj grę
                        loadGame();
                        break;
                    case 3: // Wyjdź
                        window.close();
                        break;
                    }
                }
            }

            if (gameState == GameState::PAUSED) {
                if (event.key.code == sf::Keyboard::Up) {
                    pauseMenu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    pauseMenu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    int selected = pauseMenu.getSelectedIndex();
                    switch (selected) {
                    case 0: // Wznów
                        gameState = GameState::PLAYING;
                        if (backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
                            backgroundMusic.play();
                        levelClock.restart();
                        break;
                    case 1: // Zapisz grę
                        saveGame();
                        break;
                    case 2: // Wyjdź
                        window.close();
                        break;
                    }
                }
            }

            // Dodaj obsługę klawisza V dla solvera A*
            if (event.key.code == sf::Keyboard::V && gameState == GameState::PLAYING) {
                isAStarActive = !isAStarActive;
                
                if (isAStarActive) {
                    sf::Vector2f pozycjaGracza = player.getPosition();
                    solver.reset();
                    sciezkaLabiryntu = solver.znajdzSciezkeAStar(maze.getMaze(), pozycjaGracza, exitPosition);
                    if (sciezkaLabiryntu.empty()) {
                        std::cout << "Nie znaleziono ścieżki!" << std::endl;
                        isAStarActive = false;
                    }
                }
            }
        }
    }

    // Obsługa sterowania grą
    if (gameState == GameState::PLAYING) {
        sf::Vector2f direction(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) direction.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) direction.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) direction.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) direction.x += 1;

        if (direction.x != 0 || direction.y != 0) {
            // Normalizacja wektora kierunku
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= length;
        }
        
        if (!exiting)
            player.move(direction, maze.getMaze());
        
        /*if (!isAStarActive)
            DrawStepTrail(direction);
        */
        // Logowanie współrzędnych co sekundę
        if (logClock.getElapsedTime().asSeconds() >= 1.0f) {
            sf::Vector2f playerPos = player.getPosition();
            sf::Vector2f metaPos(exitPosition.x, exitPosition.y);

            /*system("cls");
            std::cout << "Gracz: (" << playerPos.x << ", " << playerPos.y << ") | "
                << "Meta: (" << metaPos.x << ", " << metaPos.y << ") | "
                << "Cell Size: (" << cellSize << ")\n\nWon coordinates:\n";
            for (std::vector<std::string>::iterator t = v.begin(); t != v.end(); t++)
            {
                std::cout << *t << std::endl;
            }*/

            Game::logClock.restart(); // Reset zegara
        }
    }
}

void Game::update() {
    if (gameState == GameState::PAUSED) return;
    
    // Aktualizacja widoczności we mgle
    sf::Vector2f playerPos = player.getPosition();

    deltaTime = clock.restart().asSeconds(); // Czas w sekundach od ostatniego wywołania
    
    // Czas przejścia poziomu
    sf::Time elapsedTime = totalPausedTime + levelClock.getElapsedTime();
    int minutes = elapsedTime.asSeconds() / 60;
    int seconds = static_cast<int>(elapsedTime.asSeconds()) % 60;
    timerText.setString("Czas: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));

    // Zaktualizowanie poziomu
    levelText.setString("Poziom: " + std::to_string(currentLevel));
    
    if (isAStarActive && !sciezkaLabiryntu.empty()) {
        sf::Vector2f nextPosition = sciezkaLabiryntu.front();
        nextPosition.x = (nextPosition.x * cellSize + cellSize / 2.0f) + .5f;
        nextPosition.y = (nextPosition.y * cellSize + cellSize / 2.0f) + .5f;

        playerPos = { playerPos.x * cellSize, playerPos.y * cellSize};
        
        // Różnica między aktualną pozycją a kolejnym punktem na ścieżce
        sf::Vector2f difference = nextPosition - playerPos;
        float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);
        
        //std::cout << "Actual: (" << playerPos.x << "," << playerPos.y << ")" << "  Next: (" << nextPosition.x << "," << nextPosition.y << ")" << "Distance: " << distance << ", Difference: (" << difference.x << "," << difference.y << ")" << std::endl;
        if (distance < 1.f) {
            // Osiągnięto punkt, przejdź do następnego
            sciezkaLabiryntu.erase(sciezkaLabiryntu.begin());
            if (sciezkaLabiryntu.empty()) {
                std::cout << "Wylaczam solver." << std::endl;
                isAStarActive = false; // Wyłącz solver, jeśli ścieżka jest ukończona
            }
        }
        else {
            // Normalizuj kierunek ruchu i poruszaj gracza
            sf::Vector2f direction = difference / distance;
            DrawStepTrail(direction);

            player.move(direction, maze.getMaze());
        }
    }

    int won = checkWinCondition();
    if (won) {
        std::string msg = "x=" + std::to_string(playerPos.x) + ", y=" + std::to_string(playerPos.y) + "end was on >> x=" + std::to_string(exitPosition.x) + ", y=" + std::to_string(exitPosition.y);
        if (!exiting)
        {
            printf("Maze completed!\n");
            v.push_back(msg);
            resetGame();
        }
    }
}

void Game::DrawStepTrail(sf::Vector2f& direction)
{
    if (currentLevel < HIGH_LVL) return;
    
    // Oblicz środek obecnej komórki
    sf::Vector2f currentCellCenter(
        static_cast<int>(player.getPosition().x),
        static_cast<int>(player.getPosition().y)
    );

    /*std::cout << "Diff: (" << difference.x << "," << difference.y << "), " <<
    "Dist: (" << distance << "), " <<
    "Dire: (" << direction.x << "," << direction.y << "), ";*/
    if (visitedCells.find({ currentCellCenter.x, currentCellCenter.y }) == visitedCells.end())
    {
        float rotation = std::atan2(direction.y, direction.x) * 180.0f / M_PI;
        rotation += 90;

        // Dodaj ślad do listy
        visitedCells.insert({ currentCellCenter.x, currentCellCenter.y });
        trailMarks.push_back({ currentCellCenter, rotation, 0.0f });
        //std::cout << "Rota: (" << rotation << ")";
    }
}

void Game::render() {
    window.clear();
    if (gameState == GameState::MENU) {
        menu.draw(window);
    }
    else if (gameState == GameState::PAUSED) {
        pauseMenu.draw(window);
    }
    else if (gameState == GameState::PLAYING) {
        // Renderowanie gry
        const auto& mazeGrid = maze.getMaze();
        for (size_t y = 0; y < mazeGrid.size(); y++) {
            for (size_t x = 0; x < mazeGrid[y].size(); x++) {
                // Ściana
                if (mazeGrid[y][x] == 0) {
                    wallSprite.setPosition(x * cellSize, y * cellSize);
                    window.draw(wallSprite);
                }
                // Ścieżka
                else {
                    pathSprite.setPosition(x * cellSize, y * cellSize);
                    window.draw(pathSprite);
                }
            }
        }
        
        // Wyświetlenie wyjścia
        sf::RectangleShape metaArea;
        metaArea.setSize(cell.getSize());
        metaArea.setFillColor(sf::Color(0, 255, 0, 100)); // Przezroczysty zielony
        metaArea.setPosition(exitPosition.x * cell.getSize().x, exitPosition.y * cell.getSize().y);
        
        // Iteruj po odwiedzonych komórkach i rysuj ślad
        for (auto it = trailMarks.begin(); it != trailMarks.end();) {
            it->timeElapsed += deltaTime; // deltaTime to czas od ostatniej klatki
            
            // Zwiększ przezroczystość w zależności od czasu
            float transparency = 160.0f * (1.0f - it->timeElapsed / 15.0f);
            
            if (transparency <= 0.0f) {
                // Usuń ślad, jeśli jest całkowicie niewidoczny
                visitedCells.erase({ it->position.x, it->position.y });
                it = trailMarks.erase(it);
            }
            else {
                // Pobierz dane śladu
                float cellX = it->position.x;
                float cellY = it->position.y;
                
                // Ustaw pozycję i rotację
                trailSprite.setPosition(cellX * cellSize + cellSize / 2.0f, cellY * cellSize + cellSize / 2.0f);
                trailSprite.setRotation(it->rotation);
                
                // Ustaw punkt pochodzenia na środek tekstury
                trailSprite.setOrigin(trailSprite.getLocalBounds().width / 2.0f,
                    trailSprite.getLocalBounds().height / 2.0f);

                // Ustaw przezroczystość
                sf::Color color = trailSprite.getColor();
                color.a = static_cast<sf::Uint8>(transparency);
                trailSprite.setColor(color);

                // Rysuj ślad
                window.draw(trailSprite);

                ++it;
            }
        }
        
        // Rysowanie gracza
        player.draw(window);
        
        if (exiting && !player.isTeleporting) {
            exiting = false;
            updateDifficulty();
        }

        window.draw(metaArea);
        
        // Rysowanie mgły
        //generateFog();
        
        // Rysowanie menu
        window.draw(menuBar);
        
        window.draw(timerText);
        window.draw(levelText);
    }
    
    window.display();
}

bool Game::checkWinCondition() {
    sf::Vector2f playerPosition = player.getPosition();

    // Obliczenie obszaru mety
    float metaX = exitPosition.x;
    float metaY = exitPosition.y;

    // Sprawdzenie warunków
    return (playerPosition.x >= metaX && playerPosition.x <= metaX + 1 &&
        playerPosition.y >= metaY && playerPosition.y  <= metaY + 1);
}

void Game::generateFog()
{
    if (currentLevel < MEDIUM_LVL) return;
    
    sf::Vector2f pp = player.getPosition();
    float fhr = (2 * cellSize) * ( .66f + (currentLevel / 40.f)); //80.f;
    
    fogHole.setPosition(pp.x * cellSize, pp.y * cellSize);
    fogHole.setRadius(fhr); // Promień dziury
    fogHole.setOrigin(fhr, fhr); // Środek dziury
    
    if (player.isAppearing || currentLevel >= HIGH_LVL)
        fogTexture.clear(sf::Color(0, 0, 0, 240));
    fogTexture.draw(fogHole, sf::BlendNone);  // Wycięcie dziury
    fogTexture.display(); // Aktualizacja mgły
    
    // Rysowanie mgły nad sceną
    window.draw(fogRectangle);
}

void Game::prepareLevel(int mazeWidth, int mazeHeight)
{
    maze = Maze(mazeWidth, mazeHeight);
    maze.generate();
    
    // Ustaw rozmiar jednej komórki
    cell.setSize({ res.width / mazeWidth, (res.height) / mazeHeight });
    player.updateCellSize(cell.getSize().x);   // Aktualizacja rozmiaru gracza
    
    cellSize = res.width / maze.getMaze()[0].size();

    // Ponowne skalowanie śladu gracza
    trailSprite.setScale(cellSize / trailTexture.getSize().x * 0.8, cellSize / trailTexture.getSize().y * 0.8);
    
    // Znajdź pierwsze wolne pole dla gracza
    const auto& mazeGrid = maze.getMaze();
    for (size_t y = 0; y < mazeGrid.size(); y++) {
        for (size_t x = 0; x < mazeGrid[y].size(); x++) {
            if (mazeGrid[y][x] == 1) { // Pierwsze pole wolne
                float startX = x * cell.getSize().x + cell.getSize().x / 2;
                float startY = (y * cell.getSize().y) + cell.getSize().y / 2;
                player.setPosition(startX, startY);
                player.appearPlayer();
                goto exitLoop; // Wyjście z obu pętli
            }
        }
    }
exitLoop:

    // Znajdź wyjście w 4 ćwiartce labiryntu
    std::vector<std::pair<int, int>> farCells;
    for (size_t y = mazeGrid.size() - 1; y > mazeGrid.size() / 2; --y) {
        for (size_t x = mazeGrid[y].size() - 1; x > mazeGrid[y].size() / 2; --x) {
            if (mazeGrid[y][x] == 1) {
                exitPosition = { static_cast<float>(x), static_cast<float>(y) };
                goto foundExit;  // Przerywa pętlę po znalezieniu pierwszej komórki
            }
        }
    }
foundExit:

    if (!farCells.empty()) {
        auto exitCell = farCells[rand() % farCells.size()];
        exitPosition = { static_cast<float>(exitCell.first), static_cast<float>(exitCell.second) };
    }
    
    visitedCells.clear();
    trailMarks.clear();
    levelClock.restart();
}

void Game::resetGame() {
    exiting = true;
    player.isTeleporting = true;
    
    totalPausedTime = sf::Time::Zero;
}

void Game::updateDifficulty() {
    currentLevel++;
    int newSize = 21 + (((currentLevel >= 80 ? 80 : currentLevel) / 3) * 2);
    
    prepareLevel(newSize, newSize);
}

void Game::saveGame() {
    std::ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << currentLevel;
        saveFile.close();
        std::cout << "Gra zapisana!" << std::endl;
    }
}

void Game::loadGame() {
    std::ifstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile >> currentLevel;
        saveFile.close();
        std::cout << "Gra wczytana na poziomie " << currentLevel << "!" << std::endl;
        currentLevel--;
        resetGame();
        gameState = GameState::PLAYING;
        if (backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
            backgroundMusic.play();
    }
}
