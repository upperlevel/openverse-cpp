#include "world_renderer.h"

// -------------------------------------------------------------------------------------------------------------------------------- ClientWorld

ClientWorld::ClientWorld(std::string name, std::shared_ptr<Socket> socket) : World(name), socket(std::weak_ptr(socket)) {
    // Todo: register chunks packet listeners
}
