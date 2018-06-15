#pragma once

#include <string>

#include "world.h"
#include "network.h"


class ClientWorld : public World {
private:
    // The socket to use to register chunk packets listeners.
    std::weak_ptr<Socket> socket;

public:
    ClientWorld(std::string name, std::shared_ptr<Socket> socket);
};
