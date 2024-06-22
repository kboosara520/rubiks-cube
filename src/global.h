#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NINETY 90.0f
#define ONE 1.0f

// define keys
#define UP_KEY GLFW_KEY_UP
#define DOWN_KEY GLFW_KEY_DOWN
#define LEFT_KEY GLFW_KEY_LEFT
#define RIGHT_KEY GLFW_KEY_RIGHT
#define R_KEY GLFW_KEY_R
#define L_KEY GLFW_KEY_L
#define U_KEY GLFW_KEY_U
#define D_KEY GLFW_KEY_D
#define F_KEY GLFW_KEY_F
#define B_KEY GLFW_KEY_B
#define M_KEY GLFW_KEY_M

#define X 1
#define X_ 2
#define Y 3
#define Y_ 4
#define Z 5
#define Z_ 6

#define R 1
#define R_ -1
#define L 3
#define L_ -3
#define U 5
#define U_ -5
#define D 7
#define D_ -7
#define F 9
#define F_ -9
#define B 11
#define B_ -11
#define M 13
#define M_ -13
#define TS_R 2
#define TS_R_ -2
#define TS_L 6
#define TS_L_ -6
#define TS_U 10
#define TS_U_ -10
#define TS_D 14
#define TS_D_ -14
#define TS_F 18
#define TS_F_ -18
#define TS_B 22
#define TS_B_ -22

#define SCRAMBLE_LENGTH 30

extern const unsigned int WIDTH;
extern const unsigned int HEIGHT;

extern const int MAX_FPS;

extern const float sensitivity;

extern const unsigned int DIM;
extern const unsigned int FACE_COUNT;
extern const unsigned int PIECE_COUNT;
extern const float CUBE_SIZE;

extern const unsigned int FRONT;
extern const unsigned int BACK;
extern const unsigned int LEFT;
extern const unsigned int RIGHT;
extern const unsigned int BOTTOM;
extern const unsigned int TOP;

extern const glm::vec3 BLACK;
extern const glm::vec3 WHITE;
extern const glm::vec3 GREEN;
extern const glm::vec3 RED;
extern const glm::vec3 BLUE;
extern const glm::vec3 YELLOW;
extern const glm::vec3 ORANGE;

extern const float vertices[6][36];
extern unsigned int indices[6];

extern unsigned int VAO[6], VBO[6];
extern unsigned int EBO;

extern const glm::vec3 ROTATE_Y_AXIS;
extern const glm::vec3 ROTATE_X_AXIS;
extern const glm::vec3 ROTATE_Z_AXIS;

extern float deltaTime;
extern float lastFrame;

extern int speed;

extern const unsigned int scramble_length;
#endif