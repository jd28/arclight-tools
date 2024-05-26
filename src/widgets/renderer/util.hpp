#pragma once

#include <glm/glm.hpp>

#include <filesystem>

// Gets path to the shaders depending on bgfx backend
// std::filesystem::path get_shader_path();

// Logs matrix
void log_matrix(const float* mtx);
void log_matrix(const glm::mat4& mtx);
