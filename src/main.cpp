#include <array>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"
#include "Eigen/Core"

#include "emu/shader.h"

class Vertex
{
  Eigen::Vector3f position;
  Eigen::Vector3f color;

public:
  Vertex() = default;
  Vertex(float x, float y, float z)
  {
    position << x, y, z;
    color << x, y, z;
  }
};

class Triangle
{
  GLuint array_buffer_id_;
  GLuint index_buffer_id_;

  std::array<Vertex, 3> vertices_;
  std::array<unsigned int, 3> indices_;

public:
  Triangle()
  {
    vertices_[0] = Vertex(-0.5, 0.0, 0.0);
    vertices_[1] = Vertex(0.5, 0.0, 0.0);
    vertices_[2] = Vertex(0.0, 1.0, 0.0);

    glGenBuffers(1, &array_buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(), GL_STATIC_DRAW);

    indices_ = { 0, 1, 2 };

    glGenBuffers(1, &index_buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_.data(), GL_STATIC_DRAW);
  }

  void update(float x_pos, float y_pos)
  {
    vertices_[1] = Vertex(x_pos, y_pos, 0.0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(), GL_STATIC_DRAW);
  }

  void draw()
  {
    // draw vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // specify how to interpret vertex buffer data
    // index, number of elements, type, normalize, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // primitive type, index of the first vertex to draw, number of vertices to process
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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