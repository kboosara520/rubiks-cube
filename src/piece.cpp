#include "piece.h"

Piece::Piece(unsigned int shaderID, glm::vec3 real_position, glm::vec3 piecePos)
{
    world_pos = real_position;
    relative_pos = piecePos;
    sid = shaderID;
    if (piecePos.x == -1)
    {
        colors[LEFT] = ORANGE;
    }
    if (piecePos.x == DIM - 2)
    {
        colors[RIGHT] = RED;
    }
    if (piecePos.y == -1)
    {
        colors[BOTTOM] = YELLOW;
    }
    if (piecePos.y == DIM - 2)
    {
        colors[TOP] = WHITE;
    }
    if (piecePos.z == -1)
    {
        colors[BACK] = BLUE;
    }
    if (piecePos.z == DIM - 2)
    {
        colors[FRONT] = GREEN;
    }
}

void Piece::render()
{
    glm::mat4 model = glm::mat4(1.0f);
    if (animating)
    {
        if (rotate_count >= 90)
        {   
            animating = false;
            update_position();
            rotate_count = 0;
            rotating_direction = 0;
        }
        else
        {
            rotate_count += speed;
        }
        spin();
    }
    model = rotation * model;
    model = glm::translate(model, world_pos);
    std::string name = "model";
    int loc = glGetUniformLocation(sid, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
    for (int i = 0; i < FACE_COUNT; i++)
    {
        // set uniform color before rendering
        std::string cname = "color";
        glUniform3f(glGetUniformLocation(sid, cname.c_str()), colors[i].x, colors[i].y, colors[i].z);
        glBindVertexArray(VAO[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void Piece::rotate(int direction)
{
    animating = true;
    rotating_direction = direction;
}

void Piece::spin()
{
    glm::mat4 rotate = glm::mat4(1.0f);
    switch (rotating_direction)
    {
        case X:
            rotate = glm::rotate(rotate, glm::radians(-ONE * (float)speed), ROTATE_X_AXIS);
            break;
        case X_:
            rotate = glm::rotate(rotate, glm::radians(ONE * (float)speed), ROTATE_X_AXIS);
            break;
        case Y:
            rotate = glm::rotate(rotate, glm::radians(-ONE * (float)speed), ROTATE_Y_AXIS);
            break;
        case Y_:
            rotate = glm::rotate(rotate, glm::radians(ONE * (float)speed), ROTATE_Y_AXIS);
            break;
        case Z:
            rotate = glm::rotate(rotate, glm::radians(-ONE * (float)speed), ROTATE_Z_AXIS);
            break;
        case Z_:
            rotate = glm::rotate(rotate, glm::radians(ONE * (float)speed), ROTATE_Z_AXIS);
            break;
    }
    rotation = rotate * rotation;
}

void Piece::update_position()
{
    glm::mat4 rotate(1.0f);
    switch (rotating_direction)
    {
        case X:
            rotate = glm::rotate(rotate, glm::radians(-NINETY), ROTATE_X_AXIS);
            break;
        case X_:
            rotate = glm::rotate(rotate, glm::radians(NINETY), ROTATE_X_AXIS);
            break;
        case Y:
            rotate = glm::rotate(rotate, glm::radians(-NINETY), ROTATE_Y_AXIS);
            break;
        case Y_:
            rotate = glm::rotate(rotate, glm::radians(NINETY), ROTATE_Y_AXIS);
            break;
        case Z:
            rotate = glm::rotate(rotate, glm::radians(-NINETY), ROTATE_Z_AXIS);
            break;
        case Z_:
            rotate = glm::rotate(rotate, glm::radians(NINETY), ROTATE_Z_AXIS);
            break;
    }
    glm::vec4 new_relative_pos = rotate * glm::vec4(relative_pos, 1.0f);
    relative_pos = glm::vec3(std::round(new_relative_pos.x), std::round(new_relative_pos.y), std::round(new_relative_pos.z));
}