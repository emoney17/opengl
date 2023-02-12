#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "shaders.hpp"

static const GLfloat g_vertex_buffer_data[] = {
  -1.0f,-1.0f,-1.0f, // triangle 1 : begin
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f, // triangle 1 : end
   1.0f, 1.0f,-1.0f, // triangle 2 : begin
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f, // triangle 2 : end
   1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,
   1.0f,-1.0f,-1.0f,
   1.0f, 1.0f,-1.0f,
   1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
   1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
   1.0f,-1.0f, 1.0f,
   1.0f, 1.0f, 1.0f,
   1.0f,-1.0f,-1.0f,
   1.0f, 1.0f,-1.0f,
   1.0f,-1.0f,-1.0f,
   1.0f, 1.0f, 1.0f,
   1.0f,-1.0f, 1.0f,
   1.0f, 1.0f, 1.0f,
   1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
   1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
   1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
   1.0f,-1.0f, 1.0f
};

// Two UV coordinatesfor each vertex.
static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f-0.000004f,
		0.000103f, 1.0f-0.336048f,
		0.335973f, 1.0f-0.335903f,
		1.000023f, 1.0f-0.000013f,
		0.667979f, 1.0f-0.335851f,
		0.999958f, 1.0f-0.336064f,
		0.667979f, 1.0f-0.335851f,
		0.336024f, 1.0f-0.671877f,
		0.667969f, 1.0f-0.671889f,
		1.000023f, 1.0f-0.000013f,
		0.668104f, 1.0f-0.000013f,
		0.667979f, 1.0f-0.335851f,
		0.000059f, 1.0f-0.000004f,
		0.335973f, 1.0f-0.335903f,
		0.336098f, 1.0f-0.000071f,
		0.667979f, 1.0f-0.335851f,
		0.335973f, 1.0f-0.335903f,
		0.336024f, 1.0f-0.671877f,
		1.000004f, 1.0f-0.671847f,
		0.999958f, 1.0f-0.336064f,
		0.667979f, 1.0f-0.335851f,
		0.668104f, 1.0f-0.000013f,
		0.335973f, 1.0f-0.335903f,
		0.667979f, 1.0f-0.335851f,
		0.335973f, 1.0f-0.335903f,
		0.668104f, 1.0f-0.000013f,
		0.336098f, 1.0f-0.000071f,
		0.000103f, 1.0f-0.336048f,
		0.000004f, 1.0f-0.671870f,
		0.336024f, 1.0f-0.671877f,
		0.000103f, 1.0f-0.336048f,
		0.336024f, 1.0f-0.671877f,
		0.335973f, 1.0f-0.335903f,
		0.667969f, 1.0f-0.671889f,
		1.000004f, 1.0f-0.671847f,
		0.667979f, 1.0f-0.335851f
};

int main(void)
{
  // ============================= BOILERPLATE

  glewExperimental = true;
  if (!glfwInit())
  {
      fprintf(stderr, "Error initializing glfw\n");
      return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(1024, 768, "GLFW in C", NULL, NULL);
  if (window == NULL)
  {
      fprintf(stderr, "Error creating window\n");
      glfwTerminate();
      return -1;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
      fprintf(stderr, "Error initializing glew\n");
      return -1;
  }

  // ============================= BOILERPLATE

  GLuint VertexArrayID; // create a vertex array object
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID); // set it as the current one

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  Shaders shader;
  shader.vertex_shader_path = "./shader.vert";
  shader.fragment_shader_path = "./shader.frag";
  GLuint program_ID = shader.generate_shader();

  glLinkProgram(program_ID); // link the program
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // set the clear color to blue

  glm::mat4 Projection = glm::perspective(glm::radians(45.0f),
      4.0f / 3.0f, 1.0f, 100.0f);

  glm::mat4 View = glm::lookAt(
          glm::vec3{4, 3, 3},
          glm::vec3{0, 0, 0},
          glm::vec3{0, 1, 0});

  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 mvp = Projection * View * Model;

  GLuint MatrixID = glGetUniformLocation(program_ID, "MVP");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // load some textures with stb
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  int width, height, nrChannels;
  unsigned char* data = stbi_load("./uvtemplate.tga", &width, &height, &nrChannels, 0);
  if (data)
  {
    // generate and bind the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    fprintf(stderr, "Error loading the texture\n");
    return -1;
  }
  stbi_image_free(data);

  GLuint Texture_ID = glGetUniformLocation(program_ID, "myTextureSampler");

  GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
  {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(program_ID);
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      // set myTextureSampler to use texture unit 0
      glUniform1i(Texture_ID, 0);

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		  // 2nd attribute buffer : UVs
		  glEnableVertexAttribArray(1);
		  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      // draw the triangle
      glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteProgram(program_ID);
  glDeleteTextures(1, &texture);
  glDeleteVertexArrays(1, &VertexArrayID);

  glfwTerminate();

  return 0;
}
