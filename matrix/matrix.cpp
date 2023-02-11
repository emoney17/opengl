#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

const char* vertex_shader = // create vertex shader in file
"#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n" // this is the input data
"uniform mat4 MVP;\n"
"void main(){\n"
"   gl_Position = MVP * vec4(vertexPosition_modelspace, 1);\n"
// "   gl_Position.xyz = vertexPosition_modelspace;\n" // set vertex position to whatever was in the buffer
// "   gl_Position.w = 1.0;\n"
"}\n";

const char* fragment_shader = // create fragment shader in file
"#version 330 core\n"
"out vec3 color;\n"
"void main(){\n"
"   color = vec3(1,0,0);\n" // set each fragment to red
"}\n";

void validateShader(GLuint shader, const char* file)
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

int main(void)
{
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
    window = glfwCreateWindow(1024, 768, "Matracies", NULL, NULL);
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

    GLuint VertexArrayID; // create a vertex array object
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID); // set it as the current one

    static const GLfloat g_vertex_buffer_data[] = // set the coordinates of the verticies
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLuint vertexbuffer; // this will identify our vertex buffers
    glGenBuffers(1, &vertexbuffer); // generate 1 buffer, put the identifier in vertexbuffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // make the following apply to the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // give the verticies to opengl

    // create shaders
    GLuint program_ID = glCreateProgram();

    {
        //Create vertex shader
        GLuint shader_VP = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shader_VP, 1, &vertex_shader, 0);
        glCompileShader(shader_VP);
        validateShader(shader_VP, vertex_shader);
        glAttachShader(program_ID, shader_VP);

        glDeleteShader(shader_VP);
    }

    {
        //Create fragment shader
        GLuint shader_FP = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shader_FP, 1, &fragment_shader, 0);
        glCompileShader(shader_FP);
        validateShader(shader_FP, fragment_shader);
        glAttachShader(program_ID, shader_FP);

        glDeleteShader(shader_FP);
    }

    glLinkProgram(program_ID); // link the program
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // set the clear color to blue

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f),
        4.0f / 3.0f, 1.0f, 100.0f);

    // camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3{4, 3, 3}, // camera is at 4,3,3 in world space
            glm::vec3{0, 0, 0}, // and looks up at origin
            glm::vec3{0, 1, 0}); // head is up (set to 0, -1, 0 to make upsidedown)
    // model matrix, model will be at origin
    glm::mat4 Model = glm::mat4(1.0f); // identity matrix
    // model view projection: multiplication of the 3 matricies
    glm::mat4 mvp = Projection * View * Model;

    /*
     * Step 2 give it to glsl
     */

    // get handle for our mpv uniform
    // only during initializing
    GLuint MatrixID = glGetUniformLocation(program_ID, "MVP");

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

        glUseProgram(program_ID);

        // send our transformation to the currently bounded shader in mVP uniform
        // this is done in the main loop since each model with have different mvp matrix
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer
        (
           0,                     // attribute 0
           3,                     // size
           GL_FLOAT,              // type
           GL_FALSE,              // normalized?
           0,                     // stride
           (void*)0               // array buffer offset
        );

        // draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3); // starting from vertex 0; 3 verticies total -> 1 triangle
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window); // swap buffers
        glfwPollEvents();
    }
    // check if esc was pressed or window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    return 0;
}
