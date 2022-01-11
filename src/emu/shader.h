#pragma once

#include <string>
#include <fstream>

#include <GL/glew.h>

namespace emu
{
class Shader
{
  GLuint shader_program_id_;
  GLuint vert_shader_id_;
  GLuint frag_shader_id_;

  void checkProgramStatus(GLuint program_id, GLenum status) const;
  void addShader(GLuint program, const std::string& source, GLenum type) const;

public:
  Shader(const std::string& vert_shader_filename, const std::string& frag_shader_filename);
  void use();
};
}  // namespace emu