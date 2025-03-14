#pragma once
#include <vector>

class Maze {
public:
    Maze(int width, int height);
    void generate();
    const std::vector<std::vector<int>>& getMaze() const;
    
private:
    int width, height;
    std::vector<std::vector<int>> maze;
    void generateDFS(int x, int y);
    void generatePrim();
};
