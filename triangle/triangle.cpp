#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../common/read_shaders.h"

int main(void)
{
  // ============================================================ WINDOW

  glfwInit();
  if (!glfwInit())
  {
    fprintf(stderr, "Error initializing glfw\n");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenglC", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "Error creating window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glewInit();
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Error initializing glew\n");
    return -1;
  }

  // ============================================================ WINDOW

  Shaders shader;
  shader.vertex_shader_path = "./shader.vert";
  shader.fragment_shader_path = "./shader.frag";
  unsigned int shader_program = shader.generate_shader();

  glLinkProgram(shader_program);

  // Verticies of the triangle
  float verticies[] =
  {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };

  // Create VBO and VAO
  unsigned int VBO, VAO;
  // Bind VAO first then bind and set vertex buffes, next configure vertex attribs
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Unbind the VAO after so other VAO calls won't modify this one
  glBindVertexArray(0);

  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
      && glfwWindowShouldClose(window) == 0)
  {
    // Set the background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program and draw the triangle
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers and pollIO events (keys pressed, mouse moved, etc)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Delete everything afterwards
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader_program);
  glfwTerminate();

  return 0;
}
