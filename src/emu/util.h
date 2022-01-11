
#pragma once

#include <string>
#include <fstream>

#include <GL/glew.h>

#include "spdlog/spdlog.h"

namespace emu
{
namespace util
{
bool readFile(const std::string filename, std::string& file_content)
{
  std::ifstream file(filename);

  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      file_content.append(line);
      file_content.append("\n");
    }
    file.close();
    return true;
  }
  spdlog::error("Failed reading file {}\n", filename);
  return false;
}

void addShader(GLuint program, const std::string& source, GLenum type)
{
  GLuint shader = glCreateShader(type);

  if (shader == 0)
  {
    spdlog::error("Failed to create shader of type {}.", type);
  }

  const char* srcs[] = { source.c_str() };
  const GLint lengths[] = { static_cast<GLint>(source.size()) };

  glShaderSource(shader, 1, srcs, lengths);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar info[1024];
    glGetShaderInfoLog(shader, 1024, NULL, info);
    spdlog::error("Failed to compile shader of type {}, got error\n{}", type, info);
    exit(1);
  }

  glAttachShader(program, shader);
}

static void compileShaders(const std::string& vertex_shader, const std::string& fragment_shader)
{
  GLuint shader_program_id = glCreateProgram();

  if (shader_program_id == 0)
  {
    spdlog::error("Error creating shader program.");
    exit(1);
  }

  std::string vertex_shader_source, fragment_shader_source;

  if (!readFile(vertex_shader, vertex_shader_source))
  {
    exit(1);
  }
  spdlog::debug("Vertex shader:\n{}", vertex_shader_source);
  addShader(shader_program_id, vertex_shader_source, GL_VERTEX_SHADER);

  if (!readFile(fragment_shader, fragment_shader_source))
  {
    exit(1);
  }
  spdlog::debug("Fragment shader:\n{}", fragment_shader_source);
  addShader(shader_program_id, fragment_shader_source, GL_FRAGMENT_SHADER);

  GLint success{ 0 };
  GLchar info[1024]{ 0 };
  glLinkProgram(shader_program_id);
  glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
  if (success == 0)
  {
    glGetProgramInfoLog(shader_program_id, 1024, NULL, info);
    spdlog::error("Failed to link shaders, got error:\n{}", info);
    exit(1);
  }

  // GLOBALS.u_scale_location = glGetUniformLocation(shader_program_id, "u_scale");
  // if (GLOBALS.u_scale_location == -1)
  // {
  //     spdlog::error("Failed to get the location of 'u_scale' for the vertex shader.");
  //     exit(1);
  // }

  // GLOBALS.u_time_loc = glGetUniformLocation(shader_program_id, "u_time");
  // if (GLOBALS.u_time_loc == -1)
  // {
  //     spdlog::error("Failed to get the location of 'u_time' for the vertex shader.");
  //     exit(1);
  // }

  // GLOBALS.u_transform_loc = glGetUniformLocation(shader_program_id, "u_transform");
  // if (GLOBALS.u_transform_loc == -1)
  // {
  //     spdlog::error("Failed to get the location of 'u_time' for the vertex shader.");
  //     exit(1);
  // }

  glValidateProgram(shader_program_id);
  glGetProgramiv(shader_program_id, GL_VALIDATE_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shader_program_id, 1024, NULL, info);
    spdlog::error("Incompatible shader program, got error:\n{}", info);
    exit(1);
  }

  glUseProgram(shader_program_id);
}
}  // namespace util
}  // namespace emu