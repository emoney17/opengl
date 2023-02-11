#include "shaders.hpp"

void Shaders::validate_shaders(GLuint shader, const char* file)
{
  static const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  GLsizei length = 0;

  glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

  if (length > 0)
  {
      printf("Shader %d(%s) compile error: %s\n", shader, (file ? file : ""), buffer);
  }
}

GLuint Shaders::generate_shader()
{
  std::string vertex_shader_code;
  std::ifstream vertex_shader_stream(vertex_shader_path, std::ios::in);
  if (vertex_shader_stream.is_open())
  {
      std::stringstream ss;
      ss << vertex_shader_stream.rdbuf();
      vertex_shader_code = ss.str();
      vertex_shader_stream.close();
  }
  else
  {
     printf("Error reading %s", vertex_shader_path);
  }

  std::string fragment_shader_code;
  std::ifstream fragment_shader_stream(fragment_shader_path, std::ios::in);
  if (fragment_shader_stream.is_open())
  {
      std::stringstream ss;
      ss << fragment_shader_stream.rdbuf();
      fragment_shader_code = ss.str();
      fragment_shader_stream.close();
  }
  else
  {
     printf("Error reading %s", fragment_shader_path);
  }

  const char* vertex_shader = vertex_shader_code.c_str();
  const char* fragment_shader = fragment_shader_code.c_str();

  GLuint program_ID = glCreateProgram();

  {
      //Create vertex shader
      GLuint shader_VP = glCreateShader(GL_VERTEX_SHADER);

      glShaderSource(shader_VP, 1, &vertex_shader, 0);
      glCompileShader(shader_VP);
      validate_shaders(shader_VP, vertex_shader);
      glAttachShader(program_ID, shader_VP);

      glDeleteShader(shader_VP);
  }

  {
      //Create fragment shader
      GLuint shader_FP = glCreateShader(GL_FRAGMENT_SHADER);

      glShaderSource(shader_FP, 1, &fragment_shader, 0);
      glCompileShader(shader_FP);
      validate_shaders(shader_FP, fragment_shader);
      glAttachShader(program_ID, shader_FP);

      glDeleteShader(shader_FP);
  }

  return program_ID;
}
