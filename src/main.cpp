#include <vector>
#include <chrono>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "global.h"
#include "cube.h"
#include "piece.h"

const unsigned int KEY_COUNT = 1024;
bool keys[KEY_COUNT];
bool keysPressed[KEY_COUNT];

bool firstMouse = true;
float yaw   = -123.0f;
float pitch =  -20.0f;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;
float fov   =  45.0f;

glm::vec3 cameraPos = glm::vec3(6.0f, 4.0f, 9.0f);
glm::vec3 cameraFront = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
glm::vec3 cameraFrontFPS = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool cameraMode = true;
bool outOfFocus = false;

// glm::vec3 light_pos = glm::vec3(7.0f, 0.0f, -11.0f);

float lightVertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

GLFWwindow* createWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window, Cube& cube);
void processCamera(GLFWwindow* window);
void processRotation(GLFWwindow* window, Cube& cube);
void printError();

int main(void)
{
    GLFWwindow* window = createWindow();
    if (window == NULL)
    {   
        return -1;
    }
    Shader shader("./src/shaders/cube.vshader", "./src/shaders/cube.fshader");
    Shader lightShader("./src/shaders/light.vshader", "./src/shaders/light.fshader");

    Cube cube(shader.ID);

    unsigned int lightVBO, lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBindVertexArray(lightVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    const auto loop_delay = std::chrono::milliseconds(1000 / MAX_FPS);

    float angle = 0;

    while(!glfwWindowShouldClose(window))
    {
        auto start = std::chrono::high_resolution_clock::now();
        if (!(cube.animating || cube.scrambling || outOfFocus))
        {
            processInput(window, cube);
        }
        if (cube.scrambling && !cube.animating)
        {
            int length = cube.scramble_moves.size();
            if (length > 0)
            {
                cube.turn(cube.scramble_moves[length - 1]);
                cube.scramble_moves.pop_back();
            }
            else
            {
                cube.scrambling = false;
                speed = 5;
            }
        }
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        float radius = 10.0f;
        float time = glfwGetTime();
        float light_x = static_cast<float>(sin(time) * radius);
        float light_z = static_cast<float>(cos(time) * radius);

        glm::vec3 light_pos(light_x, 0.0f, light_z);

        // draw light cube
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setMat4("model", model);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader.use();
        shader.setVec3("lightColor", glm::vec3(1.0f));
        shader.setVec3("light_pos", light_pos);
        shader.setVec3("viewPos", cameraPos);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        cube.render();

        angle += 1.0f;
        if (angle >= 360) {
            angle = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        if (elapsed < loop_delay)
        {
            std::this_thread::sleep_for(loop_delay - elapsed);
        }
    }

    glfwTerminate();

    return 0;
}

GLFWwindow* createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rubik's Cube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialized GLAD" << std::endl;
        return NULL;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Cube& cube)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        outOfFocus = true;
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if ((glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS))
    {  
        if (keys[GLFW_KEY_1] && !keysPressed[GLFW_KEY_1])
        {
            cameraMode = !cameraMode;
            keysPressed[GLFW_KEY_1] = true;
            std::cout << "Changed to ";
            if (cameraMode)
            {
                std::cout << "camera mode";
                glfwSetCursorPosCallback(window, mouse_callback);
            }
            else
            {
                std::cout << "solving mode";
                glfwSetCursorPosCallback(window, nullptr);
            }
            std::cout << std::endl;
        }
    }
    if (cameraMode)
    {
        processCamera(window);
    }
    else
    {
        processRotation(window, cube);
    }
}

void processCamera(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS))
    {
        return;
    }
    
    float cameraSpeed = static_cast<float>(4.5 * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFrontFPS;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFrontFPS;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;

    // log position
    if (keys[GLFW_KEY_BACKSPACE] && !keysPressed[GLFW_KEY_BACKSPACE])
    {
        std::cout << cameraPos.x << ' ' << cameraPos.y << ' ' << cameraPos.z << std::endl;
        std::cout << "pitch: " << pitch << std::endl;
        std::cout << "yaw: " << yaw << std::endl;
        keysPressed[GLFW_KEY_BACKSPACE] = true;
    }
}

void processRotation(GLFWwindow* window, Cube& cube)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS))
    {
        if (keys[GLFW_KEY_S] && !keysPressed[GLFW_KEY_S])
        {
            cube.scramble();
            keysPressed[GLFW_KEY_S] = true;
        }
    }
    int two_slice_modifier = 1;
    int direction_modifier = 1;
    int direction_offset = 0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        two_slice_modifier = 2;
    }
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
    {
        direction_modifier = -1;
        direction_offset = 1;
    }

    if (keys[LEFT_KEY] && !keysPressed[LEFT_KEY])
    {
        cube.rotate(Y + (direction_offset * 3));
        keysPressed[LEFT_KEY] = true;
    }
    else if (keys[RIGHT_KEY] && !keysPressed[RIGHT_KEY])
    {
        cube.rotate(Y_ + direction_offset);
        keysPressed[RIGHT_KEY] = true;
    }
    else if (keys[UP_KEY] && !keysPressed[UP_KEY])
    {
        cube.rotate(X);
        keysPressed[UP_KEY] = true;
    }
    else if (keys[DOWN_KEY] && !keysPressed[DOWN_KEY])
    {
        cube.rotate(X_);
        keysPressed[DOWN_KEY] = true;
    }
    else if (keys[R_KEY] && !keysPressed[R_KEY])
    {
        cube.turn(R * direction_modifier * two_slice_modifier);
        keysPressed[R_KEY] = true;
    }
    else if (keys[L_KEY] && !keysPressed[L_KEY])
    {
        cube.turn(L * direction_modifier * two_slice_modifier);
        keysPressed[L_KEY] = true;
    }
    else if (keys[U_KEY] && !keysPressed[U_KEY])
    {
        cube.turn(U * direction_modifier * two_slice_modifier);
        keysPressed[U_KEY] = true;
    }
    else if (keys[D_KEY] && !keysPressed[D_KEY])
    {
        cube.turn(D * direction_modifier * two_slice_modifier);
        keysPressed[D_KEY] = true;
    }
    else if (keys[F_KEY] && !keysPressed[F_KEY])
    {
        cube.turn(F * direction_modifier * two_slice_modifier);
        keysPressed[F_KEY] = true;
    }
    else if (keys[B_KEY] && !keysPressed[B_KEY])
    {
        cube.turn(B * direction_modifier * two_slice_modifier);
        keysPressed[B_KEY] = true;
    }
    else if (keys[M_KEY] && !keysPressed[M_KEY])
    {
        cube.turn(M * direction_modifier);
        keysPressed[M_KEY] = true;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < KEY_COUNT)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
            keysPressed[key] = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFrontFPS.x = cos(glm::radians(yaw));
    cameraFrontFPS.z = sin(glm::radians(yaw));
    cameraFront = glm::normalize(direction);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        firstMouse = true;
        outOfFocus = false;
        if (cameraMode) {
            glfwSetCursorPosCallback(window, mouse_callback);
        }
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void printError()
{
    std::cout << glGetError() << std::endl; 
}