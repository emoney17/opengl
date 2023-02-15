#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

class Init
{
  public:
    int width;
    int height;
    const char* title;

    Init(int w, int h, const char* name);
    void glfw_init();
    void glew_init();
    GLFWwindow* create_window();
};
