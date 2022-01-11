#include "emu/shader.h"

#include <GL/glew.h>

#include "spdlog/spdlog.h"
#include "emu/util.h"

namespace emu
{
Shader::Shader(const std::string& vert_shader_filename, const std::string& frag_shader_filename)
{
  shader_program_id_ = glCreateProgram();

  if (shader_program_id_ == 0)
  {
    spdlog::error("Error creating shader program.");
    exit(1);
  }

  std::string vertex_shader_source, fragment_shader_source;

  if (!readFile(vert_shader_filename, vertex_shader_source))
  {
    exit(1);
  }
  spdlog::debug("Vertex shader:\n{}", vertex_shader_source);
  addShader(shader_program_id_, vertex_shader_source, GL_VERTEX_SHADER);

  if (!readFile(frag_shader_filename, fragment_shader_source))
  {
    exit(1);
  }
  spdlog::debug("Fragment shader:\n{}", fragment_shader_source);
  addShader(shader_program_id_, fragment_shader_source, GL_FRAGMENT_SHADER);

  glLinkProgram(shader_program_id_);
  checkProgramStatus(shader_program_id_, GL_LINK_STATUS);

  glValidateProgram(shader_program_id_);
  checkProgramStatus(shader_program_id_, GL_VALIDATE_STATUS);
}

void Shader::use()
{
  glUseProgram(shader_program_id_);
}

void Shader::checkProgramStatus(GLuint program_id, GLenum status) const
{
  GLint success{ 0 };
  glGetProgramiv(program_id, status, &success);
  if (!success)
  {
    GLchar info[1024]{ 0 };
    glGetProgramInfoLog(shader_program_id_, 1024, NULL, info);
    spdlog::error("Invalid shader status, got error:\n{}", info);
    exit(1);
  }
}

void Shader::addShader(GLuint program, const std::string& source, GLenum type) const
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
  glDeleteShader(shader);
}
}  // namespace emu
