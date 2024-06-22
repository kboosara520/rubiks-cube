#include "cube.h"

Cube::Cube(unsigned int shaderID)
{
    sid = shaderID;
    glGenVertexArrays(FACE_COUNT, VAO);
    glGenBuffers(FACE_COUNT, VBO);
    for (int i = 0; i < FACE_COUNT; i++)
    {
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 6 * sizeof(float), vertices[i], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    pieces.reserve(27);

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            for (int k = 0; k < DIM; k++) {
                float offset = CUBE_SIZE * (DIM - 1) / 2;
                float x = CUBE_SIZE * i - offset;
                float y = CUBE_SIZE * j - offset;
                float z = CUBE_SIZE * k - offset;
                pieces.emplace_back(Piece(shaderID, glm::vec3(x, y, z), glm::vec3(i - 1, j - 1, k - 1)));
            }
        }
    }
}

void Cube::render()
{
    if (animating)
    {
        if (rotate_count >= 90)
        {
            animating = false;
            // std::cout << "unfreeze" << std::endl;
            rotate_count = 0;
        }
        else
        {
            rotate_count += speed;
        }
    }
    for (int i = 0; i < PIECE_COUNT; i++)
    {
        pieces[i].render();
    }
}

void Cube::rotate(int direction)
{
    animating = true;
    // std::cout << "freeze" << std::endl;
    for (int i = 0; i < PIECE_COUNT; i++)
    {
        pieces[i].rotate(direction);
    }
}

void Cube::turn(int move)
{
    animating = true;
    // std::cout << "freeze" << std::endl;
    if (scrambling)
    {

    }
    std::function<bool(int)> condition;
    int direction;
    switch (move)
    {
        case R:
        case R_:
            condition = [this](const int& index) { return pieces[index].relative_pos.x == 1; };
            direction = (move == R) ? X : X_;
            break;
        case TS_R:
        case TS_R_:
            condition = [this](const int& index) { return pieces[index].relative_pos.x >= 0; };
            direction = (move == TS_R) ? X : X_;
            break;
        case L:
        case L_:
            condition = [this](const int& index) { return pieces[index].relative_pos.x == -1; };
            direction = (move == L) ? X_ : X;
            break;
        case TS_L:
        case TS_L_:
            condition = [this](const int& index) { return pieces[index].relative_pos.x <= 0; };
            direction = (move == TS_L) ? X_ : X;
            break;
        case U:
        case U_:
            condition = [this](const int& index) { return pieces[index].relative_pos.y == 1; };
            direction = (move == U) ? Y : Y_;
            break;
        case TS_U:
        case TS_U_:
            condition = [this](const int& index) { return pieces[index].relative_pos.y >= 0; };
            direction = (move == TS_U) ? Y : Y_;
            break;
        case D:
        case D_:
            condition = [this](const int& index) { return pieces[index].relative_pos.y == -1; };
            direction = (move == D) ? Y_ : Y;
            break;
        case TS_D:
        case TS_D_:
            condition = [this](const int& index) { return pieces[index].relative_pos.y <= 0; };
            direction = (move == TS_D) ? Y_ : Y;
            break;
        case F:
        case F_:
            condition = [this](const int& index) { return pieces[index].relative_pos.z == 1; };
            direction = (move == F) ? Z : Z_;
            break;
        case TS_F:
        case TS_F_:
            condition = [this](const int& index) { return pieces[index].relative_pos.z >= 0; };
            direction = (move == TS_F) ? Z : Z_;
            break;
        case B:
        case B_:
            condition = [this](const int& index) { return pieces[index].relative_pos.z == -1; };
            direction = (move == B) ? Z_: Z;
            break;
        case TS_B:
        case TS_B_:
            condition = [this](const int& index) { return pieces[index].relative_pos.z <= 0; };
            direction = (move == TS_B) ? Z_: Z;
            break;
        case M:
        case M_:
            condition = [this](const int& index) { return pieces[index].relative_pos.x == 0; };
            direction = (move == M) ? X : X_;
            break;
        default:
            std::cout << "Unhandled move" << std::endl;
            break;
    }
    for (int i = 0; i < PIECE_COUNT; i++)
    {
        if(condition(i))
        {
            pieces[i].rotate(direction);
        }
    }
}

void Cube::scramble()
{   
    scrambling = true;
    speed = 15;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 11);
    scramble_moves.reserve(SCRAMBLE_LENGTH);
    int count = 0;
    int loop_count = 50;
    while (count < SCRAMBLE_LENGTH)
    {
        int move = dist(gen);
        int length = scramble_moves.size();
        if (length == 0 || (scramble_moves[length] != (-1 * scramble_moves[length - 1])) || loop_count > 10)
        {
            scramble_moves.emplace_back(moves[dist(gen)]);
            count += 1;
            continue;
        }
        loop_count += 1;
    }
}