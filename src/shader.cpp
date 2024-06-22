#include "shader.h"

Shader::Shader()
{
    ID = 0;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(const std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, VSHADER);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, FSHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, PROGRAM);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glValidateProgram(ID);
}

Shader::~Shader()
{   
    glDeleteProgram(ID);
}

void Shader::use()
{
    glValidateProgram(ID);
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    // std::cout << glGetError() << std::endl;
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::checkCompileErrors(unsigned int obj, unsigned int type)
{
    int success;
    char infoLog[1024];
    if (type == PROGRAM)
    {
        glGetProgramiv(obj, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(obj, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(obj, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::";
            if (type == VSHADER)
            {
                std::cout << "VERTEX";
            }
            else if (type == FSHADER)
            {
                std::cout << "FRAGMENT";
            }
            else
            {
                std::cout << "UNKNOWN";
            }
            std::cout << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
}