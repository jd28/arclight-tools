#include "shader.hpp"

#include <nw/log.hpp>
#include <nw/util/ByteArray.hpp>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

void Shader::load(const char* vertex, const char* fragment, QOpenGLFunctions_3_3_Core* gl)
{
    unsigned int vs, fs;
    int success;
    char infoLog[512];

    // vertex Shader
    vs = gl->glCreateShader(GL_VERTEX_SHADER);
    gl->glShaderSource(vs, 1, &vertex, NULL);
    gl->glCompileShader(vs);
    gl->glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        gl->glGetShaderInfoLog(vs, 512, NULL, infoLog);
        LOG_F(ERROR, "SHADER::VERTEX::COMPILATION_FAILED: {}", infoLog);
    };

    // fragment Shader
    fs = gl->glCreateShader(GL_FRAGMENT_SHADER);
    gl->glShaderSource(fs, 1, &fragment, NULL);
    gl->glCompileShader(fs);
    gl->glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        gl->glGetShaderInfoLog(fs, 512, NULL, infoLog);
        LOG_F(ERROR, "SHADER::fragment::COMPILATION_FAILED: {}", infoLog);
    };

    // shader Program
    id_ = gl->glCreateProgram();
    gl->glAttachShader(id_, vs);
    gl->glAttachShader(id_, fs);
    gl->glLinkProgram(id_);
    // print linking errors if any
    gl->glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        gl->glGetShaderInfoLog(vs, 512, NULL, infoLog);
        LOG_F(ERROR, "SHADER::VERTEX::LINKING_FAILED", infoLog);
    }

    gl->glDeleteShader(vs);
    gl->glDeleteShader(fs);
}

void Shader::use(QOpenGLFunctions_3_3_Core* gl)
{
    gl->glUseProgram(id_);
}

void Shader::set_uniform(const std::string& name, bool value, QOpenGLFunctions_3_3_Core* gl)
{
    gl->glUniform1i(gl->glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::set_uniform(const std::string& name, int value, QOpenGLFunctions_3_3_Core* gl)
{
    gl->glUniform1i(gl->glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::set_uniform(const std::string& name, float value, QOpenGLFunctions_3_3_Core* gl)
{
    gl->glUniform1f(gl->glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::set_uniform(const std::string& name, const glm::mat4& value, QOpenGLFunctions_3_3_Core* gl)
{
    unsigned int transformLoc = gl->glGetUniformLocation(id_, name.c_str());
    gl->glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}
