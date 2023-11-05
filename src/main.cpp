#include <SFML/Graphics.hpp>
#include "crt.h"
#include "loop.h"
#include "mainapp.h"

int main()
{
    CLoop Loop;
    CMainApp MainLoop(Loop);
    auto window = sf::RenderWindow{ { 800u, 600u }, "CMake SFML Project" };
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    // Start Emulator Thread
    Loop.Start();
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        MainLoop.getCRT().RenderScreen(window);
    }
    Loop.Stop();
}