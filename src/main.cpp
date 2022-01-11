#include <array>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"
#include "Eigen/Core"

#include "emu/shader.h"

class Triangle
{
  GLuint array_buffer_id_;
  std::array<Eigen::Vector3f, 3> vertices_;

public:
  Triangle()
  {
    using Eigen::Vector3f;
    vertices_[0] = Vector3f(-0.5, 0.0, 0.0);
    vertices_[1] = Vector3f(0.5, 0.0, 0.0);
    vertices_[2] = Vector3f(0.0, 1.0, 0.0);

    glGenBuffers(1, &array_buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(), GL_STATIC_DRAW);
  }

  void update(float x_pos, float y_pos)
  {
    using Eigen::Vector3f;
    vertices_[1] = Vector3f(x_pos, y_pos, 0.0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(), GL_STATIC_DRAW);
  }

  void draw()
  {
    // draw vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_id_);
    glEnableVertexAttribArray(0);
    // specify how to interpret vertex buffer data
    // index, number of elements, type, normalize, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // primitive type, index of the first vertex to draw, number of vertices to process
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
  }
};
using TrianglePtr = std::unique_ptr<Triangle>;

class Engine
{
  GLFWwindow* window_;
  std::vector<TrianglePtr> shapes_;

  void input()
  {
  }

  void update()
  {
    for (auto& shape : shapes_)
    {
      shape->update(0.5, 0.0);
    }
  }

  void render()
  {
    glClear(GL_COLOR_BUFFER_BIT);
    for (auto& shape : shapes_)
    {
      shape->draw();
    }
  }

public:
  Engine()
  {
    /* Initialize the library */
    if (!glfwInit())
      exit(1);

    /* Create a windowed mode window and its OpenGL context */
    window_ = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window_)
    {
      glfwTerminate();
      spdlog::error("Failed to create opengl context.");
      exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window_);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
      spdlog::error("Failed to initialize glew.");
    }

    spdlog::info("{}", glGetString(GL_VERSION));
  }

  void run()
  {
    shapes_.push_back(std::make_unique<Triangle>());
    glClearColor(0.0, 0.0, 1.0, 1.0);

    emu::Shader shader("../shaders/shader.vert", "../shaders/shader.frag");
    shader.use();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window_))
    {
      input();
      update();
      render();

      /* Swap front and back buffers */
      glfwSwapBuffers(window_);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }

  ~Engine()
  {
    glfwTerminate();
  }
};

int main(void)
{
  //   spdlog::set_level(spdlog::level::debug);

  Engine e;
  e.run();

  return 0;
}