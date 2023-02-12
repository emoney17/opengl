#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
  // ============================================================ WINDOW

  glewExperimental = true; // needed for core profile
  if (!glfwInit())
  {
      fprintf(stderr, "Error initializing glfw\n");
      return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  GLFWwindow* window;
  window = glfwCreateWindow(1024, 768, "GLFW in C", NULL, NULL);
  if (window == NULL)
  {
      fprintf(stderr, "Error creating window\n");
      glfwTerminate();
      return -1;
  }

  glfwMakeContextCurrent(window); // initialize glew
  glewExperimental = true; // needed in core profile
  if (glewInit() != GLEW_OK)
  {
      fprintf(stderr, "Error initializing glew\n");
      return -1;
  }
  // ============================================================ WINDOW

  do
  {
      glClear(GL_COLOR_BUFFER_BIT); // clear the screen
      glfwSwapBuffers(window); // swap buffers
      glfwPollEvents();
  }

  // ============================================================ MAIN LOOP
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
  return 0;
}
