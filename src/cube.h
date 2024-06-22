#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <functional>
#include <random>
#include "global.h"
#include "piece.h"

class Cube 
{
public:
    Cube(unsigned int shaderID);
    void render();
    // rotate turns every piece
    void rotate(int direction);
    // turn only turns a face(9 pieces)
    void turn(int move);
    void scramble();
    bool animating = false;
    bool scrambling = false;
    std::vector<int> scramble_moves;
private:
    int rotate_count = 0;
    int moves[12] = {-11, -9, -7, -5, -3, -1, 1, 3, 5, 7, 9, 11};
    std::vector<Piece> pieces;
    uint sid;
};

#endif