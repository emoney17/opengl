#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* vertex_shader = // create vertex shader in file
"#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n" // this is the input data
"layout(location = 1) in vec3 vertexColor;\n"
"out vec3 fragmentColor;\n" // output data
"uniform mat4 MVP;\n"
"void main(){\n"
"   gl_Position = MVP * vec4(vertexPosition_modelspace, 1);\n"
"   fragmentColor = vertexColor;\n" // forward to fragment shader
"}\n";

const char* fragment_shader = // create fragment shader in file
"#version 330 core\n"
"in vec3 fragmentColor;\n"
"out vec3 color;\n"
"void main(){\n"
"   color = fragmentColor;\n" // output color and color specified in the vertex shader
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

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
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

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

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

    GLuint VertexArrayID; // create a vertex array object
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID); // set it as the current one

    GLuint vertexbuffer; // this will identify our vertex buffers
    glGenBuffers(1, &vertexbuffer); // generate 1 buffer, put the identifier in vertexbuffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // make the following apply to the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // give the verticies to opengl

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

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

    /*
     * Step 1 generate mvp matrix
     */

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

    // Z buffer to handle depth so far triangles do not go over near ones
    glEnable(GL_DEPTH_TEST); // enable depth test
    glDepthFunc(GL_LESS); // accept fragment if it is closer to the camera than the former one

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen and depth and color

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

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // starting from vertex 0; 3 verticies total -> 1 triangle
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window); // swap buffers
        glfwPollEvents();
    }
    // check if esc was pressed or window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    return 0;
}
