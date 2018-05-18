#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>

int main() {
    if (!glewInit()) {
        throw std::runtime_error("Cannot init Glew");
    }
    if (!glfwInit()) {
        throw std::runtime_error("Cannot init Glfw");
    }
    GLFWwindow *window = glfwCreateWindow(300, 300, "Openverse", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.0f, 0.0f);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
