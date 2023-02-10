#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shaders
{
    public:
        const char* fragment_shader_path;
        const char* vertex_shader_path;

        void validate_shaders(GLuint shader, const char* file);
        GLuint generate_shader();
};
