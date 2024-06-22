#ifndef PIECE_H
#define PIECE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "global.h"

class Piece 
{
public:
    Piece(unsigned int shaderID, glm::vec3 real_position, glm::vec3 piecePos);
    glm::vec3 relative_pos;
    void render();
    void rotate(int direction);
private:
    uint sid;
    int rotating_direction = 0;
    int rotate_count = 0;
    bool animating = false;
    glm::vec3 world_pos;
    glm::vec3 colors[6] = {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};
    glm::mat4 rotation = glm::mat4(1.0f);
    void update_position();
    void spin();
};

#endif