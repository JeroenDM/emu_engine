#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"
#include "emu/util.h"

GLuint VBO;

static void createAndFillVertexBuffer()
{
    // Vector3 vertices[3];
    std::array<Vector3, 3> vertices;
    vertices[0] = Vector3(-0.5, 0.0, 0.0);
    vertices[1] = Vector3(0.5, 0.0, 0.0);
    vertices[2] = Vector3(0.0, 1.0, 0.0);

    glGenBuffers(1, &VBO);              // create handle
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind handle to array buffer target
    // write data to the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
}

void updateVertexBuffer(float x_pos, float y_pos)
{
    // Vector3 vertices[3];
    std::array<Vector3, 3> vertices;
    vertices[0] = Vector3(-0.5, 0.0, 0.0);
    vertices[1] = Vector3(x_pos, y_pos, 0.0);
    vertices[2] = Vector3(0.0, 1.0, 0.0);

    // glGenBuffers(1, &VBO);              // create handle
    // glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind handle to array buffer target
    // write data to the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
}

void render()
{
    // draw vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    // specify how to interpret vertex buffer data
    // index, number of elements, type, normalize, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // primitive type, index of the first vertex to draw, number of vertices to process
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

int main(void)
{
    spdlog::set_level(spdlog::level::debug);

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        spdlog::error("Failed to create opengl context.");
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        spdlog::error("Failed to initialize glew.");
    }

    spdlog::info("{}", glGetString(GL_VERSION));

    createAndFillVertexBuffer();
    glClearColor(0.0, 0.0, 1.0, 1.0);

    // util::compileShaders("../shaders/shader.vert", "../shaders/shader.frag");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        updateVertexBuffer(0.5, 0.0);
        render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}