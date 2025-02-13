#include "game.h"

int main(int argc, char ** argv)
{

    Game game{"Planetoids++", 1024,768};


    if (!game.Initialize()) {
        std::cout << "ERROR: Game window could not be initialized." << std::endl;
        return -1;
    }

    game.RunLoop();

    game.ShutDown();

    return 0;
}
