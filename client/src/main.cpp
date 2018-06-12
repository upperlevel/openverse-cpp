#include <iostream>

#include <thread>

#include "protocol.h"
#include "server.h"

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

    // Window setup
    GLFWwindow *window = glfwCreateWindow(300, 300, "Openverse", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwShowWindow(window);

    // Network setup
    // Todo now it's only singleplayer
    std::cout << "Setting up network" << std::endl;
    auto [server_socket, client_socket] = create_socket_pair(OPENVERSE_PROTOCOL);
    auto server_conn = server_socket->get_connection();
    auto client_conn = client_socket->get_connection();

    Openverse::Server ov_server(*server_socket);
    std::thread server_thread([&ov_server]{
        std::cout << "Starting server async..." << std::endl;
        ov_server.start();
    });
    server_thread.join();

    // Game cycle
    std::cout << "Starting game cycle" << std::endl;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.0f, 0.0f);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
