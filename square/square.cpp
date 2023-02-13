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

  // Instead write 2 triangles like this
  float vertices[] =
  {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
  };

  unsigned int indices[] =
  {  // note that we start from 0!
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };

  // Create Element buffer object, VAO, AND VBO
  unsigned int VBO, VAO, EBO;
  // Bind VAO first then bind and set vertex buffes, next configure vertex attribs
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap buffers and poll IO events (keys pressed, mouse moved, etc)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Delete everything afterwards
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader_program);
  glfwTerminate();

  return 0;
}
