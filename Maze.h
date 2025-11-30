#pragma once
#include <vector>
#include "Door.h"
#include "Key.h"

class Maze {
public:
    Maze(int width, int height);
    void generate();
    const std::vector<std::vector<int>>& getMaze() const;
    Door* door = new Door({ -1, -1 }, { 1,1 });
    Key* key = new Key({ -1,-1 }, 1, 0);

    int neighborCount(int x, int y) const;
    bool isDeadEnd(int x, int y) const;
    void addRandomLoops(float loopChance = 0.15f, int searchRadius = 6);

private:
    int width, height;
    std::vector<std::vector<int>> maze;
    void generateDFS(int x, int y);
    void generatePrim();
};
