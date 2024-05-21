#pragma once

#include <glm/glm.hpp>

#include <QOpenGLFunctions_3_3_Core>

struct Shader {
    // the program ID
    unsigned int id_ = 0;

    Shader() = default;
    void load(const char* vertex, const char* fragment, QOpenGLFunctions_3_3_Core* gl);

    void use(QOpenGLFunctions_3_3_Core* gl);

    void set_uniform(const std::string& name, bool value, QOpenGLFunctions_3_3_Core* gl);
    void set_uniform(const std::string& name, int value, QOpenGLFunctions_3_3_Core* gl);
    void set_uniform(const std::string& name, float value, QOpenGLFunctions_3_3_Core* gl);
    void set_uniform(const std::string& name, const glm::mat4& value, QOpenGLFunctions_3_3_Core* gl);
};

struct ShaderPrograms {
    Shader basic;
    Shader skin;
};
