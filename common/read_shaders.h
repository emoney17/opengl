#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shaders
{
  const char* fragment_shader_path;
  const char* vertex_shader_path;
  public:
    Shaders(const char* f_path, const char* v_path);
    void validate_shaders(GLuint shader, const char* file);
    GLuint generate_shader();
};
