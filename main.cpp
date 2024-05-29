#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "yaml.h"
#include "shader.h"

const int WIDTH = 800;
const int HEIGHT = 600;

void resize(GLFWwindow *window, int width, int height);
void input(GLFWwindow *window);
void cursor(GLFWwindow *window, double xpos, double ypos);

struct Position
{
  double x;
  double y;
};

struct Size
{
  int width;
  int height;
};

class App
{
public:
  Position cursor;
  Size size;
  YAML *tokens; // pointer to YAML object for static purposes

  App(int width, int height)
  {
    size.width = width;
    size.height = height;
  }
};

static App app(WIDTH, HEIGHT);

int main(int argc, char *argv[])
{
  bool debug = false;
  if (argc > 1 && !strcmp(argv[1], "--debug"))
    debug = true;

  YAML yaml("tests/yaml/1.yaml");
  yaml.process();
  if (debug)
  {
    std::ofstream tokens("tokens.json");
    if (!tokens.is_open())
    {
      std::cout << "ERROR::DEBUG::WRITE_FAILED" << std::endl;
      return -1;
    }

    tokens << "[";
    for (auto i = 0; i < yaml.lexer.tokens.size(); i++)
    {
      tokens << yaml.lexer.tokens[i];
      if (i != yaml.lexer.tokens.size() - 1)
        tokens << ", ";
    }
    tokens << "]";

    tokens.close();

    std::ofstream ast("ast.json");
    if (!ast.is_open())
    {
      std::cout << "ERROR::DEBUG::WRITE_FAILED" << std::endl;
      return -1;
    }

    for (auto i = 0; i < yaml.parser.ast.size(); i++)
    {
      std::cout << yaml.parser.ast.at(i).toString() << std::endl;
    }

    ast.close();
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "yaml-visualizer", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resize);

  // Load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  while (!glfwWindowShouldClose(window))
  {
    input(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void input(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void cursor(GLFWwindow *window, double xpos, double ypos)
{
  app.cursor.x = xpos;
  app.cursor.y = ypos;
}

void resize(GLFWwindow *window, int width, int height)
{
  app.size.width = width;
  app.size.height = height;
  glViewport(0, 0, width, height);
}