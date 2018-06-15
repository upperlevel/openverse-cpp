#include <iostream>

#include <thread>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "protocol.h"
#include "server.h"

int main() {
    std::cout << "Enjoy the game" << std::endl;

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
    std::cout << "Client: Setting up network" << std::endl;
    auto [server_socket, client_socket] = create_socket_pair(OPENVERSE_PROTOCOL);
    auto server_conn = server_socket->get_connection();
    auto client_conn = client_socket->get_connection();

    // Setup listeners before connecting to the server.
    // It's important to be done before the connection because the server is async.
    client_socket->set_handler(*PacketTypes::PLAYER_CHANGE_WORLD, [&](auto &connection, auto packet) {
        std::string world_name = *std::static_pointer_cast<std::string>(packet);
        std::cout << "Client: World name: " << world_name << std::endl;
    });

    // server start
    Openverse::Server ov_server(*server_socket);
    std::thread server_thread([&ov_server]{
        std::cout << "Client: Async server started" << std::endl;
        ov_server.start();
    });
    server_thread.join();

    client_conn->send(*PacketTypes::LOGIN, std::make_shared<std::string>("maurizio"));

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
