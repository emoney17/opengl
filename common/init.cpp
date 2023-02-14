#include "init.h"

Init::Init(int w, int h, const char* name)
{
    width = w;
    height = h;
    title = name;
}

void Init::glfw_init()
{
  glfwInit();
  if (!glfwInit())
  {
    fprintf(stderr, "Error initializing glfw\n");
    exit(-1);
  }
}

void Init::glew_init()
{
  glewInit();
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Error initializing glew\n");
    exit(-1);
  }
}

GLFWwindow* Init::create_window()
{
  glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, title, NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "Error creating window\n");
    glfwTerminate();
    exit(-1);
  }
  return window;
}

