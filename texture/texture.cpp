#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../common/read_shaders.h"
#include "../common/init.h"

int main(void)
{
  //============================================================WINDOW

  Init init;
  init.width = 800;
  init.height = 600;
  init.title = "TEXTURE";
  init.glfw_init();
  GLFWwindow* window = init.create_window();
  glfwMakeContextCurrent(window);
  init.glew_init();

  //============================================================SHADERS

  Shaders shader;
  shader.vertex_shader_path = "./shader.vert";
  shader.fragment_shader_path = "./shader.frag";
  unsigned int shader_program = shader.generate_shader();

  glLinkProgram(shader_program);

  //============================================================BUFFERS

  // Now include vertex data with the texture coordinates
  float vertices[] =
  {
     // positions         // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,     // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,     // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,     // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f      // top left
  };

  unsigned int indices[] =
  {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Notify opengl of the new vertex format
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Texture coordinates attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  //============================================================TEXTURES

  // Create an ID for the textures
  unsigned int texture1, texture2;
  int width, height, nrChannels;
  unsigned char* data;

  // Tell stb_image.h to flip loaded textures on the y-axis
  stbi_set_flip_vertically_on_load(true);

  // Texture 1
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // Set texture wrapping/filtering options on the currently bound texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load texture data from file
  data = stbi_load("./background.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    // Generate texture from the data as well as the mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    fprintf(stderr, "Error loading texture\n");
    return -1;
  }
  // Afterwards free image data
  stbi_image_free(data);

  // Texture 2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // Set texture wrapping/filtering options on the currently bound texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load texture data from file
  data = stbi_load("./konata.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    // Set RGBA instead of just RGB so that we can controll the alpha value in shader.frag
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    fprintf(stderr, "Error loading texture\n");
    return -1;
  }
  // Afterwards free image data
  stbi_image_free(data);

  // Activate shader before settings uniform for the two textures
  glUseProgram(shader_program);
  glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
  glUniform1i(glGetUniformLocation(shader_program, "texture2"), 1);

  //============================================================GAMELOOP

  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
      && glfwWindowShouldClose(window) == 0)
  {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);

    // Bind the texture before drawing elements
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader_program);
  glfwTerminate();

  return 0;
}
