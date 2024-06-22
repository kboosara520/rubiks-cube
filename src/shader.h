#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define PROGRAM 0
#define VSHADER 1
#define FSHADER 2

class Shader
{
public:
    unsigned int ID;
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 mat) const;
    void setVec3(const std::string &name, glm::vec3 value) const;

private:
    void checkCompileErrors(unsigned int obj, unsigned int type);
};

#endif