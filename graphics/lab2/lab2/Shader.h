#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;
    // constructor generates the shader on the fly

    Shader(const char* vertexPath, const char* fragmentPath);
    // activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void set3f(const std::string& name, float val1, float val2, float val3);
    void set2f(const std::string& name, float val1, float val2);
    void setMatrix4fv(const std::string& name, glm::mat4 mat);

private:
    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};