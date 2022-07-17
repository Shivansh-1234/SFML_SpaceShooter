#include <SFML/Graphics.hpp>
#include "Application.h"

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    Application::instance = new Application();

    Application::instance->run();

    delete Application::instance;

    return EXIT_SUCCESS;
}